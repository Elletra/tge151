# Microsoft Developer Studio Project File - Name="Torque Demo" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=Torque Demo - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Torque Demo.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Torque Demo.mak" CFG="Torque Demo - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Torque Demo - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "Torque Demo - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Torque Demo - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../engine/out.VC6.RELEASE"
# PROP Intermediate_Dir "../engine/out.VC6.RELEASE"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MT /GR /O2 /I "../lib/xiph/include" /I "../lib/directx8" /I "../lib/vorbis/include" /I "../lib/zlib" /I "../lib/lungif" /I "../lib/lpng" /I "../lib/ljpeg" /I "../lib/openal/win32" /I "../engine" /D "UNICODE" /YX /FD /c /Tp
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 unicows.lib kernel32.lib advapi32.lib user32.lib gdi32.lib shell32.lib comdlg32.lib version.lib mpr.lib rasapi32.lib winmm.lib winspool.lib vfw32.lib secur32.lib oleacc.lib oledlg.lib sensapi.lib imm32.lib wsock32.lib ljpeg.lib lpng.lib lungif.lib zlib.lib ogg_static.lib theora_static.lib vorbis_static.lib /nologo /subsystem:windows /incremental:yes /machine:I386 /nodefaultlib:"kernel32.lib" /nodefaultlib:"advapi32.lib" /nodefaultlib:"user32.lib" /nodefaultlib:"gdi32.lib" /nodefaultlib:"shell32.lib" /nodefaultlib:"comdlg32.lib" /nodefaultlib:"version.lib" /nodefaultlib:"mpr.lib" /nodefaultlib:"rasapi32.lib" /nodefaultlib:"winmm.lib" /nodefaultlib:"winspool.lib" /nodefaultlib:"vfw32.lib" /nodefaultlib:"secur32.lib" /nodefaultlib:"oleacc.lib" /nodefaultlib:"oledlg.lib" /nodefaultlib:"sensapi.lib" /nodefaultlib:"imm32.lib" /nodefaultlib:"wsock32.lib" /nodefaultlib:"LIBC" /out:"../example/torqueDemo.exe" /libpath:"../lib/xiph/win32" /libpath:"../lib/MSPlatformSDK/Lib" /libpath:"../lib/out.VC6.RELEASE"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "Torque Demo - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../engine/out.VC6.DEBUG"
# PROP Intermediate_Dir "../engine/out.VC6.DEBUG"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MT /Gm /GR /ZI /Od /I "../lib/xiph/include" /I "../lib/SDL/include" /I "../lib/directx8" /I "../lib/vorbis/include" /I "../lib/zlib" /I "../lib/lungif" /I "../lib/lpng" /I "../lib/ljpeg" /I "../lib/openal/win32" /I "../engine" /D "UNICODE" /D "TORQUE_DEBUG" /FR /YX /FD /GZ /c /Tp
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 unicows.lib kernel32.lib advapi32.lib user32.lib gdi32.lib shell32.lib comdlg32.lib version.lib mpr.lib rasapi32.lib winmm.lib winspool.lib vfw32.lib secur32.lib oleacc.lib oledlg.lib sensapi.lib imm32.lib wsock32.lib ljpeg_DEBUG.lib lpng_DEBUG.lib lungif_DEBUG.lib zlib_DEBUG.lib ogg_static.lib theora_static.lib vorbis_static.lib /nologo /subsystem:console /debug /machine:I386 /nodefaultlib:"kernel32.lib" /nodefaultlib:"advapi32.lib" /nodefaultlib:"user32.lib" /nodefaultlib:"gdi32.lib" /nodefaultlib:"shell32.lib" /nodefaultlib:"comdlg32.lib" /nodefaultlib:"version.lib" /nodefaultlib:"mpr.lib" /nodefaultlib:"rasapi32.lib" /nodefaultlib:"winmm.lib" /nodefaultlib:"winspool.lib" /nodefaultlib:"vfw32.lib" /nodefaultlib:"secur32.lib" /nodefaultlib:"oleacc.lib" /nodefaultlib:"oledlg.lib" /nodefaultlib:"sensapi.lib" /nodefaultlib:"imm32.lib" /nodefaultlib:"wsock32.libLIBC" /nodefaultlib:"LIBCD" /out:"../example/torqueDemo_DEBUG.exe" /pdbtype:sept /libpath:"../lib/xiph/win32" /libpath:"../lib/SDL/win32" /libpath:"../lib/MSPlatformSDK/Lib" /libpath:"../lib/out.VC6.DEBUG"
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "Torque Demo - Win32 Release"
# Name "Torque Demo - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat;cc"
# Begin Group "collision"

# PROP Default_Filter "cc"
# Begin Source File

SOURCE=..\engine\collision\abstractPolyList.cc
# End Source File
# Begin Source File

SOURCE=..\engine\collision\abstractPolyList.h
# End Source File
# Begin Source File

SOURCE=..\engine\collision\boxConvex.cc
# End Source File
# Begin Source File

SOURCE=..\engine\collision\boxConvex.h
# End Source File
# Begin Source File

SOURCE=..\engine\collision\clippedPolyList.cc
# End Source File
# Begin Source File

SOURCE=..\engine\collision\clippedPolyList.h
# End Source File
# Begin Source File

SOURCE=..\engine\collision\collision.h
# End Source File
# Begin Source File

SOURCE=..\engine\collision\concretePolyList.cc
# End Source File
# Begin Source File

SOURCE=..\engine\collision\concretePolyList.h
# End Source File
# Begin Source File

SOURCE=..\engine\collision\convex.cc
# End Source File
# Begin Source File

SOURCE=..\engine\collision\convex.h
# End Source File
# Begin Source File

SOURCE=..\engine\collision\convexBrush.cc
# End Source File
# Begin Source File

SOURCE=..\engine\collision\convexBrush.h
# End Source File
# Begin Source File

SOURCE=..\engine\collision\depthSortList.cc
# End Source File
# Begin Source File

SOURCE=..\engine\collision\depthSortList.h
# End Source File
# Begin Source File

SOURCE=..\engine\collision\earlyOutPolyList.cc
# End Source File
# Begin Source File

SOURCE=..\engine\collision\earlyOutPolyList.h
# End Source File
# Begin Source File

SOURCE=..\engine\collision\extrudedPolyList.cc
# End Source File
# Begin Source File

SOURCE=..\engine\collision\extrudedPolyList.h
# End Source File
# Begin Source File

SOURCE=..\engine\collision\gjk.cc
# End Source File
# Begin Source File

SOURCE=..\engine\collision\gjk.h
# End Source File
# Begin Source File

SOURCE=..\engine\collision\optimizedPolyList.cc
# End Source File
# Begin Source File

SOURCE=..\engine\collision\optimizedPolyList.h
# End Source File
# Begin Source File

SOURCE=..\engine\collision\planeExtractor.cc
# End Source File
# Begin Source File

SOURCE=..\engine\collision\planeExtractor.h
# End Source File
# Begin Source File

SOURCE=..\engine\collision\polyhedron.cc
# End Source File
# Begin Source File

SOURCE=..\engine\collision\polyhedron.h
# End Source File
# Begin Source File

SOURCE=..\engine\collision\polytope.cc
# End Source File
# Begin Source File

SOURCE=..\engine\collision\polytope.h
# End Source File
# End Group
# Begin Group "console"

# PROP Default_Filter "cc"
# Begin Source File

SOURCE=..\engine\console\ast.h
# End Source File
# Begin Source File

SOURCE=..\engine\console\astAlloc.cc
# End Source File
# Begin Source File

SOURCE=..\engine\console\astNodes.cc
# End Source File
# Begin Source File

SOURCE=..\engine\console\basgram.cc
# End Source File
# Begin Source File

SOURCE=..\engine\console\basgram.h
# End Source File
# Begin Source File

SOURCE=..\engine\console\BASgram.y
# End Source File
# Begin Source File

SOURCE=..\engine\console\BASscan.cc
# End Source File
# Begin Source File

SOURCE=..\engine\console\BASscan.l
# End Source File
# Begin Source File

SOURCE=..\engine\console\cmdgram.cc
# End Source File
# Begin Source File

SOURCE=..\engine\console\cmdgram.h
# End Source File
# Begin Source File

SOURCE=..\engine\console\CMDgram.y
# End Source File
# Begin Source File

SOURCE=..\engine\console\CMDscan.cc
# End Source File
# Begin Source File

SOURCE=..\engine\console\CMDscan.l
# End Source File
# Begin Source File

SOURCE=..\engine\console\codeBlock.cc
# End Source File
# Begin Source File

SOURCE=..\engine\console\codeBlock.h
# End Source File
# Begin Source File

SOURCE=..\engine\console\compiledEval.cc
# End Source File
# Begin Source File

SOURCE=..\engine\console\compiler.cc
# End Source File
# Begin Source File

SOURCE=..\engine\console\compiler.h
# End Source File
# Begin Source File

SOURCE=..\engine\console\console.cc
# End Source File
# Begin Source File

SOURCE=..\engine\console\console.h
# End Source File
# Begin Source File

SOURCE=..\engine\console\consoleDoc.cc
# End Source File
# Begin Source File

SOURCE=..\engine\console\consoleDoc.h
# End Source File
# Begin Source File

SOURCE=..\engine\console\consoleFunctions.cc
# End Source File
# Begin Source File

SOURCE=..\engine\console\consoleInternal.cc
# End Source File
# Begin Source File

SOURCE=..\engine\console\consoleInternal.h
# End Source File
# Begin Source File

SOURCE=..\engine\console\consoleLogger.cc
# End Source File
# Begin Source File

SOURCE=..\engine\console\consoleLogger.h
# End Source File
# Begin Source File

SOURCE=..\engine\console\consoleObject.cc
# End Source File
# Begin Source File

SOURCE=..\engine\console\consoleObject.h
# End Source File
# Begin Source File

SOURCE=..\engine\console\consoleParser.cc
# End Source File
# Begin Source File

SOURCE=..\engine\console\consoleParser.h
# End Source File
# Begin Source File

SOURCE=..\engine\console\consoleTypes.cc
# End Source File
# Begin Source File

SOURCE=..\engine\console\consoleTypes.h
# End Source File
# Begin Source File

