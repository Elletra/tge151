//-----------------------------------------------------------------------------
// Torque Game Engine
// Copyright (C) GarageGames.com, Inc.
//-----------------------------------------------------------------------------

#include "audio/audio.h"
#include "audio/audioDataBlock.h"
#include "core/tVector.h"
#include "console/console.h"
#include "console/consoleTypes.h"
#include "game/gameConnection.h"
#include "core/fileStream.h"
#include "audio/audioStreamSourceFactory.h"

#ifdef TORQUE_OS_MAC
//#define REL_WORKAROUND
#endif

//-------------------------------------------------------------------------
#define MAX_AUDIOSOURCES      16                // maximum number of concurrent sources
#define MIN_GAIN              0.05f             // anything with lower gain will not be started
#define MIN_UNCULL_PERIOD     500               // time before buffer is checked to be unculled
#define MIN_UNCULL_GAIN       0.1f              // min gain of source to be unculled

#define ALX_DEF_SAMPLE_RATE      44100          // default values for mixer
#define ALX_DEF_SAMPLE_BITS      16
#define ALX_DEF_CHANNELS         2

#define FORCED_OUTER_FALLOFF  10000.f           // forced falloff distance
static bool mDisableOuterFalloffs = false;      // forced max falloff?
static F32 mInnerFalloffScale = 1.f;            // amount to scale inner falloffs

static ALCdevice *mDevice   = NULL;             // active OpenAL device
static ALCcontext *mContext = NULL;             // active OpenAL context
F32 mAudioTypeVolume[Audio::NumAudioTypes];     // the attenuation for each of the channel types

//-------------------------------------------------------------------------
struct LoopingImage
{
   AUDIOHANDLE             mHandle;
   Resource<AudioBuffer>   mBuffer;
   Audio::Description      mDescription;
   AudioSampleEnvironment *mEnvironment;

   Point3F                 mPosition;
   Point3F                 mDirection;
   F32                     mPitch;
   F32                     mScore;
   U32                     mCullTime;

   LoopingImage()  { clear(); }

   void clear()
   {
      mHandle           = NULL_AUDIOHANDLE;
      mBuffer           = NULL;
      dMemset(&mDescription, 0, sizeof(Audio::Description));
      mEnvironment = 0;
      mPosition.set(0.f,0.f,0.f);
      mDirection.set(0.f,1.f,0.f);
      mPitch = 1.f;
      mScore = 0.f;
      mCullTime = 0;
   }
};

//-------------------------------------------------------------------------
static F32 mMasterVolume = 1.f;           // traped from AL_LISTENER gain (miles has difficulties with 3d sources)

static ALuint                 mSource[MAX_AUDIOSOURCES];                   // ALSources
static AUDIOHANDLE            mHandle[MAX_AUDIOSOURCES];                   // unique handles
static Resource<AudioBuffer>  mBuffer[MAX_AUDIOSOURCES];                   // each of the playing buffers (needed for AudioThread)
static F32                    mScore[MAX_AUDIOSOURCES];                    // for figuring out which sources to cull/uncull
static F32                    mSourceVolume[MAX_AUDIOSOURCES];             // the samples current un-attenuated gain (not scaled by master/channel gains)
static U32                    mType[MAX_AUDIOSOURCES];                     // the channel which this source belongs

static AudioSampleEnvironment*        mSampleEnvironment[MAX_AUDIOSOURCES];           // currently playing sample environments
static bool                           mEnvironmentEnabled = false;                    // environment enabled?
static SimObjectPtr<AudioEnvironment> mCurrentEnvironment;                            // the last environment set

static ALuint mEnvironment = 0;                        // al environment handle

struct LoopingList : VectorPtr<LoopingImage*>
{
   LoopingList() : VectorPtr<LoopingImage*>(__FILE__, __LINE__) { }

   LoopingList::iterator findImage(AUDIOHANDLE handle);
   void sort();
};

struct StreamingList : VectorPtr<AudioStreamSource*>
{
   StreamingList() : VectorPtr<AudioStreamSource*>(__FILE__, __LINE__) { }

   StreamingList::iterator findImage(AUDIOHANDLE handle);
   void sort();
};

// LoopingList and LoopingFreeList own the images
static LoopingList mLoopingList;                 // all the looping sources
static LoopingList mLoopingFreeList;             // free store
static LoopingList mLoopingInactiveList;         // sources which have not been played yet
static LoopingList mLoopingCulledList;           // sources which have been culled (alxPlay called)

// StreamingList and StreamingFreeList own the images
static StreamingList mStreamingList;                 // all the streaming sources
//static StreamingList mStreamingFreeList;             // free store
static StreamingList mStreamingInactiveList;         // sources which have not been played yet
static StreamingList mStreamingCulledList;           // sources which have been culled (alxPlay called)

#define AUDIOHANDLE_LOOPING_BIT  (0x80000000)
#define AUDIOHANDLE_STREAMING_BIT  (0x40000000)
#define AUDIOHANDLE_INACTIVE_BIT (0x20000000)
#define AUDIOHANDLE_LOADING_BIT  (0x10000000)
#define HANDLE_MASK             ~(AUDIOHANDLE_LOOPING_BIT | AUDIOHANDLE_INACTIVE_BIT | AUDIOHANDLE_LOADING_BIT)

// keep the 'AUDIOHANDLE_LOOPING_BIT' on the handle returned to the caller so that
// the handle can quickly be rejected from looping list queries
#define RETURN_MASK             ~(AUDIOHANDLE_INACTIVE_BIT | AUDIOHANDLE_LOADING_BIT)
static AUDIOHANDLE mLastHandle = NULL_AUDIOHANDLE;

static bool mForceMaxDistanceUpdate = false;       // force gain setting for 3d distances
static U32  mNumSources = 0;                       // total number of sources to work with
static U32  mRequestSources = MAX_AUDIOSOURCES;    // number of sources to request from openAL

#define INVALID_SOURCE        0xffffffff

inline bool areEqualHandles(AUDIOHANDLE a, AUDIOHANDLE b)
{
   return((a & HANDLE_MASK) == (b & HANDLE_MASK));
}

//-------------------------------------------------------------------------
// Looping image
//-------------------------------------------------------------------------
inline LoopingList::iterator LoopingList::findImage(AUDIOHANDLE handle)
{
   if(handle & AUDIOHANDLE_LOOPING_BIT)
   {
      LoopingList::iterator itr = begin();
      while(itr != end())
      {
         if(areEqualHandles((*itr)->mHandle, handle))
            return(itr);
         itr++;
      }
   }
   return(0);
}

inline int QSORT_CALLBACK loopingImageSort(const void * p1, const void * p2)
{
   const LoopingImage * ip1 = *(const LoopingImage**)p1;
   const LoopingImage * ip2 = *(const LoopingImage**)p2;

   // min->max
   return ip2->mScore - ip1->mScore;
}

void LoopingList::sort()
{
   dQsort(address(), size(), sizeof(LoopingImage*), loopingImageSort);
}

//-------------------------------------------------------------------------
// StreamingList
//-------------------------------------------------------------------------
inline StreamingList::iterator StreamingList::findImage(AUDIOHANDLE handle)
{
   if(handle & AUDIOHANDLE_STREAMING_BIT)
   {
      StreamingList::iterator itr = begin();
      while(itr != end())
      {
         if(areEqualHandles((*itr)->mHandle, handle))
            return(itr);
         itr++;
      }
   }
   return(0);
}

inline int QSORT_CALLBACK streamingSourceSort(const void * p1, const void * p2)
{
   const AudioStreamSource * ip1 = *(const AudioStreamSource**)p1;
   const AudioStreamSource * ip2 = *(const AudioStreamSource**)p2;

   // min->max
   return ip2->mScore - ip1->mScore;
}

void StreamingList::sort()
{
   dQsort(address(), size(), sizeof(AudioStreamSource*), streamingSourceSort);
}

//-------------------------------------------------------------------------
LoopingImage * createLoopingImage()
{
   LoopingImage *image;
   if (mLoopingFreeList.size())
   {
      image = mLoopingFreeList.last();
      mLoopingFreeList.pop_back();
   }
   else
      image = new LoopingImage;
   return(image);
}

//-------------------------------------------------------------------------
AudioStreamSource * createStreamingSource(const char* filename)
{
   AudioStreamSource *streamSource = AudioStreamSourceFactory::getNewInstance(filename);
   return(streamSource);
}

//-------------------------------------------------------------------------
static AUDIOHANDLE getNewHandle()
{
   mLastHandle++;
   mLastHandle &= HANDLE_MASK;
   if (mLastHandle == NULL_AUDIOHANDLE)
      mLastHandle++;
   return mLastHandle;
}


//-------------------------------------------------------------------------
// function declarations
void alxLoopingUpdate();
void alxStreamingUpdate();
void alxUpdateScores(bool);

static bool findFreeSource(U32 *index)
{
   for(U32 i = 0; i < mNumSources; i++)
      if(mHandle[i] == NULL_AUDIOHANDLE)
      {
         *index = i;
         return(true);
      }
   return(false);
}

//--------------------------------------------------------------------------
// - cull out the min source that is below volume
// - streams/voice/loading streams are all scored > 2
// - volumes are attenuated by channel only
static bool cullSource(U32 *index, F32 volume)
{
   alGetError();

   F32 minVolume = volume;
   S32 best = -1;
   for(S32 i = 0; i < mNumSources; i++)
   {
      if(mScore[i] < minVolume)
      {
         minVolume = mScore[i];
         best = i;
      }
   }

   if(best == -1)
      return(false);

   // check if culling a looper
   LoopingList::iterator itr = mLoopingList.findImage(mHandle[best]);
   if(itr)
   {
      // check if culling an inactive looper
      if(mHandle[best] & AUDIOHANDLE_INACTIVE_BIT)
      {
         AssertFatal(!mLoopingInactiveList.findImage(mHandle[best]), "cullSource: image already in inactive list");
         AssertFatal(!mLoopingCulledList.findImage(mHandle[best]), "cullSource: image should not be in culled list");
         mLoopingInactiveList.push_back(*itr);
      }
      else
      {
         (*itr)->mHandle |= AUDIOHANDLE_INACTIVE_BIT;
         AssertFatal(!mLoopingCulledList.findImage(mHandle[best]), "cullSource: image already in culled list");
         AssertFatal(!mLoopingInactiveList.findImage(mHandle[best]), "cullSource: image should no be in inactive list");
         (*itr)->mCullTime = Platform::getRealMilliseconds();
         mLoopingCulledList.push_back(*itr);
      }
   }

   // check if culling a streamer
   StreamingList::iterator itr2 = mStreamingList.findImage(mHandle[best]);
   if(itr2)
   {
      // check if culling an inactive streamer
      if(mHandle[best] & AUDIOHANDLE_INACTIVE_BIT)
      {
         AssertFatal(!mStreamingInactiveList.findImage(mHandle[best]), "cullSource: image already in inactive list");
         AssertFatal(!mStreamingCulledList.findImage(mHandle[best]), "cullSource: image should not be in culled list");
         mStreamingInactiveList.push_back(*itr2);
      }
      else
      {
         (*itr2)->mHandle |= AUDIOHANDLE_INACTIVE_BIT;
         AssertFatal(!mStreamingCulledList.findImage(mHandle[best]), "cullSource: image already in culled list");
         AssertFatal(!mStreamingInactiveList.findImage(mHandle[best]), "cullSource: image should no be in inactive list");
         (*itr2)->freeStream();
		 (*itr2)->mCullTime = Platform::getRealMilliseconds();
         mStreamingCulledList.push_back(*itr2);
      }
   }

   alSourceStop(mSource[best]);
   mHandle[best] = NULL_AUDIOHANDLE;
   mBuffer[best] = 0;
   *index = best;

   return(true);
}

