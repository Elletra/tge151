//-----------------------------------------------------------------------------
// Torque Game Engine 
// Copyright (C) GarageGames.com, Inc.
//-----------------------------------------------------------------------------

#ifdef _MSC_VER
#pragma warning(disable : 4786)
#endif

#include <maya/MPxFileTranslator.h>
#include <maya/MObject.h>
#include <maya/MGlobal.h>
#include <maya/MString.h>
#include <maya/MFnPlugin.h>
#include <fstream>
#include "appConfig.h"
#include "mayaSceneEnum.h"


class exportDTS : public MPxFileTranslator {
public:
	exportDTS ();
	virtual ~exportDTS (); 

	virtual MStatus 	writer (
							const MFileObject &file,
							const MString &optionsString,
							FileAccessMode mode
						);

public:
	virtual bool		haveWriteMethod () const;
	virtual MString 	defaultExtension () const;
	virtual MFileKind	identifyFile (
							const MFileObject &,
							const char *buffer,
							short size
						) const;

   static MStatus writeTS(const MFileObject &file,
                          const MString &optionsString,
                          FileAccessMode mode,
                          bool seqOnly);

	static void *		creator ();
};


class exportDSQ : public MPxFileTranslator {
public:
	exportDSQ ();
	virtual ~exportDSQ (); 

	virtual MStatus 	writer (
							const MFileObject &file,
							const MString &optionsString,
							FileAccessMode mode
						);

public:
	virtual bool		haveWriteMethod () const;
	virtual MString 	defaultExtension () const;
	virtual MFileKind	identifyFile (
							const MFileObject &,
							const char *buffer,
							short size
						) const;

	static void *		creator ();
};

//--------------------------------------------------------------------------------
//	Export functionality
//--------------------------------------------------------------------------------

MStatus exportDTS::writeTS(const MFileObject &file,const MString &optionsString,FileAccessMode mode, bool seqOnly)
{
   // start error logging, setup status for failure
	MStatus status = MS::kFailure;
   MGlobal::startErrorLogging();

   MString fileNameStr = file.fullName();
   const char * name = fileNameStr.asChar();

   if (!DTS::AppConfig::SetDumpFile(name))
   {
      MGlobal::doErrorLogEntry(DTS::avar("Unable to create dumpfile for shape \"%s\".",name));
      MGlobal::stopErrorLogging();
      return status;
   }

   if (seqOnly)
   {
      DTS::AppConfig::SetExportOptimized(false);
      DTS::AppConfig::PrintDump(-1,"Setting up for sequence export only (dsq) -- turning off optimized meshes");
   }
   else
   {
      DTS::AppConfig::SetExportOptimized(true);
      DTS::AppConfig::PrintDump(-1,"Setting up for full export (dts) -- turning on optimized meshes");
   }

   // Get config file if it exists...
   MString sourceFile;
   MGlobal::executeCommand("file -q -sn;",sourceFile);
   DTS::AppConfig::SearchConfigFile(sourceFile.asChar());

   DTS::MayaSceneEnum mayaSceneEnum;
   DTS::Shape * shape = mayaSceneEnum.processScene();
   if (!DTS::AppConfig::IsExportError())
   {
      std::ofstream os;

      // not sure why maya sometimes doesn't handle this, but on occassion
      // we get multiple dts's in filename if user selects a dts file
      // when saving.  Get rid of the extra dts here. [Or dsq]
      char * delMe = NULL;
      const char * extraEXT = seqOnly ? ".dsq.dsq" : ".dts.dts";
      if (strlen(name)>strlen(extraEXT) && !_stricmp(extraEXT,name + strlen(name) - strlen(extraEXT)))
      {
         delMe = new char [strlen(name)+1];
         delMe[strlen(name)-4] = '\0';
         name = delMe;
      }

      os.open(name,std::ofstream::binary);
      if (!seqOnly)
         shape->save(os);
      else
         shape->saveSequences(os);
      os.close();
      delete [] delMe;
   }
   DTS::AppConfig::CloseDumpFile();
   delete shape;

   if (DTS::AppConfig::IsExportError())
   {
      MGlobal::doErrorLogEntry(DTS::AppConfig::GetExportError());
      const char * cmdPattern = "confirmDialog -t \"Error Exporting DTS Shape\" -m \"%s\"";
      char * confirmDialog = new char[strlen(DTS::AppConfig::GetExportError()) + strlen(cmdPattern) + 1];
      sprintf(confirmDialog,cmdPattern,DTS::AppConfig::GetExportError());
      MGlobal::executeCommand(confirmDialog,true);
      MGlobal::displayError(MString(DTS::AppConfig::GetExportError()));
   }
   else
      status = MS::kSuccess;

   // stop error logging, return status
   MGlobal::stopErrorLogging();

   return status;
}

//--------------------------------------------------------------------------------
//	Plugin management
//--------------------------------------------------------------------------------

MStatus initializePlugin (MObject obj)
{
	MStatus status;
	MFnPlugin plugin(obj, "GarageGames", "0.9.6 beta");
	status = plugin.registerFileTranslator ("exportDTS", "", exportDTS::creator);
	status = plugin.registerFileTranslator ("exportDSQ", "", exportDSQ::creator);
	return (status);
}

MStatus uninitializePlugin (MObject obj)
{
	MFnPlugin plugin (obj);
	plugin.deregisterFileTranslator ("exportDTS");
	plugin.deregisterFileTranslator ("exportDSQ");
	return (MS::kSuccess);
}

//--------------------------------------------------------------------------------
//	Export DTS
//--------------------------------------------------------------------------------

bool exportDTS::haveWriteMethod () const
{
	return (true);
}

MString exportDTS::defaultExtension () const
{
	return (MString("dts"));
}

MPxFileTranslator::MFileKind exportDTS::identifyFile (const MFileObject &file, const char *buffer, short size) const
{
	const char * name = file.name().asChar();
	int nameLength = strlen(name);

	if ((nameLength > 4) && !_stricmp(name+nameLength-4, ".dts"))
		return (kIsMyFileType);

	return (kNotMyFileType);
}

void * exportDTS::creator ()
{
	return (new exportDTS);
}

exportDTS::exportDTS ()
{
}

exportDTS::~exportDTS ()
{
}

MStatus exportDTS::writer (const MFileObject &file,const MString &optionsString,FileAccessMode mode)
{
   return writeTS(file,optionsString,mode,false);
}

//--------------------------------------------------------------------------------
//	Export DSQ
//--------------------------------------------------------------------------------

bool exportDSQ::haveWriteMethod () const
{
	return (true);
}

MString exportDSQ::defaultExtension () const
{
	return (MString("dsq"));
}

MPxFileTranslator::MFileKind exportDSQ::identifyFile (const MFileObject &file, const char *buffer, short size) const
{
	const char * name = file.name().asChar();
	int nameLength = strlen(name);

	if ((nameLength > 4) && !_stricmp(name+nameLength-4, ".dsq"))
		return (kIsMyFileType);

	return (kNotMyFileType);
}

void * exportDSQ::creator ()
{
	return (new exportDSQ);
}


exportDSQ::exportDSQ ()
{
}

exportDSQ::~exportDSQ ()
{
}

MStatus exportDSQ::writer (const MFileObject &file,const MString &optionsString,FileAccessMode mode)
{
   return exportDTS::writeTS(file,optionsString,mode,true);
}