SOURCE=..\engine\console\dynamicTypes.cc
# End Source File
# Begin Source File

SOURCE=..\engine\console\dynamicTypes.h
# End Source File
# Begin Source File

SOURCE=..\engine\console\scriptObject.cc
# End Source File
# Begin Source File

SOURCE=..\engine\console\simBase.cc
# End Source File
# Begin Source File

SOURCE=..\engine\console\simBase.h
# End Source File
# Begin Source File

SOURCE=..\engine\console\simDictionary.cc
# End Source File
# Begin Source File

SOURCE=..\engine\console\simDictionary.h
# End Source File
# Begin Source File

SOURCE=..\engine\console\simManager.cc
# End Source File
# Begin Source File

SOURCE=..\engine\console\stringStack.cc
# End Source File
# Begin Source File

SOURCE=..\engine\console\stringStack.h
# End Source File
# Begin Source File

SOURCE=..\engine\console\telnetConsole.cc
# End Source File
# Begin Source File

SOURCE=..\engine\console\telnetConsole.h
# End Source File
# Begin Source File

SOURCE=..\engine\console\telnetDebugger.cc
# End Source File
# Begin Source File

SOURCE=..\engine\console\telnetDebugger.h
# End Source File
# Begin Source File

SOURCE=..\engine\console\typeValidators.cc
# End Source File
# Begin Source File

SOURCE=..\engine\console\typeValidators.h
# End Source File
# End Group
# Begin Group "core"

# PROP Default_Filter "cc"
# Begin Source File

SOURCE=..\engine\core\bitMatrix.h
# End Source File
# Begin Source File

SOURCE=..\engine\core\bitRender.cc
# End Source File
# Begin Source File

SOURCE=..\engine\core\bitRender.h
# End Source File
# Begin Source File

SOURCE=..\engine\core\bitSet.h
# End Source File
# Begin Source File

SOURCE=..\engine\core\bitStream.cc
# End Source File
# Begin Source File

SOURCE=..\engine\core\bitStream.h
# End Source File
# Begin Source File

SOURCE=..\engine\core\BitTables.cc
# End Source File
# Begin Source File

SOURCE=..\engine\core\bitTables.h
# End Source File
# Begin Source File

SOURCE=..\engine\core\bitVector.h
# End Source File
# Begin Source File

SOURCE=..\engine\core\bitVectorW.h
# End Source File
# Begin Source File

SOURCE=..\engine\core\chunkFile.cc
# End Source File
# Begin Source File

SOURCE=..\engine\core\chunkFile.h
# End Source File
# Begin Source File

SOURCE=..\engine\core\color.h
# End Source File
# Begin Source File

SOURCE=..\engine\core\coreRes.h
# End Source File
# Begin Source File

SOURCE=..\engine\core\crc.cc
# End Source File
# Begin Source File

SOURCE=..\engine\core\crc.h
# End Source File
# Begin Source File

SOURCE=..\engine\core\dataChunker.cc
# End Source File
# Begin Source File

SOURCE=..\engine\core\dataChunker.h
# End Source File
# Begin Source File

SOURCE=..\engine\core\dnet.cc
# End Source File
# Begin Source File

SOURCE=..\engine\core\dnet.h
# End Source File
# Begin Source File

SOURCE=..\engine\core\fileio.h
# End Source File
# Begin Source File

SOURCE=..\engine\core\fileObject.cc
# End Source File
# Begin Source File

SOURCE=..\engine\core\fileObject.h
# End Source File
# Begin Source File

SOURCE=..\engine\core\fileStream.cc
# End Source File
# Begin Source File

SOURCE=..\engine\core\fileStream.h
# End Source File
# Begin Source File

SOURCE=..\engine\core\filterStream.cc
# End Source File
# Begin Source File

SOURCE=..\engine\core\filterStream.h
# End Source File
# Begin Source File

SOURCE=..\engine\core\findMatch.cc
# End Source File
# Begin Source File

SOURCE=..\engine\core\findMatch.h
# End Source File
# Begin Source File

SOURCE=..\engine\core\frameAllocator.cc
# End Source File
# Begin Source File

SOURCE=..\engine\core\frameAllocator.h
# End Source File
# Begin Source File

SOURCE=..\engine\core\idGenerator.cc
# End Source File
# Begin Source File

SOURCE=..\engine\core\idGenerator.h
# End Source File
# Begin Source File

SOURCE=..\engine\core\iTickable.cc
# End Source File
# Begin Source File

SOURCE=..\engine\core\iTickable.h
# End Source File
# Begin Source File

SOURCE=..\engine\core\llist.h
# End Source File
# Begin Source File

SOURCE=..\engine\core\memStream.cc
# End Source File
# Begin Source File

SOURCE=..\engine\core\memstream.h
# End Source File
# Begin Source File

SOURCE=..\engine\core\nStream.cc
# End Source File
# Begin Source File

SOURCE=..\engine\core\nTypes.cc
# End Source File
# Begin Source File

SOURCE=..\engine\core\polyList.h
# End Source File
# Begin Source File

SOURCE=..\engine\core\realComp.h
# End Source File
# Begin Source File

SOURCE=..\engine\core\resDictionary.cc
# End Source File
# Begin Source File

SOURCE=..\engine\core\resizeStream.cc
# End Source File
# Begin Source File

SOURCE=..\engine\core\resizeStream.h
# End Source File
# Begin Source File

SOURCE=..\engine\core\resManager.cc
# End Source File
# Begin Source File

SOURCE=..\engine\core\resManager.h
# End Source File
# Begin Source File

SOURCE=..\engine\core\stream.h
# End Source File
# Begin Source File

SOURCE=..\engine\core\stringBuffer.cc
# End Source File
# Begin Source File

SOURCE=..\engine\core\stringBuffer.h
# End Source File
# Begin Source File

SOURCE=..\engine\core\stringTable.cc
# End Source File
# Begin Source File

SOURCE=..\engine\core\stringTable.h
# End Source File
# Begin Source File

SOURCE=..\engine\core\tAlgorithm.h
# End Source File
# Begin Source File

SOURCE=..\engine\core\theoraPlayer.cc
# End Source File
# Begin Source File

SOURCE=..\engine\core\theoraPlayer.h
# End Source File
# Begin Source File

SOURCE=..\engine\core\tokenizer.cc
# End Source File
# Begin Source File

SOURCE=..\engine\core\tokenizer.h
# End Source File
# Begin Source File

SOURCE=..\engine\core\torqueConfig.h
# End Source File
# Begin Source File

SOURCE=..\engine\core\tSparseArray.h
# End Source File
# Begin Source File

SOURCE=..\engine\core\tVector.cc
# End Source File
# Begin Source File

SOURCE=..\engine\core\tVector.h
# End Source File
# Begin Source File

SOURCE=..\engine\core\unicode.cc
# End Source File
# Begin Source File

SOURCE=..\engine\core\unicode.h
# End Source File
# Begin Source File

SOURCE=..\engine\core\zipAggregate.cc
# End Source File
# Begin Source File

SOURCE=..\engine\core\zipAggregate.h
# End Source File
# Begin Source File

SOURCE=..\engine\core\zipHeaders.cc
# End Source File
# Begin Source File

SOURCE=..\engine\core\zipHeaders.h
# End Source File
# Begin Source File

SOURCE=..\engine\core\zipSubStream.cc
# End Source File
# Begin Source File

SOURCE=..\engine\core\zipSubStream.h
# End Source File
# End Group
# Begin Group "dgl"

# PROP Default_Filter "cc"
# Begin Source File

SOURCE=..\engine\dgl\bitmapBm8.cc
# End Source File
# Begin Source File

SOURCE=..\engine\dgl\bitmapBmp.cc
# End Source File
# Begin Source File

SOURCE=..\engine\dgl\bitmapGif.cc
# End Source File
# Begin Source File

SOURCE=..\engine\dgl\bitmapJpeg.cc
# End Source File
# Begin Source File

SOURCE=..\engine\dgl\bitmapPng.cc
# End Source File
# Begin Source File

SOURCE=..\engine\dgl\dgl.cc
# End Source File
# Begin Source File

SOURCE=..\engine\dgl\dgl.h
# End Source File
# Begin Source File

SOURCE=..\engine\dgl\dglMatrix.cc
# End Source File
# Begin Source File

SOURCE=..\engine\dgl\gBitmap.cc
# End Source File
# Begin Source File

SOURCE=..\engine\dgl\gBitmap.h
# End Source File
# Begin Source File

SOURCE=..\engine\dgl\gChunkedTexManager.h
# End Source File
# Begin Source File

SOURCE=..\engine\dgl\gDynamicTexture.cc
# End Source File
# Begin Source File

SOURCE=..\engine\dgl\gDynamicTexture.h
# End Source File
# Begin Source File

SOURCE=..\engine\dgl\gFont.cc
# End Source File
# Begin Source File

SOURCE=..\engine\dgl\gFont.h
# End Source File
# Begin Source File

SOURCE=..\engine\dgl\gPalette.cc
# End Source File
# Begin Source File

SOURCE=..\engine\dgl\gPalette.h
# End Source File
# Begin Source File

SOURCE=..\engine\dgl\gTexManager.cc
# End Source File
# Begin Source File

SOURCE=..\engine\dgl\gTexManager.h
# End Source File
# Begin Source File

SOURCE=..\engine\dgl\gVectorField.cc
# End Source File
# Begin Source File

SOURCE=..\engine\dgl\gVectorField.h
# End Source File
# Begin Source File

SOURCE=..\engine\dgl\lensFlare.cc
# End Source File
# Begin Source File

SOURCE=..\engine\dgl\lensFlare.h
# End Source File
# Begin Source File

SOURCE=..\engine\dgl\materialList.cc
# End Source File
# Begin Source File

SOURCE=..\engine\dgl\materialList.h
# End Source File
# Begin Source File

SOURCE=..\engine\dgl\materialPropertyMap.cc
# End Source File
# Begin Source File

SOURCE=..\engine\dgl\materialPropertyMap.h
# End Source File
# Begin Source File

SOURCE=..\engine\dgl\splineUtil.cc
# End Source File
# Begin Source File

SOURCE=..\engine\dgl\splineUtil.h
# End Source File
# Begin Source File