//--------------------------------------------------------------------------
/**   Compute approximate max volume at a particular distance
      ignore cone volume influnces
*/
static F32 approximate3DVolume(const Audio::Description *desc, const Point3F &position)
{
   Point3F p1;
   alGetListener3f(AL_POSITION, &p1.x, &p1.y, &p1.z);

   p1 -= position;
   F32 distance = p1.magnitudeSafe();

   if(distance >= desc->mMaxDistance)
      return(0.f);
   else if(distance < desc->mReferenceDistance)
      return 1.0f;
   else
      return 1.0 - (distance - desc->mReferenceDistance) / (desc->mMaxDistance - desc->mReferenceDistance);
}

//--------------------------------------------------------------------------
inline U32 alxFindIndex(AUDIOHANDLE handle)
{
   for (U32 i=0; i<mNumSources; i++)
      if(mHandle[i] && areEqualHandles(mHandle[i], handle))
         return i;
   return MAX_AUDIOSOURCES;
}

//--------------------------------------------------------------------------
ALuint alxFindSource(AUDIOHANDLE handle)
{
   for (U32 i=0; i<mNumSources; i++)
      if(mHandle[i] && areEqualHandles(mHandle[i], handle))
         return mSource[i];
   return(INVALID_SOURCE);
}


//--------------------------------------------------------------------------
/**   Determmine if an AUDIOHANDLE is valid.
      An AUDIOHANDLE is valid if it is a currently playing source, inactive source,
      or a looping source (basically anything where a alxSource??? call will succeed)
*/
bool alxIsValidHandle(AUDIOHANDLE handle)
{
   if(handle == NULL_AUDIOHANDLE)
      return(false);

   // inactive sources are valid
   U32 idx = alxFindIndex(handle);
   if(idx != MAX_AUDIOSOURCES)
   {
      if(mHandle[idx] & AUDIOHANDLE_INACTIVE_BIT)
         return(true);

      // if it is active but not playing then it has stopped...
      ALint state = AL_STOPPED;
      alGetSourcei(mSource[idx], AL_SOURCE_STATE, &state);
      return(state == AL_PLAYING);
   }

   if(mLoopingList.findImage(handle))
      return(true);

   if(mStreamingList.findImage(handle))
      return(true);

   return(false);
}

//--------------------------------------------------------------------------
/**   Determmine if an AUDIOHANDLE is currently playing
*/
bool alxIsPlaying(AUDIOHANDLE handle)
{
   if(handle == NULL_AUDIOHANDLE)
      return(false);

   U32 idx = alxFindIndex(handle);
   if(idx == MAX_AUDIOSOURCES)
      return(false);

   ALint state = 0;
   alGetSourcei(mSource[idx], AL_SOURCE_STATE, &state);
   return(state == AL_PLAYING);
}

//--------------------------------------------------------------------------
void alxEnvironmentDestroy()
{
/* todo
   if(mEnvironment)
   {
      alDeleteEnvironmentIASIG(1, &mEnvironment);
      mEnvironment = 0;
   }
*/
}

void alxEnvironmentInit()
{
/* todo
   alxEnvironmentDestroy();
   if(alIsExtensionPresent((const ALubyte *)"AL_EXT_IASIG"))
   {
      alGenEnvironmentIASIG(1, &mEnvironment);
      if(alGetError() != AL_NO_ERROR)
         mEnvironment = 0;
   }
*/
}

//--------------------------------------------------------------------------
// - setup a sources environmental effect settings
static void alxSourceEnvironment(ALuint source, F32 environmentLevel, AudioSampleEnvironment * env)
{
   // environment level is on the AudioDatablock
/* todo
   alSourcef(source, AL_ENV_SAMPLE_REVERB_MIX_EXT, environmentLevel);
*/
   if(!env)
      return;

/* todo
   alSourcei(source, AL_ENV_SAMPLE_DIRECT_EXT,                 env->mDirect);
   alSourcei(source, AL_ENV_SAMPLE_DIRECT_HF_EXT,              env->mDirectHF);
   alSourcei(source, AL_ENV_SAMPLE_ROOM_EXT,                   env->mRoom);
   alSourcei(source, AL_ENV_SAMPLE_ROOM_HF_EXT,                env->mRoomHF);
   alSourcei(source, AL_ENV_SAMPLE_OUTSIDE_VOLUME_HF_EXT,      env->mOutsideVolumeHF);
   alSourcei(source, AL_ENV_SAMPLE_FLAGS_EXT,                  env->mFlags);

   alSourcef(source, AL_ENV_SAMPLE_OBSTRUCTION_EXT,            env->mObstruction);
   alSourcef(source, AL_ENV_SAMPLE_OBSTRUCTION_LF_RATIO_EXT,   env->mObstructionLFRatio);
   alSourcef(source, AL_ENV_SAMPLE_OCCLUSION_EXT,              env->mOcclusion);
   alSourcef(source, AL_ENV_SAMPLE_OCCLUSION_LF_RATIO_EXT,     env->mOcclusionLFRatio);
   alSourcef(source, AL_ENV_SAMPLE_OCCLUSION_ROOM_RATIO_EXT,   env->mOcclusionRoomRatio);
   alSourcef(source, AL_ENV_SAMPLE_ROOM_ROLLOFF_EXT,           env->mRoomRolloff);
   alSourcef(source, AL_ENV_SAMPLE_AIR_ABSORPTION_EXT,         env->mAirAbsorption);
*/
}

static void alxSourceEnvironment(ALuint source, LoopingImage * image)
{
   AssertFatal(image, "alxSourceEnvironment: invalid looping image");
   if(image->mDescription.mIs3D)
      alxSourceEnvironment(source, image->mDescription.mEnvironmentLevel, image->mEnvironment);
}

static void alxSourceEnvironment(ALuint source, AudioStreamSource * image)
{
   AssertFatal(image, "alxSourceEnvironment: invalid looping image");
   if(image->mDescription.mIs3D)
      alxSourceEnvironment(source, image->mDescription.mEnvironmentLevel, image->mEnvironment);
}

//--------------------------------------------------------------------------
// setup a source to play... loopers have pitch cached
// - by default, pitch is 1x (settings not defined in description)
// - all the settings are cached by openAL (miles version), so no worries setting them here
static void alxSourcePlay(ALuint source, Resource<AudioBuffer> buffer, const Audio::Description *desc, const MatrixF *transform)
{
   alSourcei(source, AL_BUFFER, buffer->getALBuffer());
   alSourcef(source, AL_GAIN, Audio::linearToDB(desc->mVolume * mAudioTypeVolume[desc->mType] * mMasterVolume));
   alSourcei(source, AL_LOOPING, desc->mIsLooping ? AL_TRUE : AL_FALSE);
   alSourcef(source, AL_PITCH, 1.f);

   alSourcei(source, AL_CONE_INNER_ANGLE, desc->mConeInsideAngle);
   alSourcei(source, AL_CONE_OUTER_ANGLE, desc->mConeOutsideAngle);
   alSourcef(source, AL_CONE_OUTER_GAIN, desc->mConeOutsideVolume);

   if(transform != NULL)
   {
#ifdef REL_WORKAROUND
      alSourcei(source, AL_SOURCE_ABSOLUTE, AL_TRUE);
#else
      alSourcei(source, AL_SOURCE_RELATIVE, AL_FALSE);
#endif

      Point3F p;
      transform->getColumn(3, &p);
      alSource3f(source, AL_POSITION, p.x, p.y, p.z);

      //Always use ConeVector (which is tied to transform)
      alSource3f(source, AL_DIRECTION, desc->mConeVector.x, desc->mConeVector.y, desc->mConeVector.z);

   }
   else
   {
      // 2D sound
      alSourcei(source, AL_SOURCE_RELATIVE, AL_TRUE);
      alSource3f(source, AL_POSITION, 0.0f, 0.0f, 1.0f);
   }

   alSourcef(source, AL_REFERENCE_DISTANCE, desc->mReferenceDistance);
   alSourcef(source, AL_MAX_DISTANCE, desc->mMaxDistance);

/* todo
   // environmental audio stuff:
   alSourcef(source, AL_ENV_SAMPLE_REVERB_MIX_EXT, desc->mEnvironmentLevel);
   if(desc->mEnvironmentLevel != 0.f)
      alSourceResetEnvironment_EXT(source);
*/
}

// helper for looping images
static void alxSourcePlay(ALuint source, LoopingImage * image)
{
   AssertFatal(image, "alxSourcePlay: invalid looping image");

   // 3d source? need position/direction
   if(image->mDescription.mIs3D)
   {
      MatrixF transform(true);
      transform.setColumn(3, image->mPosition);
      transform.setRow(1, image->mDirection);
      alxSourcePlay(source, image->mBuffer, &image->mDescription, &transform);
   }
   else
   {
      // 2d source
      alxSourcePlay(source, image->mBuffer, &image->mDescription, 0);
   }
}

