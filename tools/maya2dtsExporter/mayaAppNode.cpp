//-----------------------------------------------------------------------------
// Torque Game Engine 
// Copyright (C) GarageGames.com, Inc.
//-----------------------------------------------------------------------------

#ifdef _MSC_VER
#pragma warning(disable : 4786)
#endif

#include "mayaAppNode.h"
#include "mayaAppMesh.h"
#include "appConfig.h"
#include "DTSUtil.h"
#include <maya/MFnDependencyNode.h>
#include <maya/MFnDagNode.h>
#include <maya/MFnTransform.h>
#include <maya/MGlobal.h>
#include <maya/MTime.h>
#include <maya/MPoint.h>
#include <maya/MMatrix.h>
#include <maya/MPlug.h>
#include <maya/MDGContext.h>
#include <maya/MAnimUtil.h>

namespace DTS
{

   MayaAppNode::MayaAppNode(MDagPath & path)
   {
      mPath = path;
   }

   MayaAppNode::~MayaAppNode()
   {
   }

   void MayaAppNode::buildMeshList()
   {
      MStatus status;
      for (U32 i=0; i<mPath.childCount(&status); i++)
      {
         if (mPath.child(i,&status).hasFn(MFn::kMesh) && !mPath.child(i,&status).hasFn(MFn::kTransform))
         {
            CheckMayaStatus(status);
            MDagPath path = mPath;
            MObject meshNode = mPath.child(i,&status);
            CheckMayaStatus(status);
            status = path.push(meshNode);
            CheckMayaStatus(status);
            if (!MFnDagNode(path).isIntermediateObject(&status))
               mMeshes.push_back(new MayaAppMesh(mPath,path));
            CheckMayaStatus(status);
         }
         CheckMayaStatus(status);
      }
      CheckMayaStatus(status);
   }

   void MayaAppNode::buildChildList()
   {
      MStatus status;
      for (U32 i=0; i<mPath.childCount(&status); i++)
      {
         if (!mPath.child(i,&status).hasFn(MFn::kMesh)  && mPath.child(i,&status).hasFn(MFn::kTransform))
         {
            CheckMayaStatus(status);
            MDagPath path = mPath;
            MObject childNode = mPath.child(i,&status);
            CheckMayaStatus(status);
            status = path.push(childNode);
            CheckMayaStatus(status);
            mChildNodes.push_back(new MayaAppNode(path));
         }
      }
      CheckMayaStatus(status);
   }

   Matrix<4,4,F32> MayaAppNode::getNodeTransform(const AppTime & time)
   {
      // we don't use the straight transform from maya because maya
      // doesn't display origin of transform in the gui
      // instead we adjust transform to use rotation pivot point as origin
      // that means the node transforms are different than maya uses,
      // but it's ok because the mesh transforms will be the same...
      // ...so as long as the user doesn't animate the rotation pivot,
      // it should all work out
      MStatus status;
      MayaSetTime(time);

      // get local space pivot point
      MStatus status1;
      MStatus status2;
      MStatus status3;
      MFnTransform trans(mPath,&status1);
      MPoint pivot = trans.transformation(&status2).rotatePivot(MSpace::kWorld,&status3);
      CheckMayaStatus(status1);
      CheckMayaStatus(status2);
      CheckMayaStatus(status3);

      // get world space transform
      MMatrix mayaMat = mPath.inclusiveMatrix(&status);
      CheckMayaStatus(status);

      // compute world space pivot point and revise world space transform
      MPoint wpivot = pivot * mayaMat;
      mayaMat[3][0] = wpivot[0];
      mayaMat[3][1] = wpivot[1];
      mayaMat[3][2] = wpivot[2];

      return MayaToDtsTransform(mayaMat);
   }

   bool MayaAppNode::isEqual(AppNode * node)
   {
      MayaAppNode * mayaNode = dynamic_cast<MayaAppNode*>(node);
      return mayaNode && mayaNode->mPath == mPath;
   }

   bool MayaAppNode::animatesTransform(const AppSequenceData & seqData)
   {
      MStatus status;
      bool ret = MAnimUtil::isAnimated (mPath, false, &status);
      CheckMayaStatus(status);
      return ret;
   }

   const char * MayaAppNode::getName()
   {
      MStatus status;
      if (!mName)
      {
         MFnDagNode fnNode(mPath,&status);
         CheckMayaStatus(status);
         const char * newname = fnNode.name(&status).asChar();
         if (newname)
         {
            mName = strnew(newname);
            HandleMayaNegative(mName);
         }
      }
      CheckMayaStatus(status);
      return mName;
   }

   const char * MayaAppNode::getParentName()
   {
      MStatus status;
      if (!mParentName)
      {
         MFnDagNode fnNode(mPath,&status);
         CheckMayaStatus(status);
         if (fnNode.parentCount(&status) > 0)
         {
            MObject parentNode = fnNode.parent(0,&status);
            MFnDependencyNode fnParent(parentNode,&status);
            const char * pname = fnParent.name(&status).asChar();
            if (pname)
            {
               mParentName = strnew(pname);
               HandleMayaNegative(mParentName);
            }
         }
      }
      CheckMayaStatus(status);
      return mParentName;
   }

   bool MayaAppNode::getFloat(const char * propName, F32  & defaultVal)
   {
      MStatus status;

      MFnDagNode fnNode(mPath,&status);
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

   bool   MayaAppNode::getInt(const char * propName, S32  & defaultVal)
   {
      MStatus status;

      MFnDagNode fnNode(mPath,&status);
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

   bool  MayaAppNode::getBool(const char * propName, bool & defaultVal)
   {
      MStatus status;

      MFnDagNode fnNode(mPath,&status);
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

   bool MayaAppNode::isParentRoot()
   {
      MStatus status;
      bool ret = false;

      // not sure the best way to do this
      // our parent is the root if our parent has no parent
      MObject parentNode = mPath.node(&status);
      MFnDagNode fnNode(parentNode,&status);
      if (fnNode.parentCount(&status) > 0)
      {
         ret = !stricmp( MFnDagNode(fnNode.parent(0,&status)).name().asChar(), "world");
      }
      CheckMayaStatus(status);
      return ret;
   }

}; // namespace DTS