SOURCE=..\engine\dgl\stripCache.cc
# End Source File
# Begin Source File

SOURCE=..\engine\dgl\stripCache.h
# End Source File
# End Group
# Begin Group "editor"

# PROP Default_Filter "cc"
# Begin Source File

SOURCE=..\engine\editor\creator.cc
# End Source File
# Begin Source File

SOURCE=..\engine\editor\creator.h
# End Source File
# Begin Source File

SOURCE=..\engine\editor\editor.cc
# End Source File
# Begin Source File

SOURCE=..\engine\editor\editor.h
# End Source File
# Begin Source File

SOURCE=..\engine\editor\editTSCtrl.cc
# End Source File
# Begin Source File

SOURCE=..\engine\editor\editTSCtrl.h
# End Source File
# Begin Source File

SOURCE=..\engine\editor\guiTerrPreviewCtrl.cc
# End Source File
# Begin Source File

SOURCE=..\engine\editor\guiTerrPreviewCtrl.h
# End Source File
# Begin Source File

SOURCE=..\engine\editor\missionAreaEditor.cc
# End Source File
# Begin Source File

SOURCE=..\engine\editor\missionAreaEditor.h
# End Source File
# Begin Source File

SOURCE=..\engine\editor\terraformer.cc
# End Source File
# Begin Source File

SOURCE=..\engine\editor\terraformer.h
# End Source File
# Begin Source File

SOURCE=..\engine\editor\terraformerNoise.cc
# End Source File
# Begin Source File

SOURCE=..\engine\editor\terraformerNoise.h
# End Source File
# Begin Source File

SOURCE=..\engine\editor\terraformerTexture.cc
# End Source File
# Begin Source File

SOURCE=..\engine\editor\terrainActions.cc
# End Source File
# Begin Source File

SOURCE=..\engine\editor\terrainActions.h
# End Source File
# Begin Source File

SOURCE=..\engine\editor\terrainEditor.cc
# End Source File
# Begin Source File

SOURCE=..\engine\editor\terrainEditor.h
# End Source File
# Begin Source File

SOURCE=..\engine\editor\worldEditor.cc
# End Source File
# Begin Source File

SOURCE=..\engine\editor\worldEditor.h
# End Source File
# End Group
# Begin Group "game"

# PROP Default_Filter "cc"
# Begin Group "fps"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\engine\game\fps\guiClockHud.cc
# End Source File
# Begin Source File

SOURCE=..\engine\game\fps\guiCrossHairHud.cc
# End Source File
# Begin Source File

SOURCE=..\engine\game\fps\guiHealthBarHud.cc
# End Source File
# Begin Source File

SOURCE=..\engine\game\fps\guiShapeNameHud.cc
# End Source File
# End Group
# Begin Group "fx"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\engine\game\fx\cameraFXMgr.cc
# End Source File
# Begin Source File

SOURCE=..\engine\game\fx\cameraFXMgr.h
# End Source File
# Begin Source File

SOURCE=..\engine\game\fx\explosion.cc
# End Source File
# Begin Source File

SOURCE=..\engine\game\fx\explosion.h
# End Source File
# Begin Source File

SOURCE=..\engine\game\fx\fxFoliageReplicator.cc
# End Source File
# Begin Source File

SOURCE=..\engine\game\fx\fxFoliageReplicator.h
# End Source File
# Begin Source File

SOURCE=..\engine\game\fx\fxLight.cc
# End Source File
# Begin Source File

SOURCE=..\engine\game\fx\fxLight.h
# End Source File
# Begin Source File

SOURCE=..\engine\game\fx\fxRenderObject.cc
# End Source File
# Begin Source File

SOURCE=..\engine\game\fx\fxRenderObject.h
# End Source File
# Begin Source File

SOURCE=..\engine\game\fx\fxShapeReplicator.cc
# End Source File
# Begin Source File

SOURCE=..\engine\game\fx\fxShapeReplicator.h
# End Source File
# Begin Source File

SOURCE=..\engine\game\fx\fxSunLight.cc
# End Source File
# Begin Source File

SOURCE=..\engine\game\fx\fxSunLight.h
# End Source File
# Begin Source File

SOURCE=..\engine\game\fx\lightning.cc
# End Source File
# Begin Source File

SOURCE=..\engine\game\fx\lightning.h
# End Source File
# Begin Source File

SOURCE=..\engine\game\fx\particleEmitter.cc
# End Source File
# Begin Source File

SOURCE=..\engine\game\fx\particleEmitter.h
# End Source File
# Begin Source File

SOURCE=..\engine\game\fx\particleEngine.cc
# End Source File
# Begin Source File

SOURCE=..\engine\game\fx\particleEngine.h
# End Source File
# Begin Source File

SOURCE=..\engine\game\fx\precipitation.cc
# End Source File
# Begin Source File

SOURCE=..\engine\game\fx\precipitation.h
# End Source File
# Begin Source File

SOURCE=..\engine\game\fx\splash.cc
# End Source File
# Begin Source File

SOURCE=..\engine\game\fx\splash.h
# End Source File
# Begin Source File

SOURCE=..\engine\game\fx\underLava.cc
# End Source File
# Begin Source File

SOURCE=..\engine\game\fx\underLava.h
# End Source File
# Begin Source File

SOURCE=..\engine\game\fx\weatherLightning.cpp
# End Source File
# Begin Source File

SOURCE=..\engine\game\fx\weatherLightning.h
# End Source File
# End Group
# Begin Group "net"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\engine\game\net\httpObject.cc
# End Source File
# Begin Source File

SOURCE=..\engine\game\net\httpObject.h
# End Source File
# Begin Source File

SOURCE=..\engine\game\net\net.cc
# End Source File
# Begin Source File

SOURCE=..\engine\game\net\netTest.cc
# End Source File
# Begin Source File

SOURCE=..\engine\game\net\serverQuery.cc
# End Source File
# Begin Source File

SOURCE=..\engine\game\net\serverQuery.h
# End Source File
# Begin Source File

SOURCE=..\engine\game\net\tcpObject.cc
# End Source File
# Begin Source File

SOURCE=..\engine\game\net\tcpObject.h
# End Source File
# End Group
# Begin Group "vehicles"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\engine\game\vehicles\flyingVehicle.cc
# End Source File
# Begin Source File

SOURCE=..\engine\game\vehicles\flyingVehicle.h
# End Source File
# Begin Source File

SOURCE=..\engine\game\vehicles\guiSpeedometer.cc
# End Source File
# Begin Source File

SOURCE=..\engine\game\vehicles\hoverVehicle.cc
# End Source File
# Begin Source File

SOURCE=..\engine\game\vehicles\hoverVehicle.h
# End Source File
# Begin Source File

SOURCE=..\engine\game\vehicles\vehicle.cc
# End Source File
# Begin Source File

SOURCE=..\engine\game\vehicles\vehicle.h
# End Source File
# Begin Source File

SOURCE=..\engine\game\vehicles\vehicleBlocker.cc
# End Source File
# Begin Source File

SOURCE=..\engine\game\vehicles\vehicleBlocker.h
# End Source File
# Begin Source File

SOURCE=..\engine\game\vehicles\wheeledVehicle.cc
# End Source File
# Begin Source File

SOURCE=..\engine\game\vehicles\wheeledVehicle.h
# End Source File
# End Group
# Begin Source File

SOURCE=..\engine\game\aiConnection.cc
# End Source File
# Begin Source File

SOURCE=..\engine\game\aiConnection.h
# End Source File
# Begin Source File

SOURCE=..\engine\game\aiPlayer.cc
# End Source File
# Begin Source File

SOURCE=..\engine\game\aiPlayer.h
# End Source File
# Begin Source File

SOURCE=..\engine\game\aiWheeledVehicle.cc
# End Source File
# Begin Source File

SOURCE=..\engine\game\aiWheeledVehicle.h
# End Source File
# Begin Source File

SOURCE=..\engine\game\ambientAudioManager.cc
# End Source File
# Begin Source File

SOURCE=..\engine\game\ambientAudioManager.h
# End Source File
# Begin Source File

SOURCE=..\engine\game\audioEmitter.cc
# End Source File
# Begin Source File

SOURCE=..\engine\game\audioEmitter.h
# End Source File
# Begin Source File

SOURCE=..\engine\game\auth.h
# End Source File
# Begin Source File

SOURCE=..\engine\game\badWordFilter.cc
# End Source File
# Begin Source File

SOURCE=..\engine\game\badWordFilter.h
# End Source File
# Begin Source File

SOURCE=..\engine\game\banList.cc
# End Source File
# Begin Source File

SOURCE=..\engine\game\banList.h
# End Source File
# Begin Source File

SOURCE=..\engine\game\camera.cc
# End Source File
# Begin Source File

SOURCE=..\engine\game\camera.h
# End Source File
# Begin Source File

SOURCE=..\engine\game\cameraSpline.cc
# End Source File
# Begin Source File

SOURCE=..\engine\game\cameraSpline.h
# End Source File
# Begin Source File

SOURCE=..\engine\game\collisionTest.cc
# End Source File
# Begin Source File

SOURCE=..\engine\game\collisionTest.h
# End Source File
# Begin Source File

SOURCE=..\engine\game\debris.cc
# End Source File
# Begin Source File

SOURCE=..\engine\game\debris.h
# End Source File
# Begin Source File

SOURCE=..\engine\game\debugView.cc
# End Source File
# Begin Source File

SOURCE=..\engine\game\debugView.h
# End Source File
# Begin Source File

SOURCE=..\engine\game\demoGame.h
# End Source File
# Begin Source File

SOURCE=..\engine\game\fireballAtmosphere.cc
# End Source File
# Begin Source File

SOURCE=..\engine\game\fireballAtmosphere.h
# End Source File
# Begin Source File

SOURCE=..\engine\game\game.cc
# End Source File
# Begin Source File

SOURCE=..\engine\game\game.h
# End Source File
# Begin Source File

SOURCE=..\engine\game\gameBase.cc
# End Source File
# Begin Source File

SOURCE=..\engine\game\gameBase.h
# End Source File
# Begin Source File

SOURCE=..\engine\game\gameConnection.cc
# End Source File
# Begin Source File