//--------------------------------------------------------------------------
// setup a streaming source to play
static void alxSourcePlay(AudioStreamSource *streamSource)
{
   ALuint source = streamSource->mSource;
   Audio::Description *desc = &streamSource->mDescription;

   bool ret = streamSource->initStream();

   alSourcef(source, AL_GAIN, Audio::linearToDB(desc->mVolume * mAudioTypeVolume[desc->mType] * mMasterVolume));
//   alSourcei(source, AL_LOOPING, AL_FALSE);
   alSourcef(source, AL_PITCH, 1.f);

   alSourcei(source, AL_CONE_INNER_ANGLE, desc->mConeInsideAngle);
   alSourcei(source, AL_CONE_OUTER_ANGLE, desc->mConeOutsideAngle);
   alSourcef(source, AL_CONE_OUTER_GAIN, desc->mConeOutsideVolume);

   if(streamSource->mDescription.mIs3D)
   {
      MatrixF transform(true);
      transform.setColumn(3, streamSource->mPosition);
      transform.setRow(1, streamSource->mDirection);

#ifdef REL_WORKAROUND
      alSourcei(source, AL_SOURCE_ABSOLUTE, AL_TRUE);
#else
      alSourcei(source, AL_SOURCE_RELATIVE, AL_FALSE);
#endif

      Point3F p;
      transform.getColumn(3, &p);
      alSource3f(source, AL_POSITION, p.x, p.y, p.z);

      //Always use ConeVector (which is tied to transform)
      alSource3f(source, AL_DIRECTION, desc->mConeVector.x, desc->mConeVector.y, desc->mConeVector.z);
   }
   else
   {
      // 2D sound
      // JMQ: slam the stream source's position to our desired value
      streamSource->mPosition = Point3F(0.0f, 0.0f, 1.0f);
      alSourcei(source, AL_SOURCE_RELATIVE, AL_TRUE);
      alSource3f(source, AL_POSITION,
                 streamSource->mPosition.x,
                 streamSource->mPosition.y,
                 streamSource->mPosition.z);
   }

   alSourcef(source, AL_REFERENCE_DISTANCE, desc->mReferenceDistance);
   alSourcef(source, AL_MAX_DISTANCE, desc->mMaxDistance);

/* todo
   // environmental audio stuff:
   alSourcef(source, AL_ENV_SAMPLE_REVERB_MIX_EXT, desc->mEnvironmentLevel);
   if(desc->mEnvironmentLevel != 0.f)
      alSourceResetEnvironment_EXT(source);
*/
}

//--------------------------------------------------------------------------
AUDIOHANDLE alxCreateSource(const Audio::Description *desc,
                            const char *filename,
                            const MatrixF *transform,
                            AudioSampleEnvironment *sampleEnvironment)
{
   if (!mContext)
      return NULL_AUDIOHANDLE;

   if(desc == NULL || filename == NULL || *filename == '\0')
      return NULL_AUDIOHANDLE;

   F32 volume = desc->mVolume;

   // calculate an approximate attenuation for 3d sounds
   if(transform && desc->mIs3D)
   {
      Point3F position;
      transform->getColumn(3, &position);
      volume *= approximate3DVolume(desc, position);
   }

   // check the type specific volume
   AssertFatal(desc->mType < Audio::NumAudioTypes, "alxCreateSource: invalid type for source");
   if(desc->mType >= Audio::NumAudioTypes)
      return(NULL_AUDIOHANDLE);

   // done if channel is muted (and not a looper)
   if(!desc->mIsLooping && !desc->mIsStreaming && (mAudioTypeVolume[desc->mType] == 0.f))
      return(NULL_AUDIOHANDLE);

   // scale volume by channel attenuation
   volume *= mAudioTypeVolume[desc->mType];

   // non-loopers don't add if < minvolume
   if(!desc->mIsLooping && !desc->mIsStreaming && (volume <= MIN_GAIN))
      return(NULL_AUDIOHANDLE);

   U32 index = MAX_AUDIOSOURCES;

   // try and find an available source: 0 volume loopers get added to inactive list
   if(volume > MIN_GAIN)
   {
      if(!findFreeSource(&index))
      {
         alxUpdateScores(true);

         // scores do not include master volume
         if(!cullSource(&index, volume))
            index = MAX_AUDIOSOURCES;
      }
   }

   // make sure that loopers are added
   if(index == MAX_AUDIOSOURCES)
   {
      if(desc->mIsLooping && !(desc->mIsStreaming))
      {
         Resource<AudioBuffer> buffer = AudioBuffer::find(filename);
         if(!(bool)buffer)
            return(NULL_AUDIOHANDLE);

         // create the inactive looping image
         LoopingImage * image = createLoopingImage();

         image->mHandle = getNewHandle() | AUDIOHANDLE_LOOPING_BIT | AUDIOHANDLE_INACTIVE_BIT;
         image->mBuffer = buffer;
         image->mDescription = *desc;
         image->mScore = volume;
         image->mEnvironment = sampleEnvironment;

         // grab position/direction if 3d source
         if(transform)
         {
            transform->getColumn(3, &image->mPosition);
            transform->getColumn(1, &image->mDirection);
         }

         AssertFatal(!mLoopingInactiveList.findImage(image->mHandle), "alxCreateSource: handle in inactive list");
         AssertFatal(!mLoopingCulledList.findImage(image->mHandle), "alxCreateSource: handle in culled list");

         // add to the looping and inactive lists
         mLoopingList.push_back(image);
         mLoopingInactiveList.push_back(image);
         return(image->mHandle & RETURN_MASK);
      }
      else
         return(NULL_AUDIOHANDLE);
   }

   // make sure that streamers are added
   if(index == MAX_AUDIOSOURCES)
   {
      if(desc->mIsStreaming)
      {
         // create the inactive audio stream
         AudioStreamSource * streamSource = createStreamingSource(filename);
         if (streamSource)
         {
            streamSource->mHandle = getNewHandle() | AUDIOHANDLE_STREAMING_BIT | AUDIOHANDLE_INACTIVE_BIT;
            streamSource->mSource = NULL;
            streamSource->mDescription = *desc;
            streamSource->mScore = volume;
            streamSource->mEnvironment = sampleEnvironment;

            // grab position/direction if 3d source
            if(transform)
            {
               transform->getColumn(3, &streamSource->mPosition);
               transform->getColumn(1, &streamSource->mDirection);
            }

            AssertFatal(!mStreamingInactiveList.findImage(streamSource->mHandle), "alxCreateSource: handle in inactive list");
            AssertFatal(!mStreamingCulledList.findImage(streamSource->mHandle), "alxCreateSource: handle in culled list");

            // add to the streaming and inactive lists
            mStreamingList.push_back(streamSource);
            mStreamingInactiveList.push_back(streamSource);
            return(streamSource->mHandle & RETURN_MASK);
         }
         else
            return NULL_AUDIOHANDLE;
      }
      else
         return(NULL_AUDIOHANDLE);
   }

   // clear the error state
   alGetError();

   // grab the buffer
   Resource<AudioBuffer> buffer;
   if(!(desc->mIsStreaming)) {
		buffer = AudioBuffer::find(filename);
		if((bool)buffer == false)
			return NULL_AUDIOHANDLE;
	}

   // init the source (created inactive) and store needed values
   mHandle[index] = getNewHandle() | AUDIOHANDLE_INACTIVE_BIT;
   mType[index] = desc->mType;
   if(!(desc->mIsStreaming)) {
	mBuffer[index] = buffer;
   }
   mScore[index] = volume;
   mSourceVolume[index] = desc->mVolume;
   mSampleEnvironment[index] = sampleEnvironment;

   ALuint source = mSource[index];

   // setup play info
   if(!desc->mIsStreaming)
      alxSourcePlay(source, buffer, desc, desc->mIs3D ? transform : 0);

   if(mEnvironmentEnabled)
      alxSourceEnvironment(source, desc->mEnvironmentLevel, sampleEnvironment);

   // setup a LoopingImage ONLY if the sound is a looper:
   if(desc->mIsLooping && !(desc->mIsStreaming))
   {
      mHandle[index] |= AUDIOHANDLE_LOOPING_BIT;

      LoopingImage * image = createLoopingImage();
      image->mHandle = mHandle[index];
      image->mBuffer = buffer;
      image->mDescription = *desc;
      image->mScore = volume;
      image->mEnvironment = sampleEnvironment;

      // grab position/direction
      if(transform)
      {
         transform->getColumn(3, &image->mPosition);
         transform->getColumn(1, &image->mDirection);
      }

      AssertFatal(!mLoopingInactiveList.findImage(image->mHandle), "alxCreateSource: handle in inactive list");
      AssertFatal(!mLoopingCulledList.findImage(image->mHandle), "alxCreateSource: handle in culled list");

      // add to the looping list
      mLoopingList.push_back(image);
   }

   // setup a AudioStreamSource ONLY if the sound is a streamer:
   if(desc->mIsStreaming)
   {
       // Intangir> why is loading bit never used anywhere else?
       // comes in handy for my oggmixedstream
       // (prevents it from being deleted before it is loaded)
       mHandle[index] |= AUDIOHANDLE_STREAMING_BIT | AUDIOHANDLE_LOADING_BIT;

      AudioStreamSource * streamSource = createStreamingSource(filename);
      if (streamSource)
      {
         streamSource->mHandle = mHandle[index];
         streamSource->mSource = mSource[index];
         streamSource->mDescription = *desc;
         streamSource->mScore = volume;
         streamSource->mEnvironment = sampleEnvironment;

         // grab position/direction
         if(transform)
         {
            transform->getColumn(3, &streamSource->mPosition);
            transform->getColumn(1, &streamSource->mDirection);
         }

         AssertFatal(!mStreamingInactiveList.findImage(streamSource->mHandle), "alxCreateSource: handle in inactive list");
         AssertFatal(!mStreamingCulledList.findImage(streamSource->mHandle), "alxCreateSource: handle in culled list");

         alxSourcePlay(streamSource);

         // add to the looping list
         mStreamingList.push_back(streamSource);
      }
      else
      {
         mSampleEnvironment[index] = 0;
         mHandle[index] = NULL_AUDIOHANDLE;
         mBuffer[index] = 0;
         return NULL_AUDIOHANDLE;
      }
   }

   // clear off all but looping bit
   return(mHandle[index] & RETURN_MASK);
}

//------------------------------------------------------------------------------
AUDIOHANDLE alxCreateSource(AudioDescription *descObject,
                            const char *filename,
                            const MatrixF *transform,
                            AudioSampleEnvironment * sampleEnvironment )
{
   if(!descObject || !descObject->getDescription())
      return(NULL_AUDIOHANDLE);
   return (alxCreateSource(descObject->getDescription(), filename, transform, sampleEnvironment));
}

AUDIOHANDLE alxCreateSource(const AudioProfile *profile, const MatrixF *transform)
{
   if (profile == NULL)
      return NULL_AUDIOHANDLE;

   return alxCreateSource(profile->mDescriptionObject, profile->mFilename, transform, profile->mSampleEnvironment);
}

//--------------------------------------------------------------------------
extern void threadPlay(AudioBuffer * buffer, AUDIOHANDLE handle);

