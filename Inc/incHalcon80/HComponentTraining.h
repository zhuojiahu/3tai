/*****************************************************************************
 * HComponentTraining.h
 ***************************************************************************** 
 *
 * Project:     Halcon/C++
 * Description: Class HComponentTraining
 *
 * (c) 1996-2005 by MVTec Software GmbH
 *                  www.mvtec.com
 * 
 *****************************************************************************
 *
 * $Revision: 1.3 $
 * $Date: 2005/05/06 07:36:05 $
 *
 */

#ifndef H_COMPONENT_TRAINING_H
#define H_COMPONENT_TRAINING_H


namespace Halcon {


class LIntExport HComponentTraining: public HRootObject {
public:
  HComponentTraining(void) {id = -1;}
  HComponentTraining(
          const HImage &ModelImage, 
          const HRegionArray &InitialComponents, 
          const HImageArray &ExampleImages, 
          HRegionArray *ModelComponents, 
          const HTuple &ContrastLow,
          const HTuple &ContrastHigh,
          const HTuple &MinSize,
          const HTuple &MinScore,
          const HTuple &SearchRowTol,
          const HTuple &SearchColumnTol,
          const HTuple &SearchAngleTol,
          const HTuple &TrainingEmphasis,
          const HTuple &AmbiguityCriterion,
          const HTuple &MaxEdgeOverlap,
          const HTuple &ClusterThreshold);
  HComponentTraining(
          const HImage &ModelImage, 
          const HRegionArray &InitialComponents, 
          const HImageArray &ExampleImages, 
          HRegionArray *ModelComponents, 
          const Hlong ContrastLow,
          const Hlong ContrastHigh,
          const Hlong MinSize,
          const double MinScore,
          const Hlong SearchRowTol,
          const Hlong SearchColumnTol,
          const double SearchAngleTol,
          const char *TrainingEmphasis,
          const char *AmbiguityCriterion,
          const double MaxEdgeOverlap,
          const double ClusterThreshold);

  ~HComponentTraining(void);
private:
  // Copying and Assignment not allowed
  HComponentTraining(const HComponentTraining &inst);
  HComponentTraining &operator= (const HComponentTraining &inst);
  // Internal Cleanup
  void Cleanup(void);

public: 
  // Tools
  const char *ClassName(void) const {return "HComponentTraining";}

  // Cast Operator
  operator HTuple () const {return HTuple(GetHandle());}