SOURCE=..\engine\game\gameConnection.h
# End Source File
# Begin Source File

SOURCE=..\engine\game\gameConnectionEvents.cc
# End Source File
# Begin Source File

SOURCE=..\engine\game\gameConnectionEvents.h
# End Source File
# Begin Source File

SOURCE=..\engine\game\gameConnectionMoves.cc
# End Source File
# Begin Source File

SOURCE=..\engine\game\gameFunctions.cc
# End Source File
# Begin Source File

SOURCE=..\engine\game\gameFunctions.h
# End Source File
# Begin Source File

SOURCE=..\engine\game\gameProcess.cc
# End Source File
# Begin Source File

SOURCE=..\engine\game\gameTSCtrl.cc
# End Source File
# Begin Source File

SOURCE=..\engine\game\gameTSCtrl.h
# End Source File
# Begin Source File

SOURCE=..\engine\game\guiNoMouseCtrl.cc
# End Source File
# Begin Source File

SOURCE=..\engine\game\guiPlayerView.cc
# End Source File
# Begin Source File

SOURCE=..\engine\game\guiPlayerView.h
# End Source File
# Begin Source File

SOURCE=..\engine\game\item.cc
# End Source File
# Begin Source File

SOURCE=..\engine\game\item.h
# End Source File
# Begin Source File

SOURCE=..\engine\game\main.cc
# End Source File
# Begin Source File

SOURCE=..\engine\game\missionArea.cc
# End Source File
# Begin Source File

SOURCE=..\engine\game\missionArea.h
# End Source File
# Begin Source File

SOURCE=..\engine\game\missionMarker.cc
# End Source File
# Begin Source File

SOURCE=..\engine\game\missionMarker.h
# End Source File
# Begin Source File

SOURCE=..\engine\game\moveManager.h
# End Source File
# Begin Source File

SOURCE=..\engine\game\objectTypes.h
# End Source File
# Begin Source File

SOURCE=..\engine\game\pathCamera.cc
# End Source File
# Begin Source File

SOURCE=..\engine\game\pathCamera.h
# End Source File
# Begin Source File

SOURCE=..\engine\game\physicalZone.cc
# End Source File
# Begin Source File

SOURCE=..\engine\game\physicalZone.h
# End Source File
# Begin Source File

SOURCE=..\engine\game\player.cc
# End Source File
# Begin Source File

SOURCE=..\engine\game\player.h
# End Source File
# Begin Source File

SOURCE=..\engine\game\projectile.cc
# End Source File
# Begin Source File

SOURCE=..\engine\game\projectile.h
# End Source File
# Begin Source File

SOURCE=..\engine\game\resource.h
# End Source File
# Begin Source File

SOURCE=..\engine\game\rigid.cc
# End Source File
# Begin Source File

SOURCE=..\engine\game\rigid.h
# End Source File
# Begin Source File

SOURCE=..\engine\game\rigidShape.cc
# End Source File
# Begin Source File

SOURCE=..\engine\game\rigidShape.h
# End Source File
# Begin Source File

SOURCE=..\engine\game\scopeAlwaysShape.cc
# End Source File
# Begin Source File

SOURCE=..\engine\game\shadow.cc
# End Source File
# Begin Source File

SOURCE=..\engine\game\shadow.h
# End Source File
# Begin Source File

SOURCE=..\engine\game\shapeBase.cc
# End Source File
# Begin Source File

SOURCE=..\engine\game\shapeBase.h
# End Source File
# Begin Source File

SOURCE=..\engine\game\shapeCollision.cc
# End Source File
# Begin Source File

SOURCE=..\engine\game\shapeImage.cc
# End Source File
# Begin Source File

SOURCE=..\engine\game\showTSShape.cc
# End Source File
# Begin Source File

SOURCE=..\engine\game\showTSShape.h
# End Source File
# Begin Source File

SOURCE=..\engine\game\sphere.cc
# End Source File
# Begin Source File

SOURCE=..\engine\game\sphere.h
# End Source File
# Begin Source File

SOURCE=..\engine\game\staticShape.cc
# End Source File
# Begin Source File

SOURCE=..\engine\game\staticShape.h
# End Source File
# Begin Source File

SOURCE=..\engine\game\trigger.cc
# End Source File
# Begin Source File

SOURCE=..\engine\game\trigger.h
# End Source File
# Begin Source File

SOURCE=..\engine\game\tsStatic.cc
# End Source File
# Begin Source File

SOURCE=..\engine\game\tsStatic.h
# End Source File
# Begin Source File

SOURCE=..\engine\game\version.cc
# End Source File
# Begin Source File

SOURCE=..\engine\game\version.h
# End Source File
# End Group
# Begin Group "gui"

# PROP Default_Filter "cc"
# Begin Group "containers - gui"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\engine\gui\containers\guiCtrlArrayCtrl.cc
# End Source File
# Begin Source File

SOURCE=..\engine\gui\containers\guiCtrlArrayCtrl.h
# End Source File
# Begin Source File

SOURCE=..\engine\gui\containers\guiFrameCtrl.cc
# End Source File
# Begin Source File

SOURCE=..\engine\gui\containers\guiFrameCtrl.h
# End Source File
# Begin Source File

SOURCE=..\engine\gui\containers\guiPaneCtrl.cc
# End Source File
# Begin Source File

SOURCE=..\engine\gui\containers\guiPaneCtrl.h
# End Source File
# Begin Source File

SOURCE=..\engine\gui\containers\guiScrollCtrl.cc
# End Source File
# Begin Source File

SOURCE=..\engine\gui\containers\guiScrollCtrl.h
# End Source File
# Begin Source File

SOURCE=..\engine\gui\containers\guiStackCtrl.cc
# End Source File
# Begin Source File

SOURCE=..\engine\gui\containers\guiStackCtrl.h
# End Source File
# Begin Source File

SOURCE=..\engine\gui\containers\guiTabBookCtrl.cc
# End Source File
# Begin Source File

SOURCE=..\engine\gui\containers\guiTabBookCtrl.h
# End Source File
# Begin Source File

SOURCE=..\engine\gui\containers\guiWindowCtrl.cc
# End Source File
# Begin Source File

SOURCE=..\engine\gui\containers\guiWindowCtrl.h
# End Source File
# End Group
# Begin Group "controls - gui"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\engine\gui\controls\guiBackgroundCtrl.cc
# End Source File
# Begin Source File

SOURCE=..\engine\gui\controls\guiBackgroundCtrl.h
# End Source File
# Begin Source File

SOURCE=..\engine\gui\controls\guiBitmapBorderCtrl.cc
# End Source File
# Begin Source File

SOURCE=..\engine\gui\controls\guiBitmapButtonCtrl.cc
# End Source File
# Begin Source File

SOURCE=..\engine\gui\controls\guiBitmapButtonCtrl.h
# End Source File
# Begin Source File

SOURCE=..\engine\gui\controls\guiBitmapCtrl.cc
# End Source File
# Begin Source File

SOURCE=..\engine\gui\controls\guiBitmapCtrl.h
# End Source File
# Begin Source File

SOURCE=..\engine\gui\controls\guiBorderButton.cc
# End Source File
# Begin Source File

SOURCE=..\engine\gui\controls\guiButtonBaseCtrl.cc
# End Source File
# Begin Source File

SOURCE=..\engine\gui\controls\guiButtonBaseCtrl.h
# End Source File
# Begin Source File

SOURCE=..\engine\gui\controls\guiButtonCtrl.cc
# End Source File
# Begin Source File

SOURCE=..\engine\gui\controls\guiButtonCtrl.h
# End Source File
# Begin Source File

SOURCE=..\engine\gui\controls\guiCheckBoxCtrl.cc
# End Source File
# Begin Source File

SOURCE=..\engine\gui\controls\guiCheckBoxCtrl.h
# End Source File
# Begin Source File

SOURCE=..\engine\gui\controls\guiColorPicker.cc
# End Source File
# Begin Source File

SOURCE=..\engine\gui\controls\guiColorPicker.h
# End Source File
# Begin Source File

SOURCE=..\engine\gui\controls\guiConsole.cc
# End Source File
# Begin Source File

SOURCE=..\engine\gui\controls\guiConsole.h
# End Source File
# Begin Source File

SOURCE=..\engine\gui\controls\guiConsoleEditCtrl.cc
# End Source File
# Begin Source File

SOURCE=..\engine\gui\controls\guiConsoleEditCtrl.h
# End Source File
# Begin Source File

SOURCE=..\engine\gui\controls\guiConsoleTextCtrl.cc
# End Source File
# Begin Source File

SOURCE=..\engine\gui\controls\guiConsoleTextCtrl.h
# End Source File
# Begin Source File

SOURCE=..\engine\gui\controls\guiDirectoryFileListCtrl.cc
# End Source File
# Begin Source File

SOURCE=..\engine\gui\controls\guiDirectoryFileListCtrl.h
# End Source File
# Begin Source File

SOURCE=..\engine\gui\controls\guiDirectoryTreeCtrl.cc
# End Source File
# Begin Source File

SOURCE=..\engine\gui\controls\guiDirectoryTreeCtrl.h
# End Source File
# Begin Source File

SOURCE=..\engine\gui\controls\guiListBoxCtrl.cc
# End Source File
# Begin Source File

SOURCE=..\engine\gui\controls\guiListBoxCtrl.h
# End Source File
# Begin Source File

SOURCE=..\engine\gui\controls\guiMLTextCtrl.cc
# End Source File
# Begin Source File

SOURCE=..\engine\gui\controls\guiMLTextCtrl.h
# End Source File
# Begin Source File

SOURCE=..\engine\gui\controls\guiMLTextEditCtrl.cc
# End Source File
# Begin Source File

SOURCE=..\engine\gui\controls\guiMLTextEditCtrl.h
# End Source File
# Begin Source File

SOURCE=..\engine\gui\controls\guiPopUpCtrl.cc
# End Source File
# Begin Source File

SOURCE=..\engine\gui\controls\guiPopUpCtrl.h
# End Source File
# Begin Source File

SOURCE=..\engine\gui\controls\guiRadioCtrl.cc
# End Source File
# Begin Source File

SOURCE=..\engine\gui\controls\guiRadioCtrl.h
# End Source File
# Begin Source File