AUDIOHANDLE alxPlay(AUDIOHANDLE handle)
{
   U32 index = alxFindIndex(handle);

   if(index != MAX_AUDIOSOURCES)
   {
      // play if not already playing
      if(mHandle[index] & AUDIOHANDLE_INACTIVE_BIT)
      {
			mHandle[index] &= ~(AUDIOHANDLE_INACTIVE_BIT | AUDIOHANDLE_LOADING_BIT);

         // make sure the looping image also clears it's inactive bit
         LoopingList::iterator itr = mLoopingList.findImage(handle);
         if(itr)
            (*itr)->mHandle &= ~(AUDIOHANDLE_INACTIVE_BIT | AUDIOHANDLE_LOADING_BIT);

         // make sure the streaming image also clears it's inactive bit
         StreamingList::iterator itr2 = mStreamingList.findImage(handle);
         if(itr2)
            (*itr2)->mHandle &= ~(AUDIOHANDLE_INACTIVE_BIT | AUDIOHANDLE_LOADING_BIT);

         alSourcePlay(mSource[index]);

         return(handle);
      }
   }
   else
   {
      // move inactive loopers to the culled list, try to start the sound
      LoopingList::iterator itr = mLoopingInactiveList.findImage(handle);
      if(itr)
      {
         AssertFatal(!mLoopingCulledList.findImage(handle), "alxPlay: image already in culled list");
         mLoopingCulledList.push_back(*itr);
         mLoopingInactiveList.erase_fast(itr);
         alxLoopingUpdate();
         return(handle);
      }
      else if(mLoopingCulledList.findImage(handle))
      {
         alxLoopingUpdate();
         return(handle);
      }
      else
         return(NULL_AUDIOHANDLE);

      // move inactive streamers to the culled list, try to start the sound
      StreamingList::iterator itr2 = mStreamingInactiveList.findImage(handle);
      if(itr2)
      {
         AssertFatal(!mStreamingCulledList.findImage(handle), "alxPlay: image already in culled list");
         (*itr2)->freeStream();
         mStreamingCulledList.push_back(*itr2);
         mStreamingInactiveList.erase_fast(itr2);
         alxStreamingUpdate();
         return(handle);
      }
      else if(mStreamingCulledList.findImage(handle))
      {
         alxStreamingUpdate();
         return(handle);
      }
      else
         return(NULL_AUDIOHANDLE);
   }

   return(handle);
}

//--------------------------------------------------------------------------
// helper function.. create a source and play it
AUDIOHANDLE alxPlay(const AudioProfile *profile, const MatrixF *transform, const Point3F* /*velocity*/)
{
   if(profile == NULL)
      return NULL_AUDIOHANDLE;

   AUDIOHANDLE handle = alxCreateSource(profile->mDescriptionObject, profile->mFilename, transform, profile->mSampleEnvironment);
   if(handle != NULL_AUDIOHANDLE)
      return(alxPlay(handle));
   return(handle);
}

//--------------------------------------------------------------------------
void alxStop(AUDIOHANDLE handle)
{
   U32 index = alxFindIndex(handle);

   // stop it
   if(index != MAX_AUDIOSOURCES)
   {
      if(!(mHandle[index] & AUDIOHANDLE_INACTIVE_BIT))
      {
         alSourceStop(mSource[index]);
      }

      mSampleEnvironment[index] = 0;
      mHandle[index] = NULL_AUDIOHANDLE;
      mBuffer[index] = 0;
   }

   // remove loopingImage and add it to the free list
   LoopingList::iterator itr = mLoopingList.findImage(handle);
   if(itr)
   {
      // remove from inactive/culled list
      if((*itr)->mHandle & AUDIOHANDLE_INACTIVE_BIT)
      {
         LoopingList::iterator tmp = mLoopingInactiveList.findImage(handle);

         // inactive?
         if(tmp)
            mLoopingInactiveList.erase_fast(tmp);
         else
         {
            //culled?
            tmp = mLoopingCulledList.findImage(handle);
            AssertFatal(tmp, "alxStop: failed to find inactive looping source");
            mLoopingCulledList.erase_fast(tmp);
         }
      }

      AssertFatal(!mLoopingInactiveList.findImage((*itr)->mHandle), "alxStop: handle in inactive list");
      AssertFatal(!mLoopingCulledList.findImage((*itr)->mHandle), "alxStop: handle in culled list");

      // remove it
      (*itr)->clear();
      mLoopingFreeList.push_back(*itr);
      mLoopingList.erase_fast(itr);
   }

   // remove streamingImage and add it to the free list
   StreamingList::iterator itr2 = mStreamingList.findImage(handle);
   if(itr2)
   {
      // remove from inactive/culled list
      if((*itr2)->mHandle & AUDIOHANDLE_INACTIVE_BIT)
      {
         StreamingList::iterator tmp = mStreamingInactiveList.findImage(handle);

         // inactive?
         if(tmp)
            mStreamingInactiveList.erase_fast(tmp);
         else
         {
            //culled?
            tmp = mStreamingCulledList.findImage(handle);
            AssertFatal(tmp, "alxStop: failed to find inactive looping source");
            mStreamingCulledList.erase_fast(tmp);
         }
      }

      AssertFatal(!mStreamingInactiveList.findImage((*itr2)->mHandle), "alxStop: handle in inactive list");
      AssertFatal(!mStreamingCulledList.findImage((*itr2)->mHandle), "alxStop: handle in culled list");

      // remove it
	  (*itr2)->freeStream();
      delete(*itr2);
      mStreamingList.erase_fast(itr2);
   }
}

//--------------------------------------------------------------------------
void alxStopAll()
{
   // stop all open sources
   for(S32 i = mNumSources - 1; i >= 0; i--)
      if(mHandle[i] != NULL_AUDIOHANDLE)
         alxStop(mHandle[i]);

   // stop all looping sources
   while(mLoopingList.size())
      alxStop(mLoopingList.last()->mHandle);

// stop all streaming sources
   while(mStreamingList.size())
      alxStop(mStreamingList.last()->mHandle);
}

void alxLoopSourcef(AUDIOHANDLE handle, ALenum pname, ALfloat value)
{
   LoopingList::iterator itr = mLoopingList.findImage(handle);
   if(itr)
   {
      switch(pname)
      {
         case AL_GAIN:
            (*itr)->mDescription.mVolume = Audio::DBToLinear(value);
            break;
         case AL_GAIN_LINEAR:
            (*itr)->mDescription.mVolume = value;
            break;
         case AL_PITCH:
            (*itr)->mPitch = value;
            break;
         case AL_REFERENCE_DISTANCE:
            (*itr)->mDescription.mReferenceDistance = value;
            break;
         case AL_CONE_OUTER_GAIN:
            (*itr)->mDescription.mMaxDistance = value;
            break;
      }
   }
}

void alxLoopSource3f(AUDIOHANDLE handle, ALenum pname, ALfloat value1, ALfloat value2, ALfloat value3)
{
   LoopingList::iterator itr = mLoopingList.findImage(handle);
   if(itr)
   {
      switch(pname)
      {
         case AL_POSITION:
            (*itr)->mPosition.x = value1;
            (*itr)->mPosition.y = value2;
            (*itr)->mPosition.z = value3;
            break;

         case AL_DIRECTION:
            (*itr)->mDirection.x = value1;
            (*itr)->mDirection.y = value2;
            (*itr)->mDirection.z = value3;
            break;
      }
   }
}

void alxLoopSourcei(AUDIOHANDLE handle, ALenum pname, ALint value)
{
   LoopingList::iterator itr = mLoopingList.findImage(handle);
   if(itr)
   {
      switch(pname)
      {
         //case AL_SOURCE_AMBIENT:
         //   (*itr)->mDescription.mIs3D = value;
         //   break;
         case AL_CONE_INNER_ANGLE:
            (*itr)->mDescription.mConeInsideAngle = value;
            break;
         case AL_CONE_OUTER_ANGLE:
            (*itr)->mDescription.mConeOutsideAngle = value;
            break;
      }
   }
}

void alxLoopGetSourcef(AUDIOHANDLE handle, ALenum pname, ALfloat *value)
{
   LoopingList::iterator itr = mLoopingList.findImage(handle);
   if(itr)
   {
      switch(pname)
      {
         case AL_GAIN:
            *value = Audio::linearToDB((*itr)->mDescription.mVolume);
            break;
         case AL_GAIN_LINEAR:
            *value = (*itr)->mDescription.mVolume;
            break;
         case AL_PITCH:
            *value = (*itr)->mPitch;
            break;
         case AL_REFERENCE_DISTANCE:
            *value = (*itr)->mDescription.mReferenceDistance;
            break;
         case AL_CONE_OUTER_GAIN:
            *value = (*itr)->mDescription.mMaxDistance;
            break;
      }
   }
}

void alxLoopGetSource3f(AUDIOHANDLE handle, ALenum pname, ALfloat *value1, ALfloat *value2, ALfloat *value3)
{
   LoopingList::iterator itr = mLoopingList.findImage(handle);
   if(itr)
   {
      switch(pname)
      {
         case AL_POSITION:
            *value1 = (*itr)->mPosition.x;
            *value2 = (*itr)->mPosition.y;
            *value3 = (*itr)->mPosition.z;
            break;

         case AL_DIRECTION:
            *value1 = (*itr)->mDirection.x;
            *value2 = (*itr)->mDirection.y;
            *value3 = (*itr)->mDirection.z;
            break;
      }
   }
}

void alxLoopGetSourcei(AUDIOHANDLE handle, ALenum pname, ALint *value)
{
   LoopingList::iterator itr = mLoopingList.findImage(handle);
   if(itr)
   {
      switch(pname)
      {
         //case AL_SOURCE_AMBIENT:
         //   *value = (*itr)->mDescription.mIs3D;
         //   break;
         case AL_LOOPING:
            *value = true;
            break;
         case AL_CONE_INNER_ANGLE:
            *value = (*itr)->mDescription.mConeInsideAngle;
            break;
         case AL_CONE_OUTER_ANGLE:
            *value = (*itr)->mDescription.mConeOutsideAngle;
            break;
      }
   }
}

//------------------------------------------------------

void alxStreamSourcef(AUDIOHANDLE handle, ALenum pname, ALfloat value)
{
   StreamingList::iterator itr = mStreamingList.findImage(handle);
   if(itr)
   {
      switch(pname)
      {
         case AL_GAIN:
            (*itr)->mDescription.mVolume = Audio::DBToLinear(value);
            break;
         case AL_GAIN_LINEAR:
            (*itr)->mDescription.mVolume = value;
            break;
         case AL_PITCH:
            (*itr)->mPitch = value;
            break;
         case AL_REFERENCE_DISTANCE:
            (*itr)->mDescription.mReferenceDistance = value;
            break;
         case AL_CONE_OUTER_GAIN:
            (*itr)->mDescription.mMaxDistance = value;
            break;
      }
   }
}

void alxStreamSource3f(AUDIOHANDLE handle, ALenum pname, ALfloat value1, ALfloat value2, ALfloat value3)
{
   StreamingList::iterator itr = mStreamingList.findImage(handle);
   if(itr)
   {
      switch(pname)
      {
         case AL_POSITION:
            (*itr)->mPosition.x = value1;
            (*itr)->mPosition.y = value2;
            (*itr)->mPosition.z = value3;
            break;

         case AL_DIRECTION:
            (*itr)->mDirection.x = value1;
            (*itr)->mDirection.y = value2;
            (*itr)->mDirection.z = value3;
            break;
      }
   }
}

