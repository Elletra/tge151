//-----------------------------/------------------------------------------------
// Torque Game Engine 
// Copyright (C) GarageGames.com, Inc.
//-----------------------------------------------------------------------------

#ifdef _MSC_VER
#pragma warning(disable : 4786)
#endif

#include "mayaAppMesh.h"
#include "mayaAppNode.h"
#include "appConfig.h"
#include "appIfl.h"
#include <maya/MFnDependencyNode.h>
#include <maya/MFnDagNode.h>
#include <maya/MGlobal.h>
#include <maya/MTime.h>
#include <maya/MDistance.h>
#include <maya/MMatrix.h>
#include <maya/MPlug.h>
#include <maya/MDGContext.h>
#include <maya/MAnimUtil.h>
#include <maya/MFnMesh.h>
#include <maya/MFnSet.h>
#include <maya/MItMeshPolygon.h>
#include <maya/MItMeshVertex.h>
#include <maya/MItDependencyGraph.h>
#include <maya/MItGeometry.h>
#include <maya/MFnSkinCluster.h>
#include <maya/MDagPathArray.h>
#include <maya/MFnReflectShader.h>

namespace DTS
{

   bool getMeshAndSkin(MDagPath & path, MFnSkinCluster & skinCluster)
   {
      MStatus status;

      // The following is the "correct" way to get the
      // mesh and skinCluster in case the mesh is a skin,
      // according to (and adapted from):
      //    http://www.greggman.com/pages/mayastuff.htm
      // But we don't get the MeshFn from the input plug of the 
      // skinCluster as he does.  Two reasons for this:
      // 1) couldn't get it to work properly, not a valid MeshFn.
      // 2) we want the final mesh, not the intermediate product.

      // the deformed mesh comes into the visible mesh
      // through its "inmesh" plug
      MPlug inMeshPlug = MFnDagNode(path).findPlug("inMesh", &status);
      if (status != MS::kSuccess || !inMeshPlug.isConnected())
         return false; // no skin found

      // walk the tree of stuff upstream from this plug
      MItDependencyGraph dgIt(inMeshPlug,MFn::kInvalid,
                              MItDependencyGraph::kUpstream,
                              MItDependencyGraph::kDepthFirst,
                              MItDependencyGraph::kPlugLevel,
                              &status);

      if (MS::kSuccess == status)
      {
         S32 count = 0;
         dgIt.disablePruningOnFilter();

         for ( ; ! dgIt.isDone(); dgIt.next() )
         {
            MObject thisNode = dgIt.thisNode();

            // go until we find a skinCluster

            if (thisNode.apiType() == MFn::kSkinClusterFilter)
            {
               skinCluster.setObject(thisNode);
               return true; // found a skin
            }
         }
      }
      return false; // no skin found
   }

   MayaAppMesh::MayaAppMesh(MDagPath & nodePath, MDagPath & path)
   {
      mPath = path;
      mNodePath = nodePath;
      mName = NULL;
   }

   MayaAppMesh::~MayaAppMesh()
   {
   }