SOURCE=..\engine\gui\controls\guiSliderCtrl.cc
# End Source File
# Begin Source File

SOURCE=..\engine\gui\controls\guiSliderCtrl.h
# End Source File
# Begin Source File

SOURCE=..\engine\gui\controls\guiTabPageCtrl.cc
# End Source File
# Begin Source File

SOURCE=..\engine\gui\controls\GuiTabPageCtrl.h
# End Source File
# Begin Source File

SOURCE=..\engine\gui\controls\guiTextCtrl.cc
# End Source File
# Begin Source File

SOURCE=..\engine\gui\controls\guiTextCtrl.h
# End Source File
# Begin Source File

SOURCE=..\engine\gui\controls\guiTextEditCtrl.cc
# End Source File
# Begin Source File

SOURCE=..\engine\gui\controls\guiTextEditCtrl.h
# End Source File
# Begin Source File

SOURCE=..\engine\gui\controls\guiTextEditSliderCtrl.cc
# End Source File
# Begin Source File

SOURCE=..\engine\gui\controls\guiTextEditSliderCtrl.h
# End Source File
# Begin Source File

SOURCE=..\engine\gui\controls\guiTextListCtrl.cc
# End Source File
# Begin Source File

SOURCE=..\engine\gui\controls\guiTextListCtrl.h
# End Source File
# Begin Source File

SOURCE=..\engine\gui\controls\guiTreeViewCtrl.cc
# End Source File
# Begin Source File

SOURCE=..\engine\gui\controls\guiTreeViewCtrl.h
# End Source File
# End Group
# Begin Group "core - gui"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\engine\gui\core\guiArrayCtrl.cc
# End Source File
# Begin Source File

SOURCE=..\engine\gui\core\guiArrayCtrl.h
# End Source File
# Begin Source File

SOURCE=..\engine\gui\core\guiCanvas.cc
# End Source File
# Begin Source File

SOURCE=..\engine\gui\core\guiCanvas.h
# End Source File
# Begin Source File

SOURCE=..\engine\gui\core\guiControl.cc
# End Source File
# Begin Source File

SOURCE=..\engine\gui\core\guiControl.h
# End Source File
# Begin Source File

SOURCE=..\engine\gui\core\guiDefaultControlRender.cc
# End Source File
# Begin Source File

SOURCE=..\engine\gui\core\guiDefaultControlRender.h
# End Source File
# Begin Source File

SOURCE=..\engine\gui\core\guiTSControl.cc
# End Source File
# Begin Source File

SOURCE=..\engine\gui\core\guiTSControl.h
# End Source File
# Begin Source File

SOURCE=..\engine\gui\core\guiTypes.cc
# End Source File
# Begin Source File

SOURCE=..\engine\gui\core\guiTypes.h
# End Source File
# End Group
# Begin Group "editor - gui"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\engine\gui\editor\guiControlListPopup.cc
# End Source File
# Begin Source File

SOURCE=..\engine\gui\editor\guiDebugger.cc
# End Source File
# Begin Source File

SOURCE=..\engine\gui\editor\guiDebugger.h
# End Source File
# Begin Source File

SOURCE=..\engine\gui\editor\guiEditCtrl.cc
# End Source File
# Begin Source File

SOURCE=..\engine\gui\editor\guiEditCtrl.h
# End Source File
# Begin Source File

SOURCE=..\engine\gui\editor\guiFilterCtrl.cc
# End Source File
# Begin Source File

SOURCE=..\engine\gui\editor\guiFilterCtrl.h
# End Source File
# Begin Source File

SOURCE=..\engine\gui\editor\guiGraphCtrl.cc
# End Source File
# Begin Source File

SOURCE=..\engine\gui\editor\guiGraphCtrl.h
# End Source File
# Begin Source File

SOURCE=..\engine\gui\editor\guiInspector.cc
# End Source File
# Begin Source File

SOURCE=..\engine\gui\editor\guiInspector.h
# End Source File
# Begin Source File

SOURCE=..\engine\gui\editor\guiInspectorTypes.cc
# End Source File
# Begin Source File

SOURCE=..\engine\gui\editor\guiInspectorTypes.h
# End Source File
# Begin Source File

SOURCE=..\engine\gui\editor\guiMenuBar.cc
# End Source File
# Begin Source File

SOURCE=..\engine\gui\editor\guiMenuBar.h
# End Source File
# End Group
# Begin Group "game - gui"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\engine\gui\game\guiAviBitmapCtrl.cc
# End Source File
# Begin Source File

SOURCE=..\engine\gui\game\guiAviBitmapCtrl.h
# End Source File
# Begin Source File

SOURCE=..\engine\gui\game\guiChunkedBitmapCtrl.cc
# End Source File
# Begin Source File

SOURCE=..\engine\gui\game\guiFadeinBitmapCtrl.cc
# End Source File
# Begin Source File

SOURCE=..\engine\gui\game\guiMessageVectorCtrl.cc
# End Source File
# Begin Source File

SOURCE=..\engine\gui\game\guiMessageVectorCtrl.h
# End Source File
# Begin Source File

SOURCE=..\engine\gui\game\guiProgressCtrl.cc
# End Source File
# Begin Source File

SOURCE=..\engine\gui\game\guiProgressCtrl.h
# End Source File
# End Group
# Begin Group "utility - gui"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\engine\gui\utility\guiBubbleTextCtrl.cc
# End Source File
# Begin Source File

SOURCE=..\engine\gui\utility\guiBubbleTextCtrl.h
# End Source File
# Begin Source File

SOURCE=..\engine\gui\utility\guiInputCtrl.cc
# End Source File
# Begin Source File

SOURCE=..\engine\gui\utility\guiInputCtrl.h
# End Source File
# Begin Source File

SOURCE=..\engine\gui\utility\guiMouseEventCtrl.cc
# End Source File
# Begin Source File

SOURCE=..\engine\gui\utility\guiMouseEventCtrl.h
# End Source File
# Begin Source File

SOURCE=..\engine\gui\utility\guiTransitionCtrl.cc
# End Source File
# Begin Source File

SOURCE=..\engine\gui\utility\guiTransitionCtrl.h
# End Source File
# Begin Source File

SOURCE=..\engine\gui\utility\messageVector.cc
# End Source File
# Begin Source File

SOURCE=..\engine\gui\utility\messageVector.h
# End Source File
# End Group
# Begin Group "shiny - gui"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\engine\gui\shiny\guiEffectCanvas.cc
# End Source File
# Begin Source File

SOURCE=..\engine\gui\shiny\guiEffectCanvas.h
# End Source File
# Begin Source File

SOURCE=..\engine\gui\shiny\guiTheoraCtrl.cc
# End Source File
# Begin Source File

SOURCE=..\engine\gui\shiny\guiTheoraCtrl.h
# End Source File
# Begin Source File

SOURCE=..\engine\gui\shiny\guiTickCtrl.cc
# End Source File
# Begin Source File

SOURCE=..\engine\gui\shiny\guiTickCtrl.h
# End Source File
# End Group
# End Group
# Begin Group "interior"

# PROP Default_Filter "cc"
# Begin Source File

SOURCE=..\engine\interior\floorPlanRes.cc
# End Source File
# Begin Source File

SOURCE=..\engine\interior\floorPlanRes.h
# End Source File
# Begin Source File

SOURCE=..\engine\interior\fogCalc.h
# End Source File
# Begin Source File

SOURCE=..\engine\interior\forceField.cc
# End Source File
# Begin Source File

SOURCE=..\engine\interior\forceField.h
# End Source File
# Begin Source File

SOURCE=..\engine\interior\interior.cc
# End Source File
# Begin Source File

SOURCE=..\engine\interior\interior.h
# End Source File
# Begin Source File

SOURCE=..\engine\interior\interiorCollision.cc
# End Source File
# Begin Source File

SOURCE=..\engine\interior\interiorDebug.cc
# End Source File
# Begin Source File

SOURCE=..\engine\interior\interiorInstance.cc
# End Source File
# Begin Source File

SOURCE=..\engine\interior\interiorInstance.h
# End Source File
# Begin Source File

SOURCE=..\engine\interior\interiorIO.cc
# End Source File
# Begin Source File

SOURCE=..\engine\interior\interiorLightAnim.cc
# End Source File
# Begin Source File

SOURCE=..\engine\interior\interiorLMManager.cc
# End Source File
# Begin Source File

SOURCE=..\engine\interior\interiorLMManager.h
# End Source File
# Begin Source File

SOURCE=..\engine\interior\interiorMap.cc
# End Source File
# Begin Source File

SOURCE=..\engine\interior\interiorMap.h
# End Source File
# Begin Source File

SOURCE=..\engine\interior\interiorMapRes.cc
# End Source File
# Begin Source File

SOURCE=..\engine\interior\interiorMapRes.h
# End Source File
# Begin Source File

SOURCE=..\engine\interior\interiorRender.cc
# End Source File
# Begin Source File

SOURCE=..\engine\interior\interiorRes.cc
# End Source File
# Begin Source File

SOURCE=..\engine\interior\interiorRes.h
# End Source File
# Begin Source File

SOURCE=..\engine\interior\interiorResObjects.cc
# End Source File
# Begin Source File

SOURCE=..\engine\interior\interiorResObjects.h
# End Source File
# Begin Source File

SOURCE=..\engine\interior\interiorSubObject.cc
# End Source File
# Begin Source File

SOURCE=..\engine\interior\interiorSubObject.h
# End Source File
# Begin Source File

SOURCE=..\engine\interior\itf.h
# End Source File
# Begin Source File

SOURCE=..\engine\interior\itfdump.asm

!IF  "$(CFG)" == "Torque Demo - Win32 Release"

# Begin Custom Build
IntDir=.\../engine/out.VC6.RELEASE
WkspDir=.
InputPath=..\engine\interior\itfdump.asm
InputName=itfdump

"$(IntDir)/$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	"$(WkspDir)\..\bin\nasm\nasmw.exe" -f win32 $(InputPath) -o "$(IntDir)/$(InputName).obj"

# End Custom Build

!ELSEIF  "$(CFG)" == "Torque Demo - Win32 Debug"