void alxStreamSourcei(AUDIOHANDLE handle, ALenum pname, ALint value)
{
   StreamingList::iterator itr = mStreamingList.findImage(handle);
   if(itr)
   {
      switch(pname)
      {
         //case AL_SOURCE_AMBIENT:
         //   (*itr)->mDescription.mIs3D = value;
         //   break;
         case AL_CONE_INNER_ANGLE:
            (*itr)->mDescription.mConeInsideAngle = value;
            break;
         case AL_CONE_OUTER_ANGLE:
            (*itr)->mDescription.mConeOutsideAngle = value;
            break;
      }
   }
}

void alxStreamGetSourcef(AUDIOHANDLE handle, ALenum pname, ALfloat *value)
{
   StreamingList::iterator itr = mStreamingList.findImage(handle);
   if(itr)
   {
      switch(pname)
      {
         case AL_GAIN:
            *value = Audio::linearToDB((*itr)->mDescription.mVolume);
            break;
         case AL_GAIN_LINEAR:
            *value = (*itr)->mDescription.mVolume;
            break;
         case AL_PITCH:
            *value = (*itr)->mPitch;
            break;
         case AL_REFERENCE_DISTANCE:
            *value = (*itr)->mDescription.mReferenceDistance;
            break;
         case AL_CONE_OUTER_GAIN:
            *value = (*itr)->mDescription.mMaxDistance;
            break;
      }
   }
}

void alxStreamGetSource3f(AUDIOHANDLE handle, ALenum pname, ALfloat *value1, ALfloat *value2, ALfloat *value3)
{
   StreamingList::iterator itr = mStreamingList.findImage(handle);
   if(itr)
   {
      switch(pname)
      {
         case AL_POSITION:
            *value1 = (*itr)->mPosition.x;
            *value2 = (*itr)->mPosition.y;
            *value3 = (*itr)->mPosition.z;
            break;

         case AL_DIRECTION:
            *value1 = (*itr)->mDirection.x;
            *value2 = (*itr)->mDirection.y;
            *value3 = (*itr)->mDirection.z;
            break;
      }
   }
}

void alxStreamGetSourcei(AUDIOHANDLE handle, ALenum pname, ALint *value)
{
   StreamingList::iterator itr = mStreamingList.findImage(handle);
   if(itr)
   {
      switch(pname)
      {
         //case AL_SOURCE_AMBIENT:
         //   *value = (*itr)->mDescription.mIs3D;
         //   break;
         case AL_LOOPING:
            *value = true;
            break;
         case AL_CONE_INNER_ANGLE:
            *value = (*itr)->mDescription.mConeInsideAngle;
            break;
         case AL_CONE_OUTER_ANGLE:
            *value = (*itr)->mDescription.mConeOutsideAngle;
            break;
      }
   }
}

//--------------------------------------------------------------------------
// AL get/set methods: Source
//--------------------------------------------------------------------------
// - only need to worry about playing sources.. proper volume gets set on
//   create source (so, could get out of sync if someone changes volume between
//   a createSource and playSource call...)
void alxUpdateTypeGain(U32 type)
{
   for(U32 i = 0; i < mNumSources; i++)
   {
      if(mHandle[i] == NULL_AUDIOHANDLE)
         continue;

      if(type != mType[i])
         continue;

      ALint state = AL_STOPPED;
      alGetSourcei(mSource[i], AL_SOURCE_STATE, &state);

      if(state == AL_PLAYING)
      {
         // volume = SourceVolume * ChannelVolume * MasterVolume
         F32 vol = mClampF(mSourceVolume[i] * mAudioTypeVolume[mType[i]] * mMasterVolume, 0.f, 1.f);
         alSourcef(mSource[i], AL_GAIN, Audio::linearToDB(vol) );
      }
   }
}

void alxSourcef(AUDIOHANDLE handle, ALenum pname, ALfloat value)
{
   ALuint source = alxFindSource(handle);

   if(source != INVALID_SOURCE)
   {
      // ensure gain_linear
      if(pname == AL_GAIN)
      {
         value = Audio::DBToLinear(value);
         pname = AL_GAIN_LINEAR;
      }

      // need to process gain settings (so source can be affected by channel/master gains)
      if(pname == AL_GAIN_LINEAR)
      {
         U32 idx = alxFindIndex(handle);
         AssertFatal(idx != MAX_AUDIOSOURCES, "alxSourcef: handle not located for found source");
         if(idx == MAX_AUDIOSOURCES)
            return;

         // update the stored value
         mSourceVolume[idx] = value;

         // volume = SourceVolume * ChannelVolume * MasterVolume

// #ifdef REL_WORKAROUND
//          ALint val = AL_TRUE;
//          alGetSourcei(source, AL_SOURCE_ABSOLUTE, &val);
//          if(val == AL_FALSE)
// #else
//          ALint val = AL_FALSE;
//          alGetSourcei(source, AL_SOURCE_RELATIVE, &val);
//          if(val == AL_TRUE)
// #endif
         {
            F32 vol = mClampF(mSourceVolume[idx] * mAudioTypeVolume[mType[idx]] * mMasterVolume, 0.f, 1.f);
            alSourcef(source, AL_GAIN, Audio::linearToDB(vol) );
         }
      }
      else
         alSourcef(source, pname, value);
   }
   alxLoopSourcef(handle, pname, value);
   alxStreamSourcef(handle, pname, value);
}

void alxSourcefv(AUDIOHANDLE handle, ALenum pname, ALfloat *values)
{
   ALuint source = alxFindSource(handle);
   if(source != INVALID_SOURCE)
      alSourcefv(source, pname, values);

   if((pname == AL_POSITION) || (pname == AL_DIRECTION) || (pname == AL_VELOCITY)) {
      alxLoopSource3f(handle, pname, values[0], values[1], values[2]);
      alxStreamSource3f(handle, pname, values[0], values[1], values[2]);
	}
}

void alxSource3f(AUDIOHANDLE handle, ALenum pname, ALfloat value1, ALfloat value2, ALfloat value3)
{
   ALuint source = alxFindSource(handle);
   if(source != INVALID_SOURCE)
   {
      ALfloat values[3];
      values[0] = value1;
      values[1] = value2;
      values[2] = value3;
      alSourcefv(source, pname, values);
   }
   alxLoopSource3f(handle, pname, value1, value2, value3);
   alxStreamSource3f(handle, pname, value1, value2, value3);
}

void alxSourcei(AUDIOHANDLE handle, ALenum pname, ALint value)
{
   ALuint source = alxFindSource(handle);
   if(source != INVALID_SOURCE)
      alSourcei(source, pname, value);
   alxLoopSourcei(handle, pname, value);
   alxStreamSourcei(handle, pname, value);
}

// sets the position and direction of the source
void alxSourceMatrixF(AUDIOHANDLE handle, const MatrixF *transform)
{
   ALuint source = alxFindSource(handle);

   Point3F pos;
   transform->getColumn(3, &pos);

   Point3F dir;
   transform->getColumn(1, &dir);

   if(source != INVALID_SOURCE)
   {
      // OpenAL uses a Right-Handed corrdinate system so flip the orientation vector
      alSource3f(source, AL_POSITION, pos.x, pos.y, pos.z);
      alSource3f(source, AL_DIRECTION, -dir.x, -dir.y, -dir.z);
   }

   alxLoopSource3f(handle, AL_POSITION, pos.x, pos.y, pos.z);
   alxLoopSource3f(handle, AL_DIRECTION, dir.x, dir.y, dir.z);
   alxStreamSource3f(handle, AL_POSITION, pos.x, pos.y, pos.z);
   alxStreamSource3f(handle, AL_DIRECTION, dir.x, dir.y, dir.z);
}

//--------------------------------------------------------------------------
void alxGetSourcef(AUDIOHANDLE handle, ALenum pname, ALfloat *value)
{
   ALuint source = alxFindSource(handle);
   if(source != INVALID_SOURCE)
   {
      // gain queries return unattenuated values
      if((pname == AL_GAIN) || (pname == AL_GAIN_LINEAR))
      {
         U32 idx = alxFindIndex(handle);
         AssertFatal(idx != MAX_AUDIOSOURCES, "alxGetSourcef: found source but handle is invalid");
         if(idx == MAX_AUDIOSOURCES)
         {
            *value = 0.f;
            return;
         }

         if(pname == AL_GAIN)
            *value = Audio::linearToDB(mSourceVolume[idx]);
         else
            *value = mSourceVolume[idx];
      }
      else
         alGetSourcef(source, pname, value);
   }
   else if(handle & AUDIOHANDLE_LOOPING_BIT)
      alxLoopGetSourcef(handle, pname, value);
   else
	  alxStreamGetSourcef(handle, pname, value);
}

void alxGetSourcefv(AUDIOHANDLE handle, ALenum pname, ALfloat *values)
{
   if((pname == AL_POSITION) || (pname == AL_DIRECTION) || (pname == AL_VELOCITY))
      alxGetSource3f(handle, pname, &values[0], &values[1], &values[2]);
}

void alxGetSource3f(AUDIOHANDLE handle, ALenum pname, ALfloat *value1, ALfloat *value2, ALfloat *value3)
{
   ALuint source = alxFindSource(handle);
   if(source != INVALID_SOURCE)
   {
      ALfloat values[3];
      alGetSourcefv(source, pname, values);
      *value1 = values[0];
      *value2 = values[1];
      *value3 = values[2];
   }
   else if(handle & AUDIOHANDLE_LOOPING_BIT)
      alxLoopGetSource3f(handle, pname, value1, value2, value3);
   else
	  alxStreamGetSource3f(handle, pname, value1, value2, value3);
}

void alxGetSourcei(AUDIOHANDLE handle, ALenum pname, ALint *value)
{
   ALuint source = alxFindSource(handle);
   if(source != INVALID_SOURCE)
      alGetSourcei(source, pname, value);
   else if(handle & AUDIOHANDLE_LOOPING_BIT)
      alxLoopGetSourcei(handle, pname, value);
   else
	  alxStreamGetSourcei(handle, pname, value);
}


//--------------------------------------------------------------------------
/**   alListenerfv extension for use with MatrixF's
      Set the listener's position and orientation using a matrix
*/
void alxListenerMatrixF(const MatrixF *transform)
{
   Point3F p1, p2;
   transform->getColumn(3, &p1);
   alListener3f(AL_POSITION, p1.x, p1.y, p1.z);

   transform->getColumn(2, &p1);    // Up Vector
   transform->getColumn(1, &p2);    // Forward Vector

   F32 orientation[6];
   orientation[0] = -p1.x;
   orientation[1] = -p1.y;
   orientation[2] = -p1.z;
   orientation[3] = p2.x;
   orientation[4] = p2.y;
   orientation[5] = p2.z;

   alListenerfv(AL_ORIENTATION, orientation);
}