   void MayaAppMesh::getSkinData()
   {
      MFnSkinCluster skinCluster;
      if (!getMeshAndSkin(mPath,skinCluster))
         return; // no skin, no sweat

      MStatus status;
      S32 i;

      // get bones/influence array
		MDagPathArray bones;
		S32 numBones = skinCluster.influenceObjects(bones, &status);
      if (!CheckMayaStatus(status))
         return;

      S32 bone;
      for (bone=0; bone<numBones; bone++)
      {
         AppConfig::PrintDump(PDObjectStateDetails,avar("Adding bone %s\r\n",bones[bone].partialPathName().asChar()));
         mBones.push_back(new MayaAppNode(bones[bone]));
         mWeights.push_back(new std::vector<F32>);
      }

      MItGeometry gitr(mPath,&status);
      if (!CheckMayaStatus(status))
         return;

      AppConfig::PrintDump(PDObjectStateDetails,avar("SkinPath: %s, # verts: %i, # bones: %i",
         mPath.partialPathName().asChar(),gitr.count(),numBones));

      for (; !gitr.isDone(); gitr.next())
      {
         while ( numBones && mWeights[0]->size() <= U32(gitr.index()))
            for (i=0; i<numBones; i++)
               mWeights[i]->push_back(0.0f);

         MObject comp = gitr.component(&status);
         if (!CheckMayaStatus(status))
            return;

         MFloatArray wts;
         U32 weightCount;
         status = skinCluster.getWeights(mPath,comp,wts,weightCount);
         if (!CheckMayaStatus(status))
            return;
         if (weightCount != numBones)
         {
            AppConfig::SetExportError("maya3", "Assertion failed on skinned mesh");
            return;
         }
         for (i=0; i<numBones; i++)
            (*mWeights[i])[gitr.index()] = wts[i];
      }

      // make sure we have weights for all bones and verts
      S32 numVerts = MFnMesh(mPath,&status).numVertices();
      if (!CheckMayaStatus(status))
         return;
      while ( numBones && mWeights[0]->size() < U32(numVerts))
         for (i=0; i<numBones; i++)
            mWeights[i]->push_back(0.0f);
   }

   const char * MayaAppMesh::getName()
   {
      MStatus status;
      if (!mName)
      {
         MFnDagNode fnNode(mNodePath,&status);
         CheckMayaStatus(status);
         const char * newname = fnNode.name(&status).asChar();
         CheckMayaStatus(status);
         if (newname)
         {
            mName = strnew(newname);
            HandleMayaNegative(mName);
         }
      }
      CheckMayaStatus(status);
      return mName;
   }

   Matrix<4,4,F32> MayaAppMesh::getMeshTransform(const AppTime & time)
   {
      MStatus status;
      MayaSetTime(time);
      MMatrix mayaMat = mPath.inclusiveMatrix(&status);
      CheckMayaStatus(status);
      return MayaToDtsTransform(mayaMat);
   }

   F32 MayaAppMesh::getVisValue(const AppTime & time)
   {
      // Need to check for "visibility" plug, which is built into
      // maya and is a bool, and "vis" plug, which is custom and
      // assumed to be a float.  We added the "vis" plug to allow
      // non-bool vis values.  The "vis" plug takes precedence.
      MayaSetTime(time);

      F32 val = 1.0f;
      if (getFloat("vis",val))
         return val;
      bool ret = true;
      getBool("visibility",ret);
      return ret ? 1.0f : 0.0f;
   }

   bool MayaAppMesh::getFloat(const char * propName, F32 & defaultVal)
   {
      MStatus status;

      MFnDagNode fnNode(mNodePath,&status);
      CheckMayaStatus(status);

      MPlug plug = fnNode.findPlug(MString(propName),&status);
      if (status!=MStatus::kSuccess)
         return false;

      F32 val;
      status = plug.getValue(val);
      if (status == MStatus::kSuccess)
         defaultVal = val;
      return status==MStatus::kSuccess;
   }

   bool MayaAppMesh::getInt(const char * propName, S32 & defaultVal)
   {
      MStatus status;

      MFnDagNode fnNode(mNodePath,&status);
      CheckMayaStatus(status);

      MPlug plug = fnNode.findPlug(MString(propName),&status);
      if (status!=MStatus::kSuccess)
         return false;

      S32 val;
      status = plug.getValue(val);
      if (status == MStatus::kSuccess)
         defaultVal = val;
      return status==MStatus::kSuccess;
   }

   bool MayaAppMesh::getBool(const char * propName, bool & defaultVal)
   {
      MStatus status;

      MFnDagNode fnNode(mNodePath,&status);
      CheckMayaStatus(status);

      MPlug plug = fnNode.findPlug(MString(propName),&status);
      if (status!=MStatus::kSuccess)
         return false;

      bool val;
      status = plug.getValue(val);
      if (status == MStatus::kSuccess)
         defaultVal = val;
      return status==MStatus::kSuccess;
   }