# Begin Custom Build
IntDir=.\../engine/out.VC6.DEBUG
WkspDir=.
InputPath=..\engine\interior\itfdump.asm
InputName=itfdump

"$(IntDir)/$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	"$(WkspDir)\..\bin\nasm\nasmw.exe" -f win32 $(InputPath) -o "$(IntDir)/$(InputName).obj"

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\engine\interior\lightUpdateGrouper.cc
# End Source File
# Begin Source File

SOURCE=..\engine\interior\lightUpdateGrouper.h
# End Source File
# Begin Source File

SOURCE=..\engine\interior\mirrorSubObject.cc
# End Source File
# Begin Source File

SOURCE=..\engine\interior\mirrorSubObject.h
# End Source File
# Begin Source File

SOURCE=..\engine\interior\pathedInterior.cc
# End Source File
# Begin Source File

SOURCE=..\engine\interior\pathedInterior.h
# End Source File
# End Group
# Begin Group "math"

# PROP Default_Filter "cc"
# Begin Source File

SOURCE=..\engine\math\mathIO.h
# End Source File
# Begin Source File

SOURCE=..\engine\math\mathTypes.cc
# End Source File
# Begin Source File

SOURCE=..\engine\math\mathTypes.h
# End Source File
# Begin Source File

SOURCE=..\engine\math\mathUtils.cc
# End Source File
# Begin Source File

SOURCE=..\engine\math\mathUtils.h
# End Source File
# Begin Source File

SOURCE=..\engine\math\mBox.cc
# End Source File
# Begin Source File

SOURCE=..\engine\math\mBox.h
# End Source File
# Begin Source File

SOURCE=..\engine\math\mConsoleFunctions.cc
# End Source File
# Begin Source File

SOURCE=..\engine\math\mConstants.h
# End Source File
# Begin Source File

SOURCE=..\engine\math\mMath.h
# End Source File
# Begin Source File

SOURCE=..\engine\math\mMath_ASM.asm

!IF  "$(CFG)" == "Torque Demo - Win32 Release"

# Begin Custom Build
IntDir=.\../engine/out.VC6.RELEASE
WkspDir=.
InputPath=..\engine\math\mMath_ASM.asm
InputName=mMath_ASM

"$(IntDir)/$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	"$(WkspDir)\..\bin\nasm\nasmw.exe" -f win32 $(InputPath) -o "$(IntDir)/$(InputName).obj"

# End Custom Build

!ELSEIF  "$(CFG)" == "Torque Demo - Win32 Debug"

# Begin Custom Build
IntDir=.\../engine/out.VC6.DEBUG
WkspDir=.
InputPath=..\engine\math\mMath_ASM.asm
InputName=mMath_ASM

"$(IntDir)/$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	"$(WkspDir)\..\bin\nasm\nasmw.exe" -f win32 $(InputPath) -o "$(IntDir)/$(InputName).obj"

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\engine\math\mMath_C.cc
# End Source File
# Begin Source File

SOURCE=..\engine\math\mMathAMD.cc
# End Source File
# Begin Source File

SOURCE=..\engine\math\mMathAMD_ASM.asm

!IF  "$(CFG)" == "Torque Demo - Win32 Release"

# Begin Custom Build
IntDir=.\../engine/out.VC6.RELEASE
WkspDir=.
InputPath=..\engine\math\mMathAMD_ASM.asm
InputName=mMathAMD_ASM

"$(IntDir)/$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	"$(WkspDir)\..\bin\nasm\nasmw.exe" -f win32 $(InputPath) -o "$(IntDir)/$(InputName).obj"

# End Custom Build

!ELSEIF  "$(CFG)" == "Torque Demo - Win32 Debug"

# Begin Custom Build
IntDir=.\../engine/out.VC6.DEBUG
WkspDir=.
InputPath=..\engine\math\mMathAMD_ASM.asm
InputName=mMathAMD_ASM

"$(IntDir)/$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	"$(WkspDir)\..\bin\nasm\nasmw.exe" -f win32 $(InputPath) -o "$(IntDir)/$(InputName).obj"

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\engine\math\mMathFn.cc
# End Source File
# Begin Source File

SOURCE=..\engine\math\mMathFn.h
# End Source File
# Begin Source File

SOURCE=..\engine\math\mMathSSE.cc
# End Source File
# Begin Source File

SOURCE=..\engine\math\mMathSSE_ASM.asm

!IF  "$(CFG)" == "Torque Demo - Win32 Release"

# Begin Custom Build
IntDir=.\../engine/out.VC6.RELEASE
WkspDir=.
InputPath=..\engine\math\mMathSSE_ASM.asm
InputName=mMathSSE_ASM

"$(IntDir)/$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	"$(WkspDir)\..\bin\nasm\nasmw.exe" -f win32 $(InputPath) -o "$(IntDir)/$(InputName).obj"

# End Custom Build

!ELSEIF  "$(CFG)" == "Torque Demo - Win32 Debug"

# Begin Custom Build
IntDir=.\../engine/out.VC6.DEBUG
WkspDir=.
InputPath=..\engine\math\mMathSSE_ASM.asm
InputName=mMathSSE_ASM

"$(IntDir)/$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	"$(WkspDir)\..\bin\nasm\nasmw.exe" -f win32 $(InputPath) -o "$(IntDir)/$(InputName).obj"

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\engine\math\mMatrix.cc
# End Source File
# Begin Source File

SOURCE=..\engine\math\mMatrix.h
# End Source File
# Begin Source File

SOURCE=..\engine\math\mPlane.h
# End Source File
# Begin Source File

SOURCE=..\engine\math\mPlaneTransformer.cc
# End Source File
# Begin Source File

SOURCE=..\engine\math\mPlaneTransformer.h
# End Source File
# Begin Source File

SOURCE=..\engine\math\mPoint.h
# End Source File
# Begin Source File

SOURCE=..\engine\math\mQuadPatch.cc
# End Source File
# Begin Source File

SOURCE=..\engine\math\mQuadPatch.h
# End Source File
# Begin Source File

SOURCE=..\engine\math\mQuat.cc
# End Source File
# Begin Source File

SOURCE=..\engine\math\mQuat.h
# End Source File
# Begin Source File

SOURCE=..\engine\math\mRandom.cc
# End Source File
# Begin Source File

SOURCE=..\engine\math\mRandom.h
# End Source File
# Begin Source File

SOURCE=..\engine\math\mRect.h
# End Source File
# Begin Source File

SOURCE=..\engine\math\mSolver.cc
# End Source File
# Begin Source File

SOURCE=..\engine\math\mSphere.h
# End Source File
# Begin Source File

SOURCE=..\engine\math\mSplinePatch.cc
# End Source File
# Begin Source File

SOURCE=..\engine\math\mSplinePatch.h
# End Source File
# Begin Source File

SOURCE=..\engine\math\mTrig.h
# End Source File
# End Group
# Begin Group "platform"

# PROP Default_Filter "cc"
# Begin Source File

SOURCE=..\engine\platform\3DFX.H
# End Source File
# Begin Source File

SOURCE=..\engine\platform\event.h
# End Source File
# Begin Source File

SOURCE=..\engine\platform\gameInterface.cc
# End Source File
# Begin Source File

SOURCE=..\engine\platform\gameInterface.h
# End Source File
# Begin Source File

SOURCE=..\engine\platform\GLCoreFunc.h
# End Source File
# Begin Source File

SOURCE=..\engine\platform\GLExtFunc.h
# End Source File
# Begin Source File

SOURCE=..\engine\platform\GLUFunc.h
# End Source File
# Begin Source File

SOURCE=..\engine\platform\platform.cc
# End Source File
# Begin Source File

SOURCE=..\engine\platform\platform.h
# End Source File
# Begin Source File

SOURCE=..\engine\platform\platformAssert.cc
# End Source File
# Begin Source File

SOURCE=..\engine\platform\platformAssert.h
# End Source File
# Begin Source File

SOURCE=..\engine\platform\platformAudio.h
# End Source File
# Begin Source File

SOURCE=..\engine\platform\platformCPU.cc
# End Source File
# Begin Source File

SOURCE=..\engine\platform\platformCPUInfo.asm

!IF  "$(CFG)" == "Torque Demo - Win32 Release"

# Begin Custom Build
IntDir=.\../engine/out.VC6.RELEASE
WkspDir=.
InputPath=..\engine\platform\platformCPUInfo.asm
InputName=platformCPUInfo

"$(IntDir)/$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	"$(WkspDir)\..\bin\nasm\nasmw.exe" -f win32 $(InputPath) -o "$(IntDir)/$(InputName).obj"

# End Custom Build

!ELSEIF  "$(CFG)" == "Torque Demo - Win32 Debug"

# Begin Custom Build
IntDir=.\../engine/out.VC6.DEBUG
WkspDir=.
InputPath=..\engine\platform\platformCPUInfo.asm
InputName=platformCPUInfo

"$(IntDir)/$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	"$(WkspDir)\..\bin\nasm\nasmw.exe" -f win32 $(InputPath) -o "$(IntDir)/$(InputName).obj"

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\engine\platform\platformFileIO.cc
# End Source File
# Begin Source File

SOURCE=..\engine\platform\platformFont.h
# End Source File
# Begin Source File

SOURCE=..\engine\platform\platformInput.h
# End Source File
# Begin Source File

SOURCE=..\engine\platform\platformMemory.cc
# End Source File
# Begin Source File

SOURCE=..\engine\platform\platformMutex.h
# End Source File
# Begin Source File

SOURCE=..\engine\platform\platformRedBook.cc
# End Source File
# Begin Source File

SOURCE=..\engine\platform\platformRedBook.h
# End Source File
# Begin Source File

SOURCE=..\engine\platform\platformSemaphore.h
# End Source File
# Begin Source File

SOURCE=..\engine\platform\platformString.cc
# End Source File
# Begin Source File

SOURCE=..\engine\platform\platformThread.h
# End Source File
# Begin Source File

SOURCE=..\engine\platform\platformVideo.cc
# End Source File
# Begin Source File

SOURCE=..\engine\platform\platformVideo.h
# End Source File
# Begin Source File

SOURCE=..\engine\platform\profiler.cc
# End Source File
# Begin Source File

SOURCE=..\engine\platform\profiler.h
# End Source File
# Begin Source File

