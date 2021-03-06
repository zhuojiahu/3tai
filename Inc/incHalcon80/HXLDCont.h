/*****************************************************************************
 * HXLDCont.h
 ***************************************************************************** 
 *
 * Project:     Halcon/libhalcon
 * Description: Class HXLDCont
 *
 * (c) 1996-2006 by MVTec Software GmbH
 *                  www.mvtec.com
 * 
 *****************************************************************************
 *
 * $Revision: 1.17 $
 * $Date: 2006/07/27 14:02:38 $
 *
 */

#ifndef XLD_CONT_H
#define XLD_CONT_H


namespace Halcon {


/****************************************************************************/
/*                               HXLDCont                                   */
/****************************************************************************/

class HXLDContArray;
class LIntExport HXLDCont: public HXLD {
  friend class HXLDContArray;
public:
  HXLDCont(void): HXLD() {}
  HXLDCont(Hkey key);
  HXLDCont(const Hobject &obj);
  HXLDCont(const HXLDCont &cont);
  HXLDCont(const HXLDContArray &arr);
  HXLDCont(const HXLD &xld);
  ~HXLDCont(void) {}

  HXLDCont &operator= (const HXLDCont &cont);
  HXLDCont &operator= (const HXLDContArray &arr);
  HXLDCont &operator= (const HXLD &xld);

  // Tools
  virtual const char *HClassName(void) const;
  virtual const char *ClassName(void) const;
  virtual const char *InstClassName(void) const;
  virtual INT4        ClassDBID(void) const;
          HBool       Shared(void) const;