//--------------------------------------------------------------------------
/**   alListenerf extension supporting linear gain
*/
void alxListenerf(ALenum param, ALfloat value)
{
   if (param == AL_GAIN_LINEAR)
   {
      value = Audio::linearToDB(value);
      param = AL_GAIN;
   }
   alListenerf(param, value);
}

//--------------------------------------------------------------------------
/**   alGetListenerf extension supporting linear gain
*/
void alxGetListenerf(ALenum param, ALfloat *value)
{
   if (param == AL_GAIN_LINEAR)
   {
      alGetListenerf(AL_GAIN, value);
      *value = Audio::DBToLinear(*value);
   }
   else
      alGetListenerf(param, value);
}


//--------------------------------------------------------------------------
// Simple metrics
//--------------------------------------------------------------------------


#ifdef TORQUE_GATHER_METRICS
static void alxGatherMetrics()
{
   S32 mNumOpenHandles              = 0;
   S32 mNumOpenLoopingHandles       = 0;
   S32 mNumOpenStreamingHandles     = 0;

   S32 mNumActiveStreams            = 0;
   S32 mNumNullActiveStreams        = 0;
   S32 mNumActiveLoopingStreams     = 0;
   S32 mNumActiveStreamingStreams   = 0;

   S32 mNumLoopingStreams           = 0;
   S32 mNumInactiveLoopingStreams   = 0;
   S32 mNumCulledLoopingStreams     = 0;
   S32 mNumStreamingStreams           = 0;
   S32 mNumInactiveStreamingStreams   = 0;
   S32 mNumCulledStreamingStreams     = 0;

   // count installed streams and open handles
   for(U32 i = 0; i < mNumSources; i++)
   {
      if(mHandle[i] != NULL_AUDIOHANDLE)
      {
         mNumOpenHandles++;
         if(mHandle[i] & AUDIOHANDLE_LOOPING_BIT)
            mNumOpenLoopingHandles++;
		 if(mHandle[i] & AUDIOHANDLE_STREAMING_BIT)
            mNumOpenStreamingHandles++;
      }

      ALint state = AL_STOPPED;
      alGetSourcei(mSource[i], AL_SOURCE_STATE, &state);
      if(state == AL_PLAYING)
      {
         mNumActiveStreams++;
         if(mHandle[i] == NULL_AUDIOHANDLE)
            mNumNullActiveStreams++;
         if(mHandle[i] & AUDIOHANDLE_LOOPING_BIT)
            mNumActiveLoopingStreams++;
		 if(mHandle[i] & AUDIOHANDLE_STREAMING_BIT)
            mNumActiveStreamingStreams++;
      }
   }

   for(LoopingList::iterator itr = mLoopingList.begin(); itr != mLoopingList.end(); itr++)
      mNumLoopingStreams++;
   for(LoopingList::iterator itr = mLoopingInactiveList.begin(); itr != mLoopingInactiveList.end(); itr++)
      mNumInactiveLoopingStreams++;
   for(LoopingList::iterator itr = mLoopingCulledList.begin(); itr != mLoopingCulledList.end(); itr++)
      mNumCulledLoopingStreams++;

   for(StreamingList::iterator itr = mStreamingList.begin(); itr != mStreamingList.end(); itr++)
      mNumStreamingStreams++;
   for(StreamingList::iterator itr = mStreamingInactiveList.begin(); itr != mStreamingInactiveList.end(); itr++)
      mNumInactiveStreamingStreams++;
   for(StreamingList::iterator itr = mStreamingCulledList.begin(); itr != mStreamingCulledList.end(); itr++)
      mNumCulledStreamingStreams++;

   Con::setIntVariable("Audio::numOpenHandles",             mNumOpenHandles);
   Con::setIntVariable("Audio::numOpenLoopingHandles",      mNumOpenLoopingHandles);
   Con::setIntVariable("Audio::numOpenStreamingHandles",      mNumOpenStreamingHandles);

   Con::setIntVariable("Audio::numActiveStreams",           mNumActiveStreams);
   Con::setIntVariable("Audio::numNullActiveStreams",       mNumNullActiveStreams);
   Con::setIntVariable("Audio::numActiveLoopingStreams",    mNumActiveLoopingStreams);
   Con::setIntVariable("Audio::numActiveStreamingStreams",    mNumActiveStreamingStreams);

   Con::setIntVariable("Audio::numLoopingStreams",          mNumLoopingStreams);
   Con::setIntVariable("Audio::numInactiveLoopingStreams",  mNumInactiveLoopingStreams);
   Con::setIntVariable("Audio::numCulledLoopingStreams",    mNumCulledLoopingStreams);

   Con::setIntVariable("Audio::numStreamingStreams",          mNumStreamingStreams);
   Con::setIntVariable("Audio::numInactiveStreamingStreams",  mNumInactiveStreamingStreams);
   Con::setIntVariable("Audio::numCulledStreamingStreams",    mNumCulledStreamingStreams);
}
#endif

//--------------------------------------------------------------------------
// Audio Update...
//--------------------------------------------------------------------------
void alxLoopingUpdate()
{
   static LoopingList culledList;

   U32 updateTime = Platform::getRealMilliseconds();

   // check if can wakeup the inactive loopers
   if(mLoopingCulledList.size())
   {
      Point3F listener;
      alxGetListenerPoint3F(AL_POSITION, &listener);

      // get the 'sort' value for this sound (could be based on time played...),
      // and add to the culled list
      LoopingList::iterator itr;
      culledList.clear();

      for(itr = mLoopingCulledList.begin(); itr != mLoopingCulledList.end(); itr++)
      {
         if((*itr)->mScore <= MIN_UNCULL_GAIN)
            continue;

         if((updateTime - (*itr)->mCullTime) < MIN_UNCULL_PERIOD)
            continue;

         culledList.push_back(*itr);
      }

      if(!culledList.size())
         return;

      U32 index = MAX_AUDIOSOURCES;

      if(culledList.size() > 1)
         culledList.sort();

      for(itr = culledList.begin(); itr != culledList.end(); itr++)
      {
         if(!findFreeSource(&index))
         {
            // score does not include master volume
            if(!cullSource(&index, (*itr)->mScore))
               break;

             // check buffer
            if(!bool((*itr)->mBuffer))
            {
               // remove from culled list
               LoopingList::iterator tmp;
               tmp = mLoopingCulledList.findImage((*itr)->mHandle);
               AssertFatal(tmp, "alxLoopingUpdate: failed to find culled source");
               mLoopingCulledList.erase_fast(tmp);

               // remove from looping list (and free)
               tmp = mLoopingList.findImage((*itr)->mHandle);
               if(tmp)
               {
                  (*tmp)->clear();
                  mLoopingFreeList.push_back(*tmp);
                  mLoopingList.erase_fast(tmp);
               }

               continue;
            }
         }

         // remove from culled list
         LoopingList::iterator tmp = mLoopingCulledList.findImage((*itr)->mHandle);
         AssertFatal(tmp, "alxLoopingUpdate: failed to find culled source");
         mLoopingCulledList.erase_fast(tmp);

         // restore all state data
         mHandle[index] = (*itr)->mHandle;
         mBuffer[index] = (*itr)->mBuffer;
         mScore[index] = (*itr)->mScore;
         mSourceVolume[index] = (*itr)->mDescription.mVolume;
         mType[index] = (*itr)->mDescription.mType;
         mSampleEnvironment[index] = (*itr)->mEnvironment;

         ALuint source = mSource[index];

         // setup play info
         alGetError();

         alxSourcePlay(source, *itr);
         if(mEnvironmentEnabled)
            alxSourceEnvironment(source, *itr);

         alxPlay(mHandle[index]);
      }
   }
}

void alxStreamingUpdate()
{
   // update buffer queues on active streamers
   // update the loopers
   for(StreamingList::iterator itr = mStreamingList.begin(); itr != mStreamingList.end(); itr++)
   {
      if((*itr)->mHandle & AUDIOHANDLE_INACTIVE_BIT)
         continue;

	  (*itr)->updateBuffers();
   }

   static StreamingList culledList;

   U32 updateTime = Platform::getRealMilliseconds();

   // check if can wakeup the inactive loopers
   if(mStreamingCulledList.size())
   {
      Point3F listener;
      alxGetListenerPoint3F(AL_POSITION, &listener);

      // get the 'sort' value for this sound (could be based on time played...),
      // and add to the culled list
      StreamingList::iterator itr;
      culledList.clear();

      for(itr = mStreamingCulledList.begin(); itr != mStreamingCulledList.end(); itr++)
      {
         if((*itr)->mScore <= MIN_UNCULL_GAIN)
            continue;

         if((updateTime - (*itr)->mCullTime) < MIN_UNCULL_PERIOD)
            continue;
         culledList.push_back(*itr);
      }

      if(!culledList.size())
         return;

      U32 index = MAX_AUDIOSOURCES;

      if(culledList.size() > 1)
         culledList.sort();

      for(itr = culledList.begin(); itr != culledList.end(); itr++)
      {
         if(!findFreeSource(&index))
         {
            // score does not include master volume
            if(!cullSource(&index, (*itr)->mScore))
               break;

             // check buffer
            //if(!bool((*itr)->mBuffer))
            //{
               // remove from culled list
               StreamingList::iterator tmp;
               tmp = mStreamingCulledList.findImage((*itr)->mHandle);
               AssertFatal(tmp, "alxStreamingUpdate: failed to find culled source");
               mStreamingCulledList.erase_fast(tmp);

               // remove from streaming list (and free)
               tmp = mStreamingList.findImage((*itr)->mHandle);
               if(tmp)
               {
                  delete(*tmp);
                  mStreamingList.erase_fast(tmp);
               }

               continue;
            //}
         }

         // remove from culled list
         StreamingList::iterator tmp = mStreamingCulledList.findImage((*itr)->mHandle);
         AssertFatal(tmp, "alxStreamingUpdate: failed to find culled source");
         mStreamingCulledList.erase_fast(tmp);
         alxSourcePlay(*itr);
         // restore all state data
         mHandle[index] = (*itr)->mHandle;
         mScore[index] = (*itr)->mScore;
         mSourceVolume[index] = (*itr)->mDescription.mVolume;
         mType[index] = (*itr)->mDescription.mType;
         mSampleEnvironment[index] = (*itr)->mEnvironment;

         ALuint source = mSource[index];
		 (*itr)->mSource = mSource[index];

         // setup play info
         alGetError();

         if(mEnvironmentEnabled)
            alxSourceEnvironment(source, *itr);

         alxPlay(mHandle[index]);
      }
   }
}