SOURCE=..\engine\platform\types.h
# End Source File
# Begin Source File

SOURCE=..\engine\platform\typesWin32.h
# End Source File
# End Group
# Begin Group "platformWin32"

# PROP Default_Filter "cc"
# Begin Source File

SOURCE=..\engine\platformWin32\GLWinExtFunc.h
# End Source File
# Begin Source File

SOURCE=..\engine\platformWin32\GLWinFunc.h
# End Source File
# Begin Source File

SOURCE=..\engine\platformWIN32\platformAL.h
# End Source File
# Begin Source File

SOURCE=..\engine\platformWIN32\platformGL.h
# End Source File
# Begin Source File

SOURCE=..\engine\platformWIN32\platformWin32.h
# End Source File
# Begin Source File

SOURCE=..\engine\platformWin32\winAsmBlit.cc
# End Source File
# Begin Source File

SOURCE=..\engine\platformWin32\winConsole.cc
# End Source File
# Begin Source File

SOURCE=..\engine\platformWIN32\winConsole.h
# End Source File
# Begin Source File

SOURCE=..\engine\platformWin32\winCPUInfo.cc
# End Source File
# Begin Source File

SOURCE=..\engine\platformWin32\winD3DVideo.cc
# End Source File
# Begin Source File

SOURCE=..\engine\platformWIN32\winD3DVideo.h
# End Source File
# Begin Source File

SOURCE=..\engine\platformWin32\winDInputDevice.cc
# End Source File
# Begin Source File

SOURCE=..\engine\platformWIN32\winDInputDevice.h
# End Source File
# Begin Source File

SOURCE=..\engine\platformWin32\winDirectInput.cc
# End Source File
# Begin Source File

SOURCE=..\engine\platformWIN32\winDirectInput.h
# End Source File
# Begin Source File

SOURCE=..\engine\platformWin32\winFileio.cc
# End Source File
# Begin Source File

SOURCE=..\engine\platformWin32\winFont.cc
# End Source File
# Begin Source File

SOURCE=..\engine\platformWin32\winGL.cc
# End Source File
# Begin Source File

SOURCE=..\engine\platformWin32\winGLSpecial.cc
# End Source File
# Begin Source File

SOURCE=..\engine\platformWin32\winInput.cc
# End Source File
# Begin Source File

SOURCE=..\engine\platformWin32\winMath.cc
# End Source File
# Begin Source File

SOURCE=..\engine\platformWin32\winMath_ASM.cc
# End Source File
# Begin Source File

SOURCE=..\engine\platformWin32\winMemory.cc
# End Source File
# Begin Source File

SOURCE=..\engine\platformWin32\winMutex.cc
# End Source File
# Begin Source File

SOURCE=..\engine\platformWin32\winNet.cc
# End Source File
# Begin Source File

SOURCE=..\engine\platformWin32\winOGLVideo.cc
# End Source File
# Begin Source File

SOURCE=..\engine\platformWIN32\winOGLVideo.h
# End Source File
# Begin Source File

SOURCE=..\engine\platformWin32\winOpenAL.cc
# End Source File
# Begin Source File

SOURCE=..\engine\platformWin32\winProcessControl.cc
# End Source File
# Begin Source File

SOURCE=..\engine\platformWin32\winRedbook.cc
# End Source File
# Begin Source File

SOURCE=..\engine\platformWin32\winSemaphore.cc
# End Source File
# Begin Source File

SOURCE=..\engine\platformWin32\winStrings.cc
# End Source File
# Begin Source File

SOURCE=..\engine\platformWin32\winThread.cc
# End Source File
# Begin Source File

SOURCE=..\engine\platformWin32\winTime.cc
# End Source File
# Begin Source File

SOURCE=..\engine\platformWin32\winV2Video.cc
# End Source File
# Begin Source File

SOURCE=..\engine\platformWIN32\winV2Video.h
# End Source File
# Begin Source File

SOURCE=..\engine\platformWin32\winWindow.cc
# End Source File
# End Group
# Begin Group "sceneGraph"

# PROP Default_Filter "cc"
# Begin Source File

SOURCE=..\engine\sceneGraph\detailManager.cc
# End Source File
# Begin Source File

SOURCE=..\engine\scenegraph\detailManager.h
# End Source File
# Begin Source File

SOURCE=..\engine\sceneGraph\lightManager.cc
# End Source File
# Begin Source File

SOURCE=..\engine\scenegraph\lightManager.h
# End Source File
# Begin Source File

SOURCE=..\engine\sceneGraph\sceneGraph.cc
# End Source File
# Begin Source File

SOURCE=..\engine\scenegraph\sceneGraph.h
# End Source File
# Begin Source File

SOURCE=..\engine\sceneGraph\sceneLighting.cc
# End Source File
# Begin Source File

SOURCE=..\engine\scenegraph\sceneLighting.h
# End Source File
# Begin Source File

SOURCE=..\engine\sceneGraph\sceneRoot.cc
# End Source File
# Begin Source File

SOURCE=..\engine\scenegraph\sceneRoot.h
# End Source File
# Begin Source File

SOURCE=..\engine\sceneGraph\sceneState.cc
# End Source File
# Begin Source File

SOURCE=..\engine\scenegraph\sceneState.h
# End Source File
# Begin Source File

SOURCE=..\engine\sceneGraph\sceneTraversal.cc
# End Source File
# Begin Source File

SOURCE=..\engine\sceneGraph\sgUtil.cc
# End Source File
# Begin Source File

SOURCE=..\engine\scenegraph\sgUtil.h
# End Source File
# Begin Source File

SOURCE=..\engine\sceneGraph\shadowVolumeBSP.cc
# End Source File
# Begin Source File

SOURCE=..\engine\scenegraph\shadowVolumeBSP.h
# End Source File
# Begin Source File

SOURCE=..\engine\sceneGraph\windingClipper.cc
# End Source File
# Begin Source File

SOURCE=..\engine\scenegraph\windingClipper.h
# End Source File
# End Group
# Begin Group "sim"

# PROP Default_Filter "cc"
# Begin Source File

SOURCE=..\engine\sim\actionMap.cc
# End Source File
# Begin Source File

SOURCE=..\engine\sim\actionMap.h
# End Source File
# Begin Source File

SOURCE=..\engine\sim\connectionStringTable.cc
# End Source File
# Begin Source File

SOURCE=..\engine\sim\connectionStringTable.h
# End Source File
# Begin Source File

SOURCE=..\engine\sim\decalManager.cc
# End Source File
# Begin Source File

SOURCE=..\engine\sim\decalManager.h
# End Source File
# Begin Source File

SOURCE=..\engine\sim\netConnection.cc
# End Source File
# Begin Source File

SOURCE=..\engine\sim\netConnection.h
# End Source File
# Begin Source File

SOURCE=..\engine\sim\netDownload.cc
# End Source File
# Begin Source File

SOURCE=..\engine\sim\netEvent.cc
# End Source File
# Begin Source File

SOURCE=..\engine\sim\netGhost.cc
# End Source File
# Begin Source File

SOURCE=..\engine\sim\netInterface.cc
# End Source File
# Begin Source File

SOURCE=..\engine\sim\netInterface.h
# End Source File
# Begin Source File

SOURCE=..\engine\sim\netObject.cc
# End Source File
# Begin Source File

SOURCE=..\engine\sim\netObject.h
# End Source File
# Begin Source File

SOURCE=..\engine\sim\netStringTable.cc
# End Source File
# Begin Source File

SOURCE=..\engine\sim\netStringTable.h
# End Source File
# Begin Source File

SOURCE=..\engine\sim\pathManager.cc
# End Source File
# Begin Source File

SOURCE=..\engine\sim\pathManager.h
# End Source File
# Begin Source File

SOURCE=..\engine\sim\sceneObject.cc
# End Source File
# Begin Source File

SOURCE=..\engine\sim\sceneObject.h
# End Source File
# Begin Source File

SOURCE=..\engine\sim\simPath.cc
# End Source File
# Begin Source File

SOURCE=..\engine\sim\simPath.h
# End Source File
# End Group
# Begin Group "terrain"

# PROP Default_Filter "cc"
# Begin Source File

SOURCE=..\engine\terrain\blender.cc
# End Source File
# Begin Source File

SOURCE=..\engine\terrain\blender.h
# End Source File
# Begin Source File

SOURCE=..\engine\terrain\blender_asm.asm

!IF  "$(CFG)" == "Torque Demo - Win32 Release"

# Begin Custom Build
IntDir=.\../engine/out.VC6.RELEASE
WkspDir=.
InputPath=..\engine\terrain\blender_asm.asm
InputName=blender_asm

"$(IntDir)/$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	"$(WkspDir)\..\bin\nasm\nasmw.exe" -f win32 $(InputPath) -o "$(IntDir)/$(InputName).obj"

# End Custom Build

!ELSEIF  "$(CFG)" == "Torque Demo - Win32 Debug"

# Begin Custom Build
IntDir=.\../engine/out.VC6.DEBUG
WkspDir=.
InputPath=..\engine\terrain\blender_asm.asm
InputName=blender_asm

"$(IntDir)/$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	"$(WkspDir)\..\bin\nasm\nasmw.exe" -f win32 $(InputPath) -o "$(IntDir)/$(InputName).obj"

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\engine\terrain\bvQuadTree.cc
# End Source File
# Begin Source File

SOURCE=..\engine\terrain\bvQuadTree.h
# End Source File
# Begin Source File

SOURCE=..\engine\terrain\fluid.h
# End Source File
# Begin Source File

SOURCE=..\engine\terrain\fluidQuadTree.cc
# End Source File
# Begin Source File

SOURCE=..\engine\terrain\fluidRender.cc
# End Source File
# Begin Source File

SOURCE=..\engine\terrain\fluidSupport.cc
# End Source File
# Begin Source File

SOURCE=..\engine\terrain\sky.cc
# End Source File
# Begin Source File

SOURCE=..\engine\terrain\sky.h
# End Source File
# Begin Source File

SOURCE=..\engine\terrain\sun.cc
# End Source File
# Begin Source File

SOURCE=..\engine\terrain\sun.h
# End Source File
# Begin Source File

SOURCE=..\engine\terrain\terrCollision.cc
# End Source File
# Begin Source File