   bool MayaAppMesh::getMaterial(S32 matIdx, Material & mat)
   {
      if (matIdx >= 0 && U32(matIdx) < mMaterials.size())
      {
         mat = mMaterials[matIdx];
         return true;
      }
      return false;
   }

   bool MayaAppMesh::animatesVis(const AppSequenceData & seqData)
   {
      MStatus status;

      // Need to check to see if either "visibility" or "vis" is animated.
      // "visibility" is the built-in visibility mechanism, but it is
      // only a bool.  So we also look for custom "vis" which is assumed
      // to be a float.

      MFnDagNode fnNode(mNodePath,&status);
      CheckMayaStatus(status);

      MPlug plug = fnNode.findPlug(MString("visibility"),&status);
      CheckMayaStatus(status);
      if (status!=MStatus::kSuccess)
         return false;

      if (MAnimUtil::isAnimated(plug,false,&status))
         return Parent::animatesVis(seqData);
      CheckMayaStatus(status);

      // don't animate "visibility", what about "vis"
      MPlug plug2 = fnNode.findPlug(MString("vis"),&status);
      if (status!=MStatus::kSuccess)
         return false;
      if (MAnimUtil::isAnimated(plug2,false,&status))
         return Parent::animatesVis(seqData);

      return false;
   }