//--------------------------------------------------------------------------
void alxCloseHandles()
{
   for(U32 i = 0; i < mNumSources; i++)
   {
      if(mHandle[i] & AUDIOHANDLE_LOADING_BIT)
         continue;

      if(mHandle[i] == NULL_AUDIOHANDLE)
         continue;

      ALint state = 0;
      alGetSourcei(mSource[i], AL_SOURCE_STATE, &state);
      if(state == AL_PLAYING)
         continue;

      if(!(mHandle[i] & AUDIOHANDLE_INACTIVE_BIT))
      {
         // should be playing? must have encounted an error.. remove
         LoopingList::iterator itr = mLoopingList.findImage(mHandle[i]);
         if(itr && !((*itr)->mHandle & AUDIOHANDLE_INACTIVE_BIT))
         {
            AssertFatal(!mLoopingInactiveList.findImage((*itr)->mHandle), "alxCloseHandles: image incorrectly in inactive list");
            AssertFatal(!mLoopingCulledList.findImage((*itr)->mHandle), "alxCloseHandles: image already in culled list");
            mLoopingCulledList.push_back(*itr);
            (*itr)->mHandle |= AUDIOHANDLE_INACTIVE_BIT;

            mHandle[i] = NULL_AUDIOHANDLE;
            mBuffer[i] = 0;
         }

         // should be playing? must have encounted an error.. remove
//         StreamingList::iterator itr2 = mStreamingList.findImage(mHandle[i]);
//         if(itr2 && !((*itr2)->mHandle & AUDIOHANDLE_INACTIVE_BIT))
//         {
//            AssertFatal(!mStreamingInactiveList.findImage((*itr2)->mHandle), "alxCloseHandles: image incorrectly in inactive list");
//            AssertFatal(!mStreamingCulledList.findImage((*itr2)->mHandle), "alxCloseHandles: image already in culled list");
//			(*itr2)->freeStream();
//
//            mStreamingCulledList.push_back(*itr2);
//            (*itr2)->mHandle |= AUDIOHANDLE_INACTIVE_BIT;
//
//            mHandle[i] = NULL_AUDIOHANDLE;
//            mBuffer[i] = 0;
//         }
      }

      mHandle[i] = NULL_AUDIOHANDLE;
      mBuffer[i] = 0;
   }
}

//----------------------------------------------------------------------------------
// - update the score for each audio source.  this is used for culing sources.
//   normal ranges are between 0.f->1.f, voice/loading/music streams are scored
//   outside this range so that they will not be culled
// - does not scale by attenuated volumes
void alxUpdateScores(bool sourcesOnly)
{
   Point3F listener;
   alGetListener3f(AL_POSITION, &listener.x, &listener.y, &listener.z);

   // do the base sources
   for(U32 i = 0; i < mNumSources; i++)
   {
      if(mHandle[i] == NULL_AUDIOHANDLE)
      {
         mScore[i] = 0.f;
         continue;
      }

      // grab the volume.. (not attenuated by master for score)
      F32 volume = mSourceVolume[i] * mAudioTypeVolume[mType[i]];

      // 3d?
      mScore[i] = volume;
#ifdef REL_WORKAROUND
      ALint val = AL_FALSE;
      alGetSourcei(mSource[i], AL_SOURCE_ABSOLUTE, &val);
      if(val == AL_TRUE)
#else
      ALint val = AL_FALSE;
      alGetSourcei(mSource[i], AL_SOURCE_RELATIVE, &val);
      if(val == AL_FALSE)
#endif
      {
         // approximate 3d volume
         Point3F pos;
         alGetSourcefv(mSource[i], AL_POSITION, (ALfloat*)((F32*)pos) );

         ALfloat min=0, max=1;
         alGetSourcef(mSource[i], AL_REFERENCE_DISTANCE, &min);
         alGetSourcef(mSource[i], AL_MAX_DISTANCE, &max);

         pos -= listener;
         F32 dist = pos.magnitudeSafe();

         if(dist >= max)
            mScore[i] = 0.f;
         else if(dist > min)
            mScore[i] *= (max-dist) / (max-min);
      }
   }

   if(sourcesOnly)
      return;

   U32 updateTime = Platform::getRealMilliseconds();

   // update the loopers
   for(LoopingList::iterator itr = mLoopingList.begin(); itr != mLoopingList.end(); itr++)
   {
      if(!((*itr)->mHandle & AUDIOHANDLE_INACTIVE_BIT))
         continue;

      if((updateTime - (*itr)->mCullTime) < MIN_UNCULL_PERIOD)
         continue;

      (*itr)->mScore = (*itr)->mDescription.mVolume;
      if((*itr)->mDescription.mIs3D)
      {
         Point3F pos = (*itr)->mPosition - listener;
         F32 dist = pos.magnitudeSafe();

         F32 min = (*itr)->mDescription.mReferenceDistance;
         F32 max = (*itr)->mDescription.mMaxDistance;

         if(dist >= max)
            (*itr)->mScore = 0.f;
         else if(dist > min)
            (*itr)->mScore *= (max-dist) / (max-min);
      }

      // attenuate by the channel gain
      (*itr)->mScore *= mAudioTypeVolume[(*itr)->mDescription.mType];
   }

   // update the streamers
   for(StreamingList::iterator itr = mStreamingList.begin(); itr != mStreamingList.end(); itr++)
   {
      if(!((*itr)->mHandle & AUDIOHANDLE_INACTIVE_BIT))
         continue;

      if((updateTime - (*itr)->mCullTime) < MIN_UNCULL_PERIOD)
         continue;

      (*itr)->mScore = (*itr)->mDescription.mVolume;
      if((*itr)->mDescription.mIs3D)
      {
         Point3F pos = (*itr)->mPosition - listener;
         F32 dist = pos.magnitudeSafe();

         F32 min = (*itr)->mDescription.mReferenceDistance;
         F32 max = (*itr)->mDescription.mMaxDistance;

         if(dist >= max)
            (*itr)->mScore = 0.f;
         else if(dist > min)
            (*itr)->mScore *= (max-dist) / (max-min);
      }

      // attenuate by the channel gain
      (*itr)->mScore *= mAudioTypeVolume[(*itr)->mDescription.mType];
   }
}

// the directx buffers are set to mute at max distance, but many of the providers seem to
// ignore this flag... that is why this is here
void alxUpdateMaxDistance()
{
   Point3F listener;
   alGetListener3f(AL_POSITION, &listener.x, &listener.y, &listener.z);

   for(U32 i = 0; i < mNumSources; i++)
   {
      if(mHandle[i] == NULL_AUDIOHANDLE)
         continue;

#ifdef REL_WORKAROUND
      ALint val = AL_FALSE;
      alGetSourcei(mSource[i], AL_SOURCE_ABSOLUTE, &val);
      if(val == AL_FALSE)
#else
      ALint val = AL_FALSE;
      alGetSourcei(mSource[i], AL_SOURCE_RELATIVE, &val);
      if(val == AL_TRUE)
#endif
         continue;

      Point3F pos;
      alGetSourcefv(mSource[i], AL_POSITION, (F32*)pos);

      F32 dist = 0.f;
      alGetSourcef(mSource[i], AL_MAX_DISTANCE, &dist);

      pos -= listener;
      dist -= pos.len();

      F32 gain = (dist < 0.f) ? 0.f : mSourceVolume[i] * mAudioTypeVolume[mType[i]] * mMasterVolume;
      alSourcef(mSource[i], AL_GAIN, Audio::linearToDB(gain));

   }
}

//--------------------------------------------------------------------------
// Called to update alx system
//--------------------------------------------------------------------------
void alxUpdate()
{
   //if(mForceMaxDistanceUpdate)
      alxUpdateMaxDistance();

   alxCloseHandles();
   alxUpdateScores(false);
   alxLoopingUpdate();
   alxStreamingUpdate();

#ifdef TORQUE_GATHER_METRICS
   alxGatherMetrics();
#endif
}

//--------------------------------------------------------------------------
// Misc
//--------------------------------------------------------------------------
// client-side function only
ALuint alxGetWaveLen(ALuint buffer)
{
   if(buffer == AL_INVALID)
      return(0);

   ALint frequency = 0;
   ALint bits = 0;
   ALint channels = 0;
   ALint size;

   alGetBufferi(buffer, AL_FREQUENCY, &frequency);
   alGetBufferi(buffer, AL_BITS, &bits);
   alGetBufferi(buffer, AL_CHANNELS, &channels);
   alGetBufferi(buffer, AL_SIZE, &size);

   if(!frequency || !bits || !channels)
   {
      Con::errorf(ConsoleLogEntry::General, "alxGetWaveLen: invalid buffer");
      return(0);
   }

   F64 len = (F64(size) * 8000.f) / F64(frequency * bits * channels);
   return(len);
}


//--------------------------------------------------------------------------
// Environment:
//--------------------------------------------------------------------------
void alxEnvironmenti(ALenum pname, ALint value)
{
/* todo
   alEnvironmentiIASIG(mEnvironment, pname, value);
*/
}

void alxEnvironmentf(ALenum pname, ALfloat value)
{
/* todo
   alEnvironmentfIASIG(mEnvironment, pname, value);
*/
}

void alxGetEnvironmenti(ALenum pname, ALint * value)
{
/* todo
   alGetEnvironmentiIASIG_EXT(mEnvironment, pname, value);
*/
}

void alxGetEnvironmentf(ALenum pname, ALfloat * value)
{
/* todo
   alGetEnvironmentfIASIG_EXT(mEnvironment, pname, value);
*/
}

void alxEnableEnvironmental(bool enable)
{
   if(mEnvironmentEnabled == enable)
      return;

   // go through the playing sources and update their reverb mix
   // - only 3d samples get environmental fx
   // - only loopers can reenable fx
   for(U32 i = 0; i < mNumSources; i++)
   {
      if(mHandle[i] == NULL_AUDIOHANDLE)
         continue;

      ALint val = AL_FALSE;

      // 3d?
#ifdef REL_WORKAROUND
      alGetSourcei(mSource[i], AL_SOURCE_ABSOLUTE, &val);
      if(val == AL_FALSE)
#else
      alGetSourcei(mSource[i], AL_SOURCE_RELATIVE, &val);
      if(val == AL_TRUE)
#endif
         continue;

      // stopped?
      val = AL_STOPPED;
      alGetSourcei(mSource[i], AL_SOURCE_STATE, &val);

      // only looping sources can reenable environmental effects (no description around
      // for the non-loopers)
      if(enable)
      {
         LoopingList::iterator itr = mLoopingList.findImage(mHandle[i]);
         if(!itr)
            continue;

/* todo
         alSourcef(mSource[i], AL_ENV_SAMPLE_REVERB_MIX_EXT, (*itr)->mDescription.mEnvironmentLevel);
*/
      }
/* todo
      else
         alSourcef(mSource[i], AL_ENV_SAMPLE_REVERB_MIX_EXT, 0.f);
*/
   }

   mEnvironmentEnabled = enable;
}