  // Access and Manipulation
  Hlong GetHandle(void) const {return id;}
  void SetHandle(Hlong ID) {id = ID;}

private:
  //Handle
  Hlong  id;
public:
  virtual Hlong CreateTrainedComponentModel(const Halcon::HTuple &AngleStart, const Halcon::HTuple &AngleExtent, const Halcon::HTuple &MinContrastComp, const Halcon::HTuple &MinScoreComp, const Halcon::HTuple &NumLevelsComp, const Halcon::HTuple &AngleStepComp, const Halcon::HTuple &OptimizationComp, const Halcon::HTuple &MetricComp, const Halcon::HTuple &PregenerationComp, Halcon::HTuple *RootRanking) const;
  virtual Hlong CreateTrainedComponentModel(const Halcon::HTuple &AngleStart, const Halcon::HTuple &AngleExtent, const Halcon::HTuple &MinContrastComp, const Halcon::HTuple &MinScoreComp, const Halcon::HTuple &NumLevelsComp, const Halcon::HTuple &AngleStepComp, const Halcon::HTuple &OptimizationComp, const Halcon::HTuple &MetricComp, const Halcon::HTuple &PregenerationComp, Hlong *RootRanking) const;
  virtual Hlong CreateTrainedComponentModel(double AngleStart, double AngleExtent, Hlong MinContrastComp, double MinScoreComp, Hlong NumLevelsComp, double AngleStepComp, const char *OptimizationComp, const char *MetricComp, const char *PregenerationComp, Halcon::HTuple *RootRanking) const;
  virtual Hlong CreateTrainedComponentModel(double AngleStart, double AngleExtent, Hlong MinContrastComp, double MinScoreComp, Hlong NumLevelsComp, double AngleStepComp, const char *OptimizationComp, const char *MetricComp, const char *PregenerationComp, Hlong *RootRanking) const;
  // Return the relations between the model components that are contained in
  // a training result.
  virtual HRegionArray GetComponentRelations(const Halcon::HTuple &ReferenceComponent, const Halcon::HTuple &Image, Halcon::HTuple *Row, Halcon::HTuple *Column, Halcon::HTuple *Phi, Halcon::HTuple *Length1, Halcon::HTuple *Length2, Halcon::HTuple *AngleStart, Halcon::HTuple *AngleExtent) const;
  virtual HRegionArray GetComponentRelations(const Halcon::HTuple &ReferenceComponent, const Halcon::HTuple &Image, double *Row, double *Column, double *Phi, double *Length1, double *Length2, double *AngleStart, double *AngleExtent) const;
  // Return the initial or model components in a certain image.
  virtual HRegionArray GetTrainingComponents(const Halcon::HTuple &Components, const Halcon::HTuple &Image, const Halcon::HTuple &MarkOrientation, Halcon::HTuple *Row, Halcon::HTuple *Column, Halcon::HTuple *Angle, Halcon::HTuple *Score) const;
  virtual HRegionArray GetTrainingComponents(const Halcon::HTuple &Components, const Halcon::HTuple &Image, const Halcon::HTuple &MarkOrientation, double *Row, double *Column, double *Angle, double *Score) const;
  // Modify the relations within a training result.
  virtual void ModifyComponentRelations(const Halcon::HTuple &ReferenceComponent, const Halcon::HTuple &ToleranceComponent, const Halcon::HTuple &PositionTolerance, const Halcon::HTuple &AngleTolerance) const;
  // Read a component training result from a file.
  virtual void ReadTrainingComponents(const Halcon::HTuple &FileName);
  virtual void ReadTrainingComponents(const char *FileName);
  // Write a component training result to a file.
  virtual void WriteTrainingComponents(const Halcon::HTuple &FileName) const;
  virtual void WriteTrainingComponents(const char *FileName) const;
  // Adopt new parameters that are used to create the model components into the 
  // training result.
  virtual HRegionArray ClusterModelComponents(const HImageArray &TrainingImages, const Halcon::HTuple &AmbiguityCriterion, const Halcon::HTuple &MaxContourOverlap, const Halcon::HTuple &ClusterThreshold) const;
  virtual HRegionArray ClusterModelComponents(const HImageArray &TrainingImages, const char *AmbiguityCriterion, double MaxContourOverlap, double ClusterThreshold) const;
  // Inspect the rigid model components obtained from the training.
  virtual HRegionArray InspectClusteredComponents(const Halcon::HTuple &AmbiguityCriterion, const Halcon::HTuple &MaxContourOverlap, const Halcon::HTuple &ClusterThreshold) const;
  virtual HRegionArray InspectClusteredComponents(const char *AmbiguityCriterion, double MaxContourOverlap, double ClusterThreshold) const;
  // Train components and relations for the component-based matching.
  virtual HRegionArray TrainModelComponents(const HImage &ModelImage, const HRegionArray &InitialComponents, const HImageArray &TrainingImages, const Halcon::HTuple &ContrastLow, const Halcon::HTuple &ContrastHigh, const Halcon::HTuple &MinSize, const Halcon::HTuple &MinScore, const Halcon::HTuple &SearchRowTol, const Halcon::HTuple &SearchColumnTol, const Halcon::HTuple &SearchAngleTol, const Halcon::HTuple &TrainingEmphasis, const Halcon::HTuple &AmbiguityCriterion, const Halcon::HTuple &MaxContourOverlap, const Halcon::HTuple &ClusterThreshold);
  virtual HRegionArray TrainModelComponents(const HImage &ModelImage, const HRegionArray &InitialComponents, const HImageArray &TrainingImages, Hlong ContrastLow, Hlong ContrastHigh, Hlong MinSize, double MinScore, Hlong SearchRowTol, Hlong SearchColumnTol, double SearchAngleTol, const char *TrainingEmphasis, const char *AmbiguityCriterion, double MaxContourOverlap, double ClusterThreshold);
};

}

#endif