   AppMeshLock MayaAppMesh::lockMesh(const AppTime & time, const Matrix<4,4,F32> & objectOffset)
   {
      S32 i;
      MayaSetTime(time);

      MStatus status;
      MObject component = MObject::kNullObj; // not really sure what this is for...

      MFnMesh fnMesh(mPath,&status);
      if (!CheckMayaStatus(status))
         return Parent::lockMesh(time,objectOffset);

      MItMeshPolygon polyIter(mPath,component,&status);
      if (!CheckMayaStatus(status))
         return Parent::lockMesh(time,objectOffset);
      S32 polyVertexCount = polyIter.polygonVertexCount();
      if (polyVertexCount != 3)
      {
         AppConfig::SetExportError("maya5", "Non-triangle mesh not supported");
         return Parent::lockMesh(time,objectOffset);
      }

      MItMeshVertex vtxIter(mPath,component,&status);
      if (!CheckMayaStatus(status))
         return Parent::lockMesh(time,objectOffset);

      // Create transform based on objectOffset for transforming normals
      Matrix<4,4,F32> normOffset = objectOffset;
      normOffset[0][3] = 0.0f;
      normOffset[1][3] = 0.0f;
      normOffset[2][3] = 0.0f;
      normOffset[3][0] = 0.0f;
      normOffset[3][1] = 0.0f;
      normOffset[3][2] = 0.0f;
      normOffset[3][3] = 1.0f;
      F32 normScale = 0.0f;
      for (i=0; i<9; i++)
      {
         S32 j = i % 3;
         normScale += normOffset[(i-j)/3][j];
      }
      normScale /= 3.0f;
      normScale = 1.0f/normScale;

      // Write out the vertex table
      //

      std::vector<Point3D> verts;
      std::vector<Point2D> tverts;
      for ( ; !vtxIter.isDone(); vtxIter.next() )
      {
         MPoint p = vtxIter.position( MSpace::kObject );
         // convert from maya point to dts point...
         // ...also, convert from internal units 
         // to the current ui units
         Point3D pos;
         MDistance dist; // starts as internal units

         dist.setValue(p.x);
         pos[0] = F32(dist.asMeters());
         dist.setValue(p.y);
         pos[1] = F32(dist.asMeters());
         dist.setValue(p.z);
         pos[2] = F32(dist.asMeters());
         verts.push_back(objectOffset * pos);
      }

      MFloatArray uArray, vArray;
      fnMesh.getUVs( uArray, vArray );
      S32 uvLength = uArray.length();
      for (i=0; i<uvLength; i++ )
      {
         Point2D tvert(uArray[i],1.0f-vArray[i]);
         tverts.push_back(tvert);
      }

      for ( ; !polyIter.isDone(); polyIter.next() )
      {
         Primitive face;
         face.firstElement = mIndices.size();
         face.numElements = 3;
         face.type = Primitive::Triangles|Primitive::Indexed|Primitive::NoMaterial;

         for (S32 vtx=polyVertexCount-1; vtx>=0; vtx-- )
         {
            U16 vertId = polyIter.vertexIndex(vtx);
            Point3D vert = verts[vertId];

            // compute normal (TODO: handle non-uniform scale properly)
            MVector mayaNorm;
            status = polyIter.getNormal(vtx,mayaNorm,MSpace::kObject);
            if (!CheckMayaStatus(status))
               return Parent::lockMesh(time,objectOffset);
            F32 norm0, norm1, norm2;
            norm0 = F32(mayaNorm[0]);
            norm1 = F32(mayaNorm[1]);
            norm2 = F32(mayaNorm[2]);
            Point3D norm(norm0, norm1, norm2);
            norm = normOffset * norm;
            norm *= normScale;
            if (norm.length()>0.001f)
               norm.normalize();
            else
               norm = Point3D(0,0,1);

            // grab tvert
            Point2D tvert(0,0);
            if (fnMesh.numUVs() > 0)
            {
               if (polyIter.hasUVs())
               {
                  S32 uvIndex;
                  if (polyIter.getUVIndex(vtx,uvIndex))
                     tvert = tverts[uvIndex];
               }
            }

            mIndices.push_back(addVertex(vert,norm,tvert,vertId));
         }
         mFaces.push_back(face);
      }

      getMaterials();

      // are we 2-sided?
      bool twoSided = false;
      getBool("twoSided",twoSided);
      if (twoSided)
      {
         S32 numFaces = mFaces.size();
         for (i=0; i<numFaces; i++)
         {
            S32 first = mFaces[i].firstElement;

            Primitive face;
            face.firstElement = mIndices.size();
            face.numElements = 3;
            face.type = mFaces[i].type;

            Point3D vert = mVerts[mIndices[first + 0]];
            Point2D tvert = mTVerts[mIndices[first + 0]];
            Point3D norm = -mNormals[mIndices[first + 0]];
            S32 vertId = mVertId[mIndices[first + 0]];
            mIndices.push_back(addVertex(vert,norm,tvert,vertId));

            vert = mVerts[mIndices[first + 2]];
            tvert = mTVerts[mIndices[first + 2]];
            norm = -mNormals[mIndices[first + 2]];
            vertId = mVertId[mIndices[first + 2]];
            mIndices.push_back(addVertex(vert,norm,tvert,vertId));

            vert = mVerts[mIndices[first + 1]];
            tvert = mTVerts[mIndices[first + 1]];
            norm = -mNormals[mIndices[first + 1]];
            vertId = mVertId[mIndices[first + 1]];
            mIndices.push_back(addVertex(vert,norm,tvert,vertId));
            mFaces.push_back(face);
         }
      }

      return Parent::lockMesh(time,objectOffset);
   }