SOURCE=..\engine\terrain\terrData.cc
# End Source File
# Begin Source File

SOURCE=..\engine\terrain\terrData.h
# End Source File
# Begin Source File

SOURCE=..\engine\terrain\terrLighting.cc
# End Source File
# Begin Source File

SOURCE=..\engine\terrain\terrRender.cc
# End Source File
# Begin Source File

SOURCE=..\engine\terrain\terrRender.h
# End Source File
# Begin Source File

SOURCE=..\engine\terrain\waterBlock.cc
# End Source File
# Begin Source File

SOURCE=..\engine\terrain\waterBlock.h
# End Source File
# End Group
# Begin Group "ts"

# PROP Default_Filter "cc"
# Begin Source File

SOURCE=..\engine\ts\tsAnimate.cc
# End Source File
# Begin Source File

SOURCE=..\engine\ts\tsCollision.cc
# End Source File
# Begin Source File

SOURCE=..\engine\ts\tsDecal.cc
# End Source File
# Begin Source File

SOURCE=..\engine\ts\tsDecal.h
# End Source File
# Begin Source File

SOURCE=..\engine\ts\tsDump.cc
# End Source File
# Begin Source File

SOURCE=..\engine\ts\tsIntegerSet.cc
# End Source File
# Begin Source File

SOURCE=..\engine\ts\tsIntegerSet.h
# End Source File
# Begin Source File

SOURCE=..\engine\ts\tsLastDetail.cc
# End Source File
# Begin Source File

SOURCE=..\engine\ts\tsLastDetail.h
# End Source File
# Begin Source File

SOURCE=..\engine\ts\tsMaterialList.cc
# End Source File
# Begin Source File

SOURCE=..\engine\ts\tsMesh.cc
# End Source File
# Begin Source File

SOURCE=..\engine\ts\tsMesh.h
# End Source File
# Begin Source File

SOURCE=..\engine\ts\tsPartInstance.cc
# End Source File
# Begin Source File

SOURCE=..\engine\ts\tsPartInstance.h
# End Source File
# Begin Source File

SOURCE=..\engine\ts\tsShape.cc
# End Source File
# Begin Source File

SOURCE=..\engine\ts\tsShape.h
# End Source File
# Begin Source File

SOURCE=..\engine\ts\tsShapeAlloc.cc
# End Source File
# Begin Source File

SOURCE=..\engine\ts\tsShapeAlloc.h
# End Source File
# Begin Source File

SOURCE=..\engine\ts\tsShapeConstruct.cc
# End Source File
# Begin Source File

SOURCE=..\engine\ts\tsShapeConstruct.h
# End Source File
# Begin Source File

SOURCE=..\engine\ts\tsShapeInstance.cc
# End Source File
# Begin Source File

SOURCE=..\engine\ts\tsShapeInstance.h
# End Source File
# Begin Source File

SOURCE=..\engine\ts\tsShapeOldRead.cc
# End Source File
# Begin Source File

SOURCE=..\engine\ts\tsSortedMesh.cc
# End Source File
# Begin Source File

SOURCE=..\engine\ts\tsSortedMesh.h
# End Source File
# Begin Source File

SOURCE=..\engine\ts\tsThread.cc
# End Source File
# Begin Source File

SOURCE=..\engine\ts\tsTransform.cc
# End Source File
# Begin Source File

SOURCE=..\engine\ts\tsTransform.h
# End Source File
# End Group
# Begin Group "audio"

# PROP Default_Filter "cc"
# Begin Source File

SOURCE=..\engine\audio\audio.cc
# End Source File
# Begin Source File

SOURCE=..\engine\audio\audio.h
# End Source File
# Begin Source File

SOURCE=..\engine\audio\audioBuffer.cc
# End Source File
# Begin Source File

SOURCE=..\engine\audio\audioBuffer.h
# End Source File
# Begin Source File

SOURCE=..\engine\audio\audioDataBlock.cc
# End Source File
# Begin Source File

SOURCE=..\engine\audio\audioDataBlock.h
# End Source File
# Begin Source File

SOURCE=..\engine\audio\audioFunctions.cc
# End Source File
# Begin Source File

SOURCE=..\engine\audio\audioStreamSource.h
# End Source File
# Begin Source File

SOURCE=..\engine\audio\audioStreamSourceFactory.cc
# End Source File
# Begin Source File

SOURCE=..\engine\audio\audioStreamSourceFactory.h
# End Source File
# Begin Source File

SOURCE=..\engine\audio\oggMixedStreamSource.cc
# End Source File
# Begin Source File

SOURCE=..\engine\audio\oggMixedStreamSource.h
# End Source File
# Begin Source File

SOURCE=..\engine\audio\vorbisStream.cc
# End Source File
# Begin Source File

SOURCE=..\engine\audio\vorbisStream.h
# End Source File
# Begin Source File

SOURCE=..\engine\audio\vorbisStreamSource.cc
# End Source File
# Begin Source File

SOURCE=..\engine\audio\vorbisStreamSource.h
# End Source File
# Begin Source File

SOURCE=..\engine\audio\wavStreamSource.cc
# End Source File
# Begin Source File

SOURCE=..\engine\audio\wavStreamSource.h
# End Source File
# End Group
# Begin Group "i18n"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\engine\i18n\i18n.cc
# End Source File
# Begin Source File

SOURCE=..\engine\i18n\i18n.h
# End Source File
# Begin Source File

SOURCE=..\engine\i18n\lang.cc
# End Source File
# Begin Source File

SOURCE=..\engine\i18n\lang.h
# End Source File
# End Group
# Begin Group "utility"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\engine\util\fourcc.h
# End Source File
# Begin Source File

SOURCE=..\engine\util\frustrumCuller.cpp
# End Source File
# Begin Source File

SOURCE=..\engine\util\frustrumCuller.h
# End Source File
# Begin Source File

SOURCE=..\engine\util\quadTreeTracer.cpp
# End Source File
# Begin Source File

SOURCE=..\engine\util\quadTreeTracer.h
# End Source File
# Begin Source File

SOURCE=..\engine\util\rectClipper.cpp
# End Source File
# Begin Source File

SOURCE=..\engine\util\rectClipper.h
# End Source File
# Begin Source File

SOURCE=..\engine\util\safeDelete.h
# End Source File
# Begin Source File

SOURCE=..\engine\util\triBoxCheck.cpp
# End Source File
# Begin Source File

SOURCE=..\engine\util\triBoxCheck.h
# End Source File
# Begin Source File

SOURCE=..\engine\util\triRayCheck.cpp
# End Source File
# Begin Source File

SOURCE=..\engine\util\triRayCheck.h
# End Source File
# End Group
# Begin Group "lightingSystem"

# PROP Default_Filter "*.cpp;*.cc;*.h"
# Begin Source File

SOURCE=..\engine\lightingSystem\sgBinaryVolumePartitionTree.cc
# End Source File
# Begin Source File

SOURCE=..\engine\lightingSystem\sgBinaryVolumePartitionTree.h
# End Source File
# Begin Source File

SOURCE=..\engine\lightingSystem\sgDecalProjector.cc
# End Source File
# Begin Source File

SOURCE=..\engine\lightingSystem\sgDecalProjector.h
# End Source File
# Begin Source File

SOURCE=..\engine\lightingSystem\sgDetailMapping.cc
# End Source File
# Begin Source File

SOURCE=..\engine\lightingSystem\sgDetailMapping.h
# End Source File
# Begin Source File

SOURCE=..\engine\lightingSystem\sgHashMap.h
# End Source File
# Begin Source File

SOURCE=..\engine\lightingSystem\sgLighting.h
# End Source File
# Begin Source File

SOURCE=..\engine\lightingSystem\sgLightingModel.cc
# End Source File
# Begin Source File

SOURCE=..\engine\lightingSystem\sgLightingModel.h
# End Source File
# Begin Source File

SOURCE=..\engine\lightingSystem\sgLightManager.cc
# End Source File
# Begin Source File

SOURCE=..\engine\lightingSystem\sgLightManager.h
# End Source File
# Begin Source File

SOURCE=..\engine\lightingSystem\sgLightMap.cc
# End Source File
# Begin Source File

SOURCE=..\engine\lightingSystem\sgLightMap.h
# End Source File
# Begin Source File

SOURCE=..\engine\lightingSystem\sgLightObject.cc
# End Source File
# Begin Source File

SOURCE=..\engine\lightingSystem\sgLightObject.h
# End Source File
# Begin Source File

SOURCE=..\engine\lightingSystem\sgMissionLightingFilter.cc
# End Source File
# Begin Source File

SOURCE=..\engine\lightingSystem\sgMissionLightingFilter.h
# End Source File
# Begin Source File

SOURCE=..\engine\lightingSystem\sgObjectShadows.cc
# End Source File
# Begin Source File

SOURCE=..\engine\lightingSystem\sgObjectShadows.h
# End Source File
# Begin Source File

SOURCE=..\engine\lightingSystem\sgSceneLighting.cc
# End Source File
# Begin Source File

SOURCE=..\engine\lightingSystem\sgSceneLighting.h
# End Source File
# Begin Source File

SOURCE=..\engine\lightingSystem\sgSceneLightingGlobals.h
# End Source File
# Begin Source File

SOURCE=..\engine\lightingSystem\sgSceneLightingInterior.cc
# End Source File
# Begin Source File

SOURCE=..\engine\lightingSystem\sgSceneLightingTerrain.cc
# End Source File
# Begin Source File

SOURCE=..\engine\lightingSystem\sgScenePersist.cc
# End Source File
# Begin Source File

SOURCE=..\engine\lightingSystem\sgScenePersist.h
# End Source File
# Begin Source File

SOURCE=..\engine\lightingSystem\volLight.cc
# End Source File
# Begin Source File

SOURCE=..\engine\lightingSystem\volLight.h
# End Source File
# End Group
# Begin Group "constructor"

# PROP Default_Filter "cc"
# Begin Source File

SOURCE=..\engine\constructor\constructorSimpleMesh.cc
# End Source File
# Begin Source File

SOURCE=..\engine\constructor\constructorSimpleMesh.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\TorqueDemo.rc
# End Source File
# End Group
# End Target
# End Project