  // Class Functions

public:
  // Read XLD contours from a DXF file.
  static HXLDCont ReadContourXldDxf(const Halcon::HTuple &FileName, const Halcon::HTuple &GenParamNames, const Halcon::HTuple &GenParamValues, char *DxfStatus);
  static HXLDCont ReadContourXldDxf(const char *FileName, const char *GenParamNames, double GenParamValues, char *DxfStatus);
  // Write XLD contours to a file in DXF format.
  virtual void WriteContourXldDxf(const Halcon::HTuple &FileName) const;
  virtual void WriteContourXldDxf(const char *FileName) const;
  // Interactive modification of a NURBS curve using interpolation.
  static HXLDCont DrawNurbsInterpMod(const Halcon::HTuple &WindowHandle, const Halcon::HTuple &Rotate, const Halcon::HTuple &Move, const Halcon::HTuple &Scale, const Halcon::HTuple &KeepRatio, const Halcon::HTuple &Edit, const Halcon::HTuple &Degree, const Halcon::HTuple &RowsIn, const Halcon::HTuple &ColsIn, const Halcon::HTuple &TangentsIn, Halcon::HTuple *ControlRows, Halcon::HTuple *ControlCols, Halcon::HTuple *Knots, Halcon::HTuple *Rows, Halcon::HTuple *Cols, Halcon::HTuple *Tangents);
  // Interactive drawing of a NURBS curve using interpolation.
  static HXLDCont DrawNurbsInterp(const Halcon::HTuple &WindowHandle, const Halcon::HTuple &Rotate, const Halcon::HTuple &Move, const Halcon::HTuple &Scale, const Halcon::HTuple &KeepRatio, const Halcon::HTuple &Degree, Halcon::HTuple *ControlRows, Halcon::HTuple *ControlCols, Halcon::HTuple *Knots, Halcon::HTuple *Rows, Halcon::HTuple *Cols, Halcon::HTuple *Tangents);
  static HXLDCont DrawNurbsInterp(Hlong WindowHandle, const char *Rotate, const char *Move, const char *Scale, const char *KeepRatio, Hlong Degree, Halcon::HTuple *ControlRows, Halcon::HTuple *ControlCols, Halcon::HTuple *Knots, Halcon::HTuple *Rows, Halcon::HTuple *Cols, Halcon::HTuple *Tangents);
  // Interactive modification of a NURBS curve.
  static HXLDCont DrawNurbsMod(const Halcon::HTuple &WindowHandle, const Halcon::HTuple &Rotate, const Halcon::HTuple &Move, const Halcon::HTuple &Scale, const Halcon::HTuple &KeepRatio, const Halcon::HTuple &Edit, const Halcon::HTuple &Degree, const Halcon::HTuple &RowsIn, const Halcon::HTuple &ColsIn, const Halcon::HTuple &WeightsIn, Halcon::HTuple *Rows, Halcon::HTuple *Cols, Halcon::HTuple *Weights);
  // Interactive drawing of a NURBS curve.
  static HXLDCont DrawNurbs(const Halcon::HTuple &WindowHandle, const Halcon::HTuple &Rotate, const Halcon::HTuple &Move, const Halcon::HTuple &Scale, const Halcon::HTuple &KeepRatio, const Halcon::HTuple &Degree, Halcon::HTuple *Rows, Halcon::HTuple *Cols, Halcon::HTuple *Weights);
  static HXLDCont DrawNurbs(Hlong WindowHandle, const char *Rotate, const char *Move, const char *Scale, const char *KeepRatio, Hlong Degree, Halcon::HTuple *Rows, Halcon::HTuple *Cols, Halcon::HTuple *Weights);
  // Interactive modification of a contour.
  virtual HXLDCont DrawXldMod(const Halcon::HTuple &WindowHandle, const Halcon::HTuple &Rotate, const Halcon::HTuple &Move, const Halcon::HTuple &Scale, const Halcon::HTuple &KeepRatio, const Halcon::HTuple &Edit) const;
  virtual HXLDCont DrawXldMod(Hlong WindowHandle, const char *Rotate, const char *Move, const char *Scale, const char *KeepRatio, const char *Edit) const;
  // Interactive drawing of a contour.
  static HXLDCont DrawXld(const Halcon::HTuple &WindowHandle, const Halcon::HTuple &Rotate, const Halcon::HTuple &Move, const Halcon::HTuple &Scale, const Halcon::HTuple &KeepRatio);
  static HXLDCont DrawXld(Hlong WindowHandle, const char *Rotate, const char *Move, const char *Scale, const char *KeepRatio);
  // Calculate the minimum distance between two contours.
  virtual double DistanceCcMin(const HXLDCont &Contour2, const Halcon::HTuple &Mode) const;
  virtual double DistanceCcMin(const HXLDCont &Contour2, const char *Mode) const;
  virtual double DistanceCcMin(const HXLDContArray &Contour2, const Halcon::HTuple &Mode) const;
  virtual double DistanceCcMin(const HXLDContArray &Contour2, const char *Mode) const;
  // Calculate the distance between two contours.
  virtual double DistanceCc(const HXLDCont &Contour2, const Halcon::HTuple &Mode, double *DistanceMax) const;
  virtual double DistanceCc(const HXLDCont &Contour2, const char *Mode, double *DistanceMax) const;
  virtual double DistanceCc(const HXLDContArray &Contour2, const Halcon::HTuple &Mode, double *DistanceMax) const;
  virtual double DistanceCc(const HXLDContArray &Contour2, const char *Mode, double *DistanceMax) const;
  // Calculate the distance between one line segment and one contour.
  virtual HTuple DistanceSc(const Halcon::HTuple &Row1, const Halcon::HTuple &Column1, const Halcon::HTuple &Row2, const Halcon::HTuple &Column2, Halcon::HTuple *DistanceMax) const;
  virtual HTuple DistanceSc(double Row1, double Column1, double Row2, double Column2, Halcon::HTuple *DistanceMax) const;
  // Calculate the distance between one line and one contour.
  virtual HTuple DistanceLc(const Halcon::HTuple &Row1, const Halcon::HTuple &Column1, const Halcon::HTuple &Row2, const Halcon::HTuple &Column2, Halcon::HTuple *DistanceMax) const;
  virtual HTuple DistanceLc(double Row1, double Column1, double Row2, double Column2, Halcon::HTuple *DistanceMax) const;
  // Calculate the distance between one point and one contour.
  virtual HTuple DistancePc(const Halcon::HTuple &Row, const Halcon::HTuple &Column, Halcon::HTuple *DistanceMax) const;
  virtual HTuple DistancePc(double Row, double Column, Halcon::HTuple *DistanceMax) const;
  // Transform an XLD contour into the plane z=0 of a world coordinate system.
  virtual HXLDCont ContourToWorldPlaneXld(const Halcon::HTuple &CamParam, const Halcon::HTuple &WorldPose, const Halcon::HTuple &Scale) const;
  // Change the radial distortion of contours.
  virtual HXLDCont ChangeRadialDistortionContoursXld(const Halcon::HTuple &CamParIn, const Halcon::HTuple &CamParOut) const;
  // Create a region from an XLD contour.
  virtual HRegion GenRegionContourXld(const Halcon::HTuple &Mode) const;
  virtual HRegion GenRegionContourXld(const char *Mode) const;
  // Transform a contour in polar coordinates back to cartesian coordinates
  virtual HXLDContArray PolarTransContourXldInv(const Halcon::HTuple &Row, const Halcon::HTuple &Column, const Halcon::HTuple &AngleStart, const Halcon::HTuple &AngleEnd, const Halcon::HTuple &RadiusStart, const Halcon::HTuple &RadiusEnd, const Halcon::HTuple &WidthIn, const Halcon::HTuple &HeightIn, const Halcon::HTuple &Width, const Halcon::HTuple &Height) const;
  virtual HXLDContArray PolarTransContourXldInv(double Row, double Column, double AngleStart, double AngleEnd, double RadiusStart, double RadiusEnd, Hlong WidthIn, Hlong HeightIn, Hlong Width, Hlong Height) const;
  // Transform a contour in an annular arc to polar coordinates.
  virtual HXLDCont PolarTransContourXld(const Halcon::HTuple &Row, const Halcon::HTuple &Column, const Halcon::HTuple &AngleStart, const Halcon::HTuple &AngleEnd, const Halcon::HTuple &RadiusStart, const Halcon::HTuple &RadiusEnd, const Halcon::HTuple &Width, const Halcon::HTuple &Height) const;
  virtual HXLDCont PolarTransContourXld(double Row, double Column, double AngleStart, double AngleEnd, double RadiusStart, double RadiusEnd, Hlong Width, Hlong Height) const;
  // Transforms a NURBS curve into an XLD contour.
  static HXLDCont GenContourNurbsXld(const Halcon::HTuple &Rows, const Halcon::HTuple &Cols, const Halcon::HTuple &Knots, const Halcon::HTuple &Weights, const Halcon::HTuple &Degree, const Halcon::HTuple &MaxError, const Halcon::HTuple &MaxDistance);
  // Union of closed contours.
  virtual HXLDCont Union2ClosedContoursXld(const HXLDCont &Contours2) const;
  virtual HXLDCont Union2ClosedContoursXld(const HXLDContArray &Contours2) const;
  // Symmetric difference of closed contours.
  virtual HXLDCont SymmDifferenceClosedContoursXld(const HXLDCont &Contours2) const;
  virtual HXLDCont SymmDifferenceClosedContoursXld(const HXLDContArray &Contours2) const;
  // Difference of closed contours.
  virtual HXLDCont DifferenceClosedContoursXld(const HXLDCont &Sub) const;
  virtual HXLDCont DifferenceClosedContoursXld(const HXLDContArray &Sub) const;
  // Intersection of closed contours.
  virtual HXLDCont IntersectionClosedContoursXld(const HXLDCont &Contours2) const;
  virtual HXLDCont IntersectionClosedContoursXld(const HXLDContArray &Contours2) const;
  // Crop an XLD contour.
  virtual HXLDCont CropContoursXld(const Halcon::HTuple &Row1, const Halcon::HTuple &Col1, const Halcon::HTuple &Row2, const Halcon::HTuple &Col2, const Halcon::HTuple &CloseContours) const;
  virtual HXLDCont CropContoursXld(double Row1, double Col1, double Row2, double Col2, const char *CloseContours) const;
  // Generate one XLD contour in the shape of a cross for each input point.
  static HXLDCont GenCrossContourXld(const Halcon::HTuple &Row, const Halcon::HTuple &Col, const Halcon::HTuple &Size, const Halcon::HTuple &Angle);
  static HXLDCont GenCrossContourXld(double Row, double Col, double Size, double Angle);
  // Merge XLD contours from successive line scan images.
  virtual HXLDCont MergeContLineScanXld(const HXLDCont &PrevConts, HXLDCont *PrevMergedConts, const Halcon::HTuple &ImageHeight, const Halcon::HTuple &Margin, const Halcon::HTuple &MergeBorder, const Halcon::HTuple &MaxImagesCont) const;
  virtual HXLDCont MergeContLineScanXld(const HXLDCont &PrevConts, HXLDCont *PrevMergedConts, Hlong ImageHeight, double Margin, const char *MergeBorder, Hlong MaxImagesCont) const;
  virtual HXLDCont MergeContLineScanXld(const HXLDContArray &PrevConts, HXLDCont *PrevMergedConts, const Halcon::HTuple &ImageHeight, const Halcon::HTuple &Margin, const Halcon::HTuple &MergeBorder, const Halcon::HTuple &MaxImagesCont) const;
  virtual HXLDCont MergeContLineScanXld(const HXLDContArray &PrevConts, HXLDCont *PrevMergedConts, Hlong ImageHeight, double Margin, const char *MergeBorder, Hlong MaxImagesCont) const;
  // Read XLD contours to a file in ARC/INFO generate format.
  static HXLDCont ReadContourXldArcInfo(const Halcon::HTuple &FileName);
  static HXLDCont ReadContourXldArcInfo(const char *FileName);
  // Write XLD contours to a file in ARC/INFO generate format.
  virtual void WriteContourXldArcInfo(const Halcon::HTuple &FileName) const;
  virtual void WriteContourXldArcInfo(const char *FileName) const;
  // Create an XLD contour in the shape of a rectangle.
  static HXLDCont GenRectangle2ContourXld(const Halcon::HTuple &Row, const Halcon::HTuple &Column, const Halcon::HTuple &Phi, const Halcon::HTuple &Length1, const Halcon::HTuple &Length2);
  static HXLDCont GenRectangle2ContourXld(double Row, double Column, double Phi, double Length1, double Length2);
  // Compute the distances of all contour points to a rectangle.
  virtual HTuple DistRectangle2ContourPointsXld(const Halcon::HTuple &ClippingEndPoints, const Halcon::HTuple &Row, const Halcon::HTuple &Column, const Halcon::HTuple &Phi, const Halcon::HTuple &Length1, const Halcon::HTuple &Length2) const;
  virtual HTuple DistRectangle2ContourPointsXld(Hlong ClippingEndPoints, double Row, double Column, double Phi, double Length1, double Length2) const;
  // Fit rectangles to XLD contours.
  virtual double FitRectangle2ContourXld(const Halcon::HTuple &Algorithm, const Halcon::HTuple &MaxNumPoints, const Halcon::HTuple &MaxClosureDist, const Halcon::HTuple &ClippingEndPoints, const Halcon::HTuple &Iterations, const Halcon::HTuple &ClippingFactor, double *Column, double *Phi, double *Length1, double *Length2, char *PointOrder) const;
  virtual double FitRectangle2ContourXld(const char *Algorithm, Hlong MaxNumPoints, double MaxClosureDist, Hlong ClippingEndPoints, Hlong Iterations, double ClippingFactor, double *Column, double *Phi, double *Length1, double *Length2, char *PointOrder) const;
  // Segment XLD contours into line segments and circular or elliptic arcs.
  virtual HXLDContArray SegmentContoursXld(const Halcon::HTuple &Mode, const Halcon::HTuple &SmoothCont, const Halcon::HTuple &MaxLineDist1, const Halcon::HTuple &MaxLineDist2) const;
  virtual HXLDContArray SegmentContoursXld(const char *Mode, Hlong SmoothCont, double MaxLineDist1, double MaxLineDist2) const;
  // Approximation of XLD contours by circles.
  virtual double FitCircleContourXld(const Halcon::HTuple &Algorithm, const Halcon::HTuple &MaxNumPoints, const Halcon::HTuple &MaxClosureDist, const Halcon::HTuple &ClippingEndPoints, const Halcon::HTuple &Iterations, const Halcon::HTuple &ClippingFactor, double *Column, double *Radius, double *StartPhi, double *EndPhi, char *PointOrder) const;
  virtual double FitCircleContourXld(const char *Algorithm, Hlong MaxNumPoints, double MaxClosureDist, Hlong ClippingEndPoints, Hlong Iterations, double ClippingFactor, double *Column, double *Radius, double *StartPhi, double *EndPhi, char *PointOrder) const;
  // Approximation of XLD contours by line segments.
  virtual double FitLineContourXld(const Halcon::HTuple &Algorithm, const Halcon::HTuple &MaxNumPoints, const Halcon::HTuple &ClippingEndPoints, const Halcon::HTuple &Iterations, const Halcon::HTuple &ClippingFactor, double *ColBegin, double *RowEnd, double *ColEnd, double *Nr, double *Nc, double *Dist) const;
  virtual double FitLineContourXld(const char *Algorithm, Hlong MaxNumPoints, Hlong ClippingEndPoints, Hlong Iterations, double ClippingFactor, double *ColBegin, double *RowEnd, double *ColEnd, double *Nr, double *Nc, double *Dist) const;
  // Compute the distances of all contour points to an ellipse.
  virtual HTuple DistEllipseContourPointsXld(const Halcon::HTuple &DistanceMode, const Halcon::HTuple &ClippingEndPoints, const Halcon::HTuple &Row, const Halcon::HTuple &Column, const Halcon::HTuple &Phi, const Halcon::HTuple &Radius1, const Halcon::HTuple &Radius2) const;
  virtual HTuple DistEllipseContourPointsXld(const char *DistanceMode, Hlong ClippingEndPoints, double Row, double Column, double Phi, double Radius1, double Radius2) const;
  // Distance of contours to an ellipse.
  virtual double DistEllipseContourXld(const Halcon::HTuple &Mode, const Halcon::HTuple &MaxNumPoints, const Halcon::HTuple &ClippingEndPoints, const Halcon::HTuple &Row, const Halcon::HTuple &Column, const Halcon::HTuple &Phi, const Halcon::HTuple &Radius1, const Halcon::HTuple &Radius2, double *MaxDist, double *AvgDist, double *SigmaDist) const;
  virtual double DistEllipseContourXld(const char *Mode, Hlong MaxNumPoints, Hlong ClippingEndPoints, double Row, double Column, double Phi, double Radius1, double Radius2, double *MaxDist, double *AvgDist, double *SigmaDist) const;
  // Approximation of XLD contours by ellipses or elliptic arcs.
  virtual double FitEllipseContourXld(const Halcon::HTuple &Algorithm, const Halcon::HTuple &MaxNumPoints, const Halcon::HTuple &MaxClosureDist, const Halcon::HTuple &ClippingEndPoints, const Halcon::HTuple &VossTabSize, const Halcon::HTuple &Iterations, const Halcon::HTuple &ClippingFactor, double *Column, double *Phi, double *Radius1, double *Radius2, double *StartPhi, double *EndPhi, char *PointOrder) const;
  virtual double FitEllipseContourXld(const char *Algorithm, Hlong MaxNumPoints, double MaxClosureDist, Hlong ClippingEndPoints, Hlong VossTabSize, Hlong Iterations, double ClippingFactor, double *Column, double *Phi, double *Radius1, double *Radius2, double *StartPhi, double *EndPhi, char *PointOrder) const;
  // Creation of an XLD contour corresponding to an elliptic arc.
  static HXLDCont GenEllipseContourXld(const Halcon::HTuple &Row, const Halcon::HTuple &Column, const Halcon::HTuple &Phi, const Halcon::HTuple &Radius1, const Halcon::HTuple &Radius2, const Halcon::HTuple &StartPhi, const Halcon::HTuple &EndPhi, const Halcon::HTuple &PointOrder, const Halcon::HTuple &Resolution);
  static HXLDCont GenEllipseContourXld(double Row, double Column, double Phi, double Radius1, double Radius2, double StartPhi, double EndPhi, const char *PointOrder, double Resolution);
  // Add noise to XLD contours.
  virtual HXLDCont AddNoiseWhiteContourXld(const Halcon::HTuple &NumRegrPoints, const Halcon::HTuple &Amp) const;
  virtual HXLDCont AddNoiseWhiteContourXld(Hlong NumRegrPoints, double Amp) const;
  // Apply a projective transformation to an XLD contour.
  virtual HXLDCont ProjectiveTransContourXld(const Halcon::HTuple &HomMat2D) const;
  // Apply an arbitrary affine 2D transformation to XLD contours.
  virtual HXLDCont AffineTransContourXld(const Halcon::HTuple &HomMat2D) const;
  // Calculate the direction of an XLD contour for each contour point.
  virtual HTuple GetContourAngleXld(const Halcon::HTuple &AngleMode, const Halcon::HTuple &CalcMode, const Halcon::HTuple &Lookaround) const;
  virtual HTuple GetContourAngleXld(const char *AngleMode, const char *CalcMode, Hlong Lookaround) const;
  // Return the number of points in an XLD contour.
  virtual Hlong ContourPointNumXld(void) const;
  // Return the names of the defined global attributes of an XLD contour.
  virtual HTuple QueryContourGlobalAttribsXld(void) const;
  // Return global attributes values of an XLD contour.
  virtual HTuple GetContourGlobalAttribXld(const Halcon::HTuple &Name) const;
  virtual HTuple GetContourGlobalAttribXld(const char *Name) const;
  // Return the names of the defined attributes of an XLD contour.
  virtual HTuple QueryContourAttribsXld(void) const;
  // Return point attribute values of an XLD contour.
  virtual HTuple GetContourAttribXld(const Halcon::HTuple &Name) const;
  virtual HTuple GetContourAttribXld(const char *Name) const;
  // Return the coordinates of an XLD contour.
  virtual HTuple GetContourXld(Halcon::HTuple *Col) const;
  // Generate a XLD contour with rounded corners from a polygon (given
  // as tuples).
  static HXLDCont GenContourPolygonRoundedXld(const Halcon::HTuple &Row, const Halcon::HTuple &Col, const Halcon::HTuple &Radius, const Halcon::HTuple &SamplingInterval);
  // Generate an XLD contour from a polygon (given as tuples).
  static HXLDCont GenContourPolygonXld(const Halcon::HTuple &Row, const Halcon::HTuple &Col);
};

}

#endif