   void MayaAppMesh::getMaterials()
   {
      // Maya material extraction code pulled from
      // findTexturesPerPolyCmd.cpp from Maya sample plugins.

      MStatus status;

      S32 instanceNum = 0;
      if (mPath.isInstanced())
         instanceNum = mPath.instanceNumber();

       // Get a list of all sets pertaining to the selected shape and the
       // members of those sets.
      MFnMesh fnMesh(mPath);
      MObjectArray sets;
      MObjectArray comps;
      if (!fnMesh.getConnectedSetsAndMembers(instanceNum, sets, comps, true))
      {
         AppConfig::SetExportError("maya6", "ERROR: getMaterials: MFnMesh::getConnectedSetsAndMembers\r\n");
         return;
      }

      // This is a (hack) workaround.  If we already have got at least one
      // material, don't accept mesh iterators that include the whole mesh
      // (would be an error anyway).
      bool gotOne = false;

      // Loop through all the sets.  If the set is a polygonal set, find the
      // shader attached to the and print out the texture file name for the
      // set along with the polygons in the set.
      //
      for ( U32 i=0; i<sets.length(); i++ )
      {
         MObject set = sets[i];
         MObject comp = comps[i];

         MFnSet fnSet( set, &status );
         if (MS::kFailure == status)
         {
            AppConfig::SetExportError("maya1", "ERROR: getMaterials: MFnSet::MFnSet\r\n");
            return;
         }

         // Special (hack) case in which we have an iterator over everything
         // yet we already have got at least one material...
         if (gotOne && (comp.apiType() == MFn::kInvalid))
            continue;

         // Make sure the set is a polygonal set.  If not, continue.
         MItMeshPolygon piter(mPath, comp, &status);
         if (MS::kFailure == status)
            continue;

         // Find the texture that is applied to this set.  First, get the
         // shading node connected to the set.  Then, if there is an input
         // attribute called "color", search upstream from it for a texture
         // file node.
         MObject shaderNode = findShader(set);
         if (shaderNode == MObject::kNullObj)
            continue;

         MPlug colorPlug = MFnDependencyNode(shaderNode).findPlug("color", &status);
         if (status == MS::kFailure)
            continue;

         MItDependencyGraph dgIt(colorPlug, MFn::kFileTexture,
                        MItDependencyGraph::kUpstream, 
                        MItDependencyGraph::kBreadthFirst,
                        MItDependencyGraph::kNodeLevel, 
                        &status);

         if (status == MS::kFailure)
            continue;
         
         dgIt.disablePruningOnFilter();

         // If no texture file node was found, just continue.
         if (dgIt.isDone())
            continue;
           
         // Print out the texture node name and texture file that it references.
         //
         MObject textureNode = dgIt.thisNode();
         MPlug filenamePlug = MFnDependencyNode(textureNode).findPlug("fileTextureName");
         MString textureName;
         filenamePlug.getValue(textureName);


         // look for filename plug on transparency channel -- if it has one, material is translucent
         bool hasTransparency = false;
         MPlug transPlug = MFnDependencyNode(shaderNode).findPlug("transparency", &status);
         if (status == MS::kFailure)
            continue;

         dgIt.resetTo(transPlug, MFn::kFileTexture,
              MItDependencyGraph::kUpstream, 
              MItDependencyGraph::kBreadthFirst,
              MItDependencyGraph::kNodeLevel);
         
         dgIt.disablePruningOnFilter();

         // If no texture file node was found, just continue.
         if (!dgIt.isDone())
         {
            // Print out the texture node name and texture file that it references.
            //
            MObject textureNode = dgIt.thisNode();
            MPlug filenamePlug = MFnDependencyNode(textureNode).findPlug("fileTextureName");
            MString textureName;
            filenamePlug.getValue(textureName);
            if (textureName.length() != 0)
               hasTransparency = true;
         }

         for (; !piter.isDone(); piter.next())
         {
            S32 faceIdx = piter.index();
            if (mFaces[faceIdx].type & Primitive::NoMaterial)
            {
               mFaces[faceIdx].type ^= Primitive::NoMaterial;
               mFaces[faceIdx].type |= mMaterials.size();

               // we have at least one material now
               gotOne = true;
            }
            else
               AppConfig::SetExportError("maya7", "Mesh::getMaterials: face with multiple materials found");
         }

         Material mat;
         mat.name = textureName.asChar();
         mat.reflectance = -1;
         mat.bump = -1;
         mat.detail = -1;
         mat.detailScale = 1.0f;
         mat.reflection = 0.0f;
         mat.flags = Material::SWrap|Material::TWrap|Material::NeverEnvMap;

         MFnLambertShader lshader(shaderNode,&status);
         if (status == MStatus::kSuccess)
         {
            F32 translucence = lshader.translucenceCoeff();
            F32 selfIlluminating = lshader.glowIntensity();
            if (translucence>0.1f || hasTransparency)
               mat.flags |= Material::Translucent;
            if (selfIlluminating>0.1f)
               mat.flags |= Material::SelfIlluminating;
         }
         MFnReflectShader rshader(shaderNode,&status);
         if (status == MStatus::kSuccess)
         {
            F32 reflectivity = rshader.reflectivity();
            if (reflectivity>0.01f)
            {
               mat.reflection = reflectivity;
               mat.flags ^= Material::NeverEnvMap;
            }

         }

         // now get the texture placement node
         status = dgIt.resetTo(colorPlug, MFn::kPlace2dTexture,
                               MItDependencyGraph::kUpstream, 
                               MItDependencyGraph::kBreadthFirst,
                               MItDependencyGraph::kNodeLevel);

         dgIt.disablePruningOnFilter();
         if (status != MS::kFailure)
               if (!dgIt.isDone())
         {
            MObject placeNode = dgIt.thisNode();

            MStatus stat;

            MFnDependencyNode fnNode(placeNode,&stat);

            bool swrap = true;
            bool twrap = true;
            MPlug plug = fnNode.findPlug(MString("wrapU"),&stat);
            plug.getValue(swrap);
            plug = fnNode.findPlug(MString("wrapV"),&stat);
            plug.getValue(swrap);

            if (!swrap)
               mat.flags ^= Material::SWrap;
            if (!twrap)
               mat.flags ^= Material::TWrap;
          }
           
         /*
         Need to deal with these flags:
         Additive          = 0x00000008,
         Subtractive       = 0x00000010,
         IFLMaterial       = 0x08000000,
         */

         //------------
         // If this is an ifl, then create the ifl material if it doesn't exist and mark as ifl
         const char * dot = strchr(textureName.asChar(),'.');
         if (dot && !stricmp(dot+1,"ifl"))
         {
            mat.flags |= Material::IFLMaterial;
            S32 matIdx = mMaterials.size();
            while (mIfls.size() <= U32(matIdx))
               mIfls.push_back(NULL);
            if (!mIfls[matIdx])
               mIfls[matIdx] = new AppIfl(mat.name.c_str());
         }

         mMaterials.push_back(mat);
      }
   }