void alxSetEnvironment(const AudioEnvironment * env)
{
/* todo
   mCurrentEnvironment = const_cast<AudioEnvironment*>(env);

   // reset environmental audio?
   if(!env)
   {
      alxEnvironmenti(AL_ENV_ROOM_IASIG, AL_ENVIRONMENT_GENERIC);
      return;
   }

   // room trashes all the values
   if(env->mUseRoom)
   {
      alxEnvironmenti(AL_ENV_ROOM_IASIG,                       env->mRoom);
      return;
   }

   // set all the params
   alxEnvironmenti(AL_ENV_ROOM_HIGH_FREQUENCY_IASIG,           env->mRoomHF);
   alxEnvironmenti(AL_ENV_REFLECTIONS_IASIG,                   env->mReflections);
   alxEnvironmenti(AL_ENV_REVERB_IASIG,                        env->mReverb);

   alxEnvironmentf(AL_ENV_ROOM_ROLLOFF_FACTOR_IASIG,           env->mRoomRolloffFactor);
   alxEnvironmentf(AL_ENV_DECAY_TIME_IASIG,                    env->mDecayTime);
   alxEnvironmentf(AL_ENV_DECAY_HIGH_FREQUENCY_RATIO_IASIG,    env->mDecayTime);
   alxEnvironmentf(AL_ENV_REFLECTIONS_DELAY_IASIG,             env->mReflectionsDelay);
   alxEnvironmentf(AL_ENV_REVERB_DELAY_IASIG,                  env->mReverbDelay);
   alxEnvironmentf(AL_ENV_DENSITY_IASIG,                       env->mAirAbsorption);
   alxEnvironmentf(AL_ENV_DIFFUSION_IASIG,                     env->mEnvironmentDiffusion);

   // ext:
   alxEnvironmenti(AL_ENV_ROOM_VOLUME_EXT,                     env->mRoomVolume);
   alxEnvironmenti(AL_ENV_FLAGS_EXT,                           env->mFlags);

   alxEnvironmentf(AL_ENV_EFFECT_VOLUME_EXT,                   env->mEffectVolume);
   alxEnvironmentf(AL_ENV_DAMPING_EXT,                         env->mDamping);
   alxEnvironmentf(AL_ENV_ENVIRONMENT_SIZE_EXT,                env->mEnvironmentSize);
*/
}

const AudioEnvironment * alxGetEnvironment()
{
   return(mCurrentEnvironment);
}

F32 alxGetStreamPosition( AUDIOHANDLE handle )
{
   StreamingList::iterator itr = mStreamingList.findImage(handle);
   if( !itr )
      return -1.f;

   return (*itr)->getElapsedTime();
}

F32 alxGetStreamDuration( AUDIOHANDLE handle )
{
   StreamingList::iterator itr = mStreamingList.findImage(handle);
   if( !itr )
      return -1.f;

   return (*itr)->getTotalTime();
}

// Namespace: Audio ---------------------------------------------------------
namespace Audio
{

//---------------------------------------------------------------------------
// the following db<->linear conversion functions come from Loki openAL linux driver
// code, here more for completeness than anything else (all current audio code
// uses AL_GAIN_LINEAR)... in Audio:: so that looping updates and audio channel updates
// can convert gain types and to give the miles driver access
static const F32 logtab[] = {
   0.00,    0.001,   0.002,   0.003,   0.004,
   0.005,   0.01,    0.011,   0.012,   0.013,
   0.014,   0.015,   0.016,   0.02,    0.021,
   0.022,   0.023,   0.024,   0.025,   0.03,
   0.031,   0.032,   0.033,   0.034,   0.04,
   0.041,   0.042,   0.043,   0.044,   0.05,
   0.051,   0.052,   0.053,   0.054,   0.06,
   0.061,   0.062,   0.063,   0.064,   0.07,
   0.071,   0.072,   0.073,   0.08,    0.081,
   0.082,   0.083,   0.084,   0.09,    0.091,
   0.092,   0.093,   0.094,   0.10,    0.101,
   0.102,   0.103,   0.11,    0.111,   0.112,
   0.113,   0.12,    0.121,   0.122,   0.123,
   0.124,   0.13,    0.131,   0.132,   0.14,
   0.141,   0.142,   0.143,   0.15,    0.151,
   0.152,   0.16,    0.161,   0.162,   0.17,
   0.171,   0.172,   0.18,    0.181,   0.19,
   0.191,   0.192,   0.20,    0.201,   0.21,
   0.211,   0.22,    0.221,   0.23,    0.231,
   0.24,    0.25,    0.251,   0.26,    0.27,
   0.271,   0.28,    0.29,    0.30,    0.301,
   0.31,    0.32,    0.33,    0.34,    0.35,
   0.36,    0.37,    0.38,    0.39,    0.40,
   0.41,    0.43,    0.50,    0.60,    0.65,
   0.70,    0.75,    0.80,    0.85,    0.90,
   0.95,    0.97,    0.99 };
const int logmax = sizeof logtab / sizeof *logtab;

F32 DBToLinear(F32 value)
{
   if(value <= 0.f)
      return(0.f);
   if(value >= 1.f)
      return(1.f);

   S32 max = logmax;
   S32 min = 0;
   S32 mid;
   S32 last = -1;

   mid = (max - min) / 2;
   do {
      last = mid;

      if(logtab[mid] == value)
         break;

      if(logtab[mid] < value)
         min = mid;
      else
         max = mid;

      mid = min + ((max - min) / 2);
   } while(last != mid);

   return((F32)mid / logmax);
}

F32 linearToDB(F32 value)
{
   if(value <= 0.f)
      return(0.f);
   if(value >= 1.f)
      return(1.f);
   return(logtab[(U32)(logmax * value)]);
}
//---------------------------------------------------------------------------

static ALvoid errorCallback(ALbyte *msg)
{
   // used to allow our OpenAL implementation to display info on the console
   Con::errorf(ConsoleLogEntry::General, (const char *)msg);
}

//--------------------------------------------------------------------------
bool prepareContext()
{
//   mForceMaxDistanceUpdate = Con::getBoolVariable("$pref::audio::forceMaxDistanceUpdate", false);
   mForceMaxDistanceUpdate = false;

   // allocate source channels: can only get 16 sources on NT, so check the max before
   // jff: todo, allow for more than 16 channels on non-NT boxes
   mNumSources = mRequestSources;
   alGenSources(mRequestSources, mSource);

   // invalidate all existing handles
   dMemset(mHandle, NULL_AUDIOHANDLE, sizeof(mHandle));

   // pre-load profile data
   SimGroup* grp = Sim::getDataBlockGroup();
   if (grp != NULL)
   {
      SimObjectList::iterator itr = grp->begin();
      for (; itr != grp->end(); itr++)
      {
         AudioProfile *profile = dynamic_cast<AudioProfile*>(*itr);
         if(profile != NULL && profile->isPreload())
         {
            Resource<AudioBuffer> buffer = AudioBuffer::find(profile->mFilename);
            if((bool)buffer)
               ALuint bufferId = buffer->getALBuffer();
         }
      }
   }
   return(true);
}

void shutdownContext()
{
   // invalidate active handles
   dMemset(mSource, 0, sizeof(mSource));
}


//--------------------------------------------------------------------------
bool OpenALInit()
{
   OpenALShutdown();

   if(!OpenALDLLInit())
      return false;

   // Open a device
#ifdef TORQUE_OS_LINUX
   const char* deviceSpecifier =
     Con::getVariable("Pref::Unix::OpenALSpecifier");
   if (dStrlen(deviceSpecifier) == 0)
     // use SDL for audio output by default
     deviceSpecifier = "'((devices '(sdl)))";
   mDevice = (ALCdevice *)alcOpenDevice((ALubyte*)deviceSpecifier);
#else
   mDevice = (ALCdevice *)alcOpenDevice((ALubyte*)NULL);
#endif
   if (mDevice == (ALCdevice *)NULL)
      return false;

   // Create an openAL context
#ifdef TORQUE_OS_LINUX
   int freq = Con::getIntVariable("Pref::Unix::OpenALFrequency");
   if (freq == 0)
      freq = 22050;

   Con::printf("   Setting OpenAL output frequency to %d", freq);
   // some versions of openal have bugs converting between 22050 and 44100
   // samples when the lib is in 44100 mode.
   int attrlist[] = {
      // this 0x100 is "ALC_FREQUENCY" in the linux openal implementation.
      // it doesn't match the value of the creative headers, so we can't use
      // that define.  seems like linux torque really shouldn't be using the
      // creative headers.
      0x100, freq,
      0
   };
   mContext = alcCreateContext(mDevice,attrlist);
#else
   mContext = alcCreateContext(mDevice,NULL);
#endif
   if (mContext == NULL)
      return false;

   // Make this context the active context
   alcMakeContextCurrent(mContext);

   ALenum err = alGetError();
   mRequestSources = MAX_AUDIOSOURCES;
   while (true)
   {
      alGenSources(mRequestSources, mSource);
      err = alGetError();
      if (err == AL_NO_ERROR)
         break;
   
      mRequestSources--;

      if (mRequestSources == 0)
      {
         OpenALShutdown();
         return (false);
      }
   }
   mNumSources = mRequestSources;

   // invalidate all existing handles
   dMemset(mHandle, NULL_AUDIOHANDLE, sizeof(mHandle));

   // default all channels to full gain
   for(U32 i = 0; i < Audio::NumAudioTypes; i++)
      mAudioTypeVolume[i] = 1.f;

   // Clear Error Code
   alGetError();

   // Similiar to DSound Model w/o min distance clamping
   alEnable(AL_DISTANCE_MODEL);
   alDistanceModel(AL_INVERSE_DISTANCE);
   alListenerf(AL_GAIN_LINEAR, 1.f);

   return true;
}

//--------------------------------------------------------------------------
void OpenALShutdown()
{
   alxStopAll();

   //if(mInitialized)
   {
      alxEnvironmentDestroy();
   }

   while(mLoopingList.size())
   {
      mLoopingList.last()->mBuffer.purge();
      delete mLoopingList.last();
      mLoopingList.pop_back();
   }

   while(mLoopingFreeList.size())
   {
      mLoopingFreeList.last()->mBuffer.purge();
      delete mLoopingFreeList.last();
      mLoopingFreeList.pop_back();
   }

   for(U32 i = 0; i < MAX_AUDIOSOURCES; i++)
      mBuffer[i] = 0;

   alDeleteSources(mNumSources, mSource);

   if (mContext)
   {
      alcDestroyContext(mContext);
      mContext = NULL;
   }
   if (mDevice)
   {
      alcCloseDevice(mDevice);
      mDevice = NULL;
   }

   OpenALDLLShutdown();
}


} // end OpenAL namespace

AudioStreamSource* alxFindAudioStreamSource(AUDIOHANDLE handle)
{
	StreamingList::iterator itr2 = mStreamingList.findImage(handle);
	if(itr2)
		return *itr2;
	return NULL;
}
