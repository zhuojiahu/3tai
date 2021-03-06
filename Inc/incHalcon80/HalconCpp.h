/*****************************************************************************
 * HalconCpp.h
 ***************************************************************************** 
 *
 * Project:     HALCON/C++
 * Description: Defines / types / extern declarations for HALCON/C++ interface
 *
 * (c) 1996-2007 by MVTec Software GmbH
 *                  www.mvtec.com
 * 
 *****************************************************************************
 *
 * $Revision: 1.61 $
 * $Date: 2007/04/29 12:47:13 $
 *
 */

#ifndef H_CPP_H
#define H_CPP_H


// basic HALCON include files
#include "HCVersion.h"
#include "HIntDef.h"
#include "HDeclSpec.h"
#include "Hconst.h"
#include "HBase.h"
#include "IPtype.h"
#include "HCtype.h"
#include "Hmakro.h"
#include "Hextern.h"
#include "hlib/HError.h"



namespace Halcon {


#define     H_VERSION     8
#define     H_REVISION    0
#define     H_CREATION    "15.05.2007"

#define     H_STD       1       // status normal HTuple
#define     H_IGNORE    2       // status ignore values
#define     H_DEFAULT   3       // status default value
#define     H_UNDEF_TYPE  (-1)  // current value in CParType not defined;
                                // instead of STRING_PAR, LONG_PAR, etc
#define	    H_GLOBAL_ID 0       // global variable for "PROC_ID"


#define H_EMPTY_REGION      (Hkey)(1)   // empty region


typedef int   HBool;

typedef UINT1 HByte;
typedef INT2  HInt2;
typedef UINT2 HUInt2;


#define H_TRUE  (Halcon::HBool)1
#define H_FALSE (Halcon::HBool)0



class Hobject;
class HAbstractRegion;
class HBaseArray;
class HRegionArray;
class HXLD;
class HXLDArray;
class HXLDCont;
class HXLDContArray;
class HXLDPoly;
class HXLDPolyArray;
class HXLDPara;
class HXLDParaArray;
class HXLDModPara;
class HXLDModParaArray;
class HXLDExtPara;
class HXLDExtParaArray;
class HRegion;
class HImageArray;
class HImage;
class HByteImage;
class HInt1Image;
class HInt2Image;
class HUInt2Image;
class HInt4Image;
class HFloatImage;
class HComplexImage;
class HSemImage;
class HSemMultiImage;
class HSemAbstractMultiImage;
class HRGBImage;
class HWindow;
class HPixVal;
class HNumeric;
class HComplex;
class HAngleRad;
class HAngleDeg;
#ifndef COORD_IS_CLASS
typedef short HDCoord;
typedef float HCoord;
#else
class HDCoord;
class HCoord;
#endif
class HDPoint;
class HDPoint2D;
class HDPoint3D;
class HDVector;
class HDVector2D;
class HDVector3D;
class HPoint;
class HPoint2D;
class HPoint3D;
class HVector;
class HVector2D;
class HVector3D;
class HPolygon;
class HLine;
class HLine2D;
class HLine3D;
class HCircle;
class HEllipse;
class HRectangle1;
class HRectangle2;
class HTuple;
class HBitRegion;
class HByteRegion;
class HRLRegion;
class HRGBLUT;
class HFramegrabber;
class HNCCModel;
class HShapeModel;
class HComponentModel;
class HComponentTraining;
class HObjectModel3D;
class HShapeModel3D;
class HTemplate;
class HMeasure;
class HVariationModel;
class HFile;
class HSerial;
class HSocket;
class HBgEsti;
class HOCRBox;
class HOCRMlp;
class HOCRSvm;
class HLexicon;
class HOCV;
class HClassBox;
class HClassMlp;
class HClassSvm;
class HClassGmm;
class HFunction1D;
class HGnuplot;
class HBarCode;


// For HCPP.cxx: Passing of iconic data
#define H_REGION_ARRAY        1
#define H_REGION_SINGLE       2
#define H_IMAGE_ARRAY         3
#define H_IMAGE_SINGLE        4
#define H_XLD_CONT_ARRAY      5
#define H_XLD_CONT_SINGLE     6
#define H_XLD_POLY_ARRAY      7
#define H_XLD_POLY_SINGLE     8
#define H_XLD_PARA_ARRAY      9
#define H_XLD_PARA_SINGLE     10
#define H_XLD_MOD_PARA_ARRAY  11
#define H_XLD_MOD_PARA_SINGLE 12
#define H_XLD_EXT_PARA_ARRAY  13
#define H_XLD_EXT_PARA_SINGLE 14

extern LIntExport HTuple* _;      // ignore result values
extern LIntExport HTuple* __;     // use default values

}


#include "HException.h"
#include "HCPPUtil.h"
#include "HTuple.h"
#include "HOperator.h"
#include "HNumeric.h"
#include "HPixVal.h"
#include "HTools.h"
#include "HPrimitives.h"
#include "HRectangle.h"
#include "HDRegion.h"
#include "HCPPGlobal.h"
#include "HWindow.h"
#include "HImage.h"
#include "HByteImage.h"
#include "HImageArray.h"
#include "HRLRegion.h"
#include "HFloatImage.h"
#include "HComplexImage.h"
#include "HInt1Image.h"
#include "HInt2Image.h"
#include "HUInt2Image.h"
#include "HInt4Image.h"
#include "HConvol.h"
#include "HIterator.h"
#include "HRegion.h"
#include "HRegionArray.h"
#include "HXLD.h"
#include "HXLDArray.h"
#include "HXLDCont.h"
#include "HXLDContArray.h"
#include "HXLDPoly.h"
#include "HXLDPolyArray.h"
#include "HXLDPara.h"
#include "HXLDParaArray.h"
#include "HXLDModPara.h"
#include "HXLDModParaArray.h"
#include "HXLDExtPara.h"
#include "HXLDExtParaArray.h"
#include "HFramegrabber.h"
#include "HNCCModel.h"
#include "HShapeModel.h"
#include "HComponentModel.h"
#include "HComponentTraining.h"
#include "HObjectModel3D.h"
#include "HShapeModel3D.h"
#include "HVariationModel.h"
#include "HTemplate.h"
#include "HMeasure.h"
#include "HFile.h"
#include "HSerial.h"
#include "HSocket.h"
#include "HBgEsti.h"
#include "HOCRBox.h"
#include "HOCRMLP.h"
#include "HOCRSVM.h"
#include "HLexicon.h"
#include "HOCV.h"
#include "HClassBox.h"
#include "HClassMLP.h"
#include "HClassSVM.h"
#include "HClassGMM.h"
#include "HFunction1D.h"
#include "HGnuplot.h"
#include "HBarCode.h"
#include "HBarCode1D.h"
#include "HBarCode2D.h"
#include "HDataCode2D.h"

#include "HInterfaceCPP.h"

#endif