   MObject MayaAppMesh::findShader( MObject& setNode )
   {
      //  Description: Find the shading node for the given shading group set node.
      MFnDependencyNode fnNode(setNode);
      MPlug shaderPlug = fnNode.findPlug("surfaceShader");
            
      if (!shaderPlug.isNull())
      {
         MPlugArray connectedPlugs;
         bool asSrc = false;
         bool asDst = true;
         shaderPlug.connectedTo( connectedPlugs, asDst, asSrc );

         if (connectedPlugs.length() != 1)
            AppConfig::SetExportError("maya4", "Error getting shader\r\n");
         else 
            return connectedPlugs[0].node();
      }         
      
      return MObject::kNullObj;
   }

   void MayaAppMesh::unlockMesh()
   {
      Parent::unlockMesh();

      // no more cleanup...but if there were some to do, we'd do it here
   }

   void MayaSetTime(const AppTime & time)
   {
      MTime mayaTime(time.getF64(),MTime::kSeconds);
      MGlobal::viewFrame(mayaTime);
   }

   Matrix<4,4,F32> MayaToDtsTransform(MMatrix & mayaMat)
   {
      Matrix<4,4,F32> mat;
      for (S32 i=0; i<4; i++)
         for (S32 j=0; j<4; j++)
            mat[j][i] = F32(mayaMat[i][j]);

      F64 scale;
      MDistance dist; // starts as internal units
      dist.setValue(1.0f);
      scale = dist.asMeters();

      mat[0][3] *= F32(scale);
      mat[1][3] *= F32(scale);
      mat[2][3] *= F32(scale);

      return mat;
   }

   bool CheckMayaStatus(MStatus & status)
   {
      if (!bool(status))
      {
         AppConfig::SetExportError("maya2", avar("Maya error: %s",status.errorString().asChar()));
         status = MStatus::kSuccess;
         return false;
      }
      return true;
   }

   void HandleMayaNegative(char * name)
   {
      S32 pos = strlen(name);
      while (pos)
      {
         --pos;
         if (isdigit(name[pos]))
            continue;
         if (name[pos]=='_')
            name[pos]='-';
         break;
      }
   }

}; // namespace DTS
