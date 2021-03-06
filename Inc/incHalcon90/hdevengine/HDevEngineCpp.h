/*****************************************************************************
 * HDevEngineCpp.h
 *****************************************************************************
 *
 * Description: Classes for execution of HDevelop programs and procedures
 *              using the HALCON/CPP interface
 *
 * (c) 1996-2008 by MVTec Software GmbH
 *                  www.mvtec.com
 * 
 *****************************************************************************
 */

#ifndef H_DEV_ENGINE_CPP_H
#define H_DEV_ENGINE_CPP_H

#include "HalconCpp.h"

#ifndef HDEV_PD
#define HDEV_PD private: class Data; Data* mData
#endif

namespace HDevEngineCpp
{

// Forward declarations
class HDevEngine;
class HDevProgram;
class HDevProgramCall;
class HDevProcedure;
class HDevProcedureCall;
class HDevEngineException;
class HDevOperatorImplCpp;


/*****************************************************************************
 *****************************************************************************
 ** class HDevEngine
 **===========================================================================
 ** Class for managing global engine settings:
 **  + external procedure path
 **  + implementation of dev_ operators (HDevOperatorImpl)
 **  + Attention: all changes made to one HDevEngine instance all global
 **    for all .dev programs or .dvp procedure that are executed in one 
 **    application
 *****************************************************************************
 *****************************************************************************/
class LIntExport HDevEngine
{
public:

  HDevEngine();

  // Set path(s) for external procedures
  //  - several paths can be passed together separating them by ';' or ':'
  //    on Windows or UNIX systems resp.
  //  - NULL removes all procedure paths and unloads all external procedures
  //    (Attention: procedures that are used by programs (HDevProgram) or
  //    procedures (HDevProcedures) remain unchanged until the program or
  //    procedure is reloaded explicitely.  The appropriate calls must be
  //    recreated or reassigned by the reloaded program or procedure.)
  //  - additional calls of SetProcedurePath will remove paths set before
  //    and unload all external procedures
  void SetProcedurePath(const char* path);
  void AddProcedurePath(const char* path);
  // Get names of all available external procedures
  Halcon::HTuple GetProcedureNames() const;
  // Get names of all loaded external procedures
  Halcon::HTuple GetLoadedProcedureNames() const;
  // Unload a specific procedure <proc_name>
  void UnloadProcedure(const char* proc_name);
  // Unload all external procedures
  void UnloadAllProcedures();

  // Set implementation for HDevelop internal operators
  void SetHDevOperatorImpl(HDevOperatorImplCpp* hdev_op_impl, 
                           bool mem_free_intern=true);
};


/*****************************************************************************
 *****************************************************************************
 ** class HDevProgram
 **===========================================================================
 ** Class for managing HDevelop programs
 *****************************************************************************
 *****************************************************************************/
class LIntExport HDevProgram
{
  HDEV_PD;

public:

  // Create a program from a .dev program file
  HDevProgram(const char* file_name=NULL);

  // Copy constructor
  HDevProgram(const HDevProgram& hdev_prog);
  HDevProgram(const Data& data);

  // Assignment operation
  HDevProgram& operator=(const HDevProgram& hdev_prog);

  // Destructor
  virtual ~HDevProgram();

  // Load a program if not yet done during construction
  void            LoadProgram(const char* file_name);

  // check whether the program was successfully loaded
  bool            IsLoaded() const;

  // Get the program name
  const char*     GetName();

  // create a program call for execution
  HDevProgramCall CreateCall() const;

  // This is a method provided for convenience:
  // execute the program and return the program call for
  // accessing the variables of the program's main procedure
  HDevProgramCall Execute() const;

  // get some information about the variables of the program's main procedure:
  //  - get the variable names as a tuple
  Halcon::HTuple  GetIconicVarNames() const;
  Halcon::HTuple  GetCtrlVarNames()   const;

  //  - get the number of iconic and control variables
  size_t          GetIconicVarCount() const;
  size_t          GetCtrlVarCount()   const;

  //  - get the names of the variables
  //  TODO: (indices of the variables run from 1 to count)
  const char*     GetIconicVarName(size_t var_idx) const;
  const char*     GetCtrlVarName(size_t var_idx)   const;
};


/*****************************************************************************
 *****************************************************************************
 ** class HDevProgramCall
 **===========================================================================
 ** Class for managing the execution of an HDevelop program
 *****************************************************************************
 *****************************************************************************/
class LIntExport HDevProgramCall
{
  HDEV_PD;

public:

  // Create an empty HDevelop program call instance
  HDevProgramCall();
  // Create an HDevelop program call from a program
  HDevProgramCall(const HDevProgram& prog);
  // Copy constructor
  HDevProgramCall(const HDevProgramCall& hdev_prog_call);
  HDevProgramCall(const Data& data);
  // Assignment operation
  HDevProgramCall& operator=(const HDevProgramCall& hdev_prog_call);
  // Destructor
  virtual ~HDevProgramCall();

  // Get the program
  HDevProgram GetProgram() const;

  // Execute program
  void Execute();

  // Clear program and reset callstack
  //  - this method stops the execution of the program after the current
  //    program line
  void Reset();

  // Get the objects / values of the variables by name or by index
  //   (indices of the variables run from 1 to count)
  Halcon::Hobject GetIconicVarObject(size_t var_idx);
  Halcon::Hobject GetIconicVarObject(const char* var_name);

  Halcon::HTuple  GetCtrlVarTuple(size_t var_idx);
  Halcon::HTuple  GetCtrlVarTuple(const char* var_name);

  // these methods are provided for efficiency:
  // the results are copied directly into the tuple variable provided by
  // the user without additional copying 
  void GetCtrlVarTuple(size_t var_idx, Halcon::HTuple* tuple);
  void GetCtrlVarTuple(const char* var_name, Halcon::HTuple* tuple);
};




/*****************************************************************************
 *****************************************************************************
 ** class HDevProcedure
 **===========================================================================
 ** Class for managing HDevelop procedures
 *****************************************************************************
 *****************************************************************************/
class  LIntExport HDevProcedure
{
  HDEV_PD;

public:

  // Create HDevelop procedure from external or local procedure
  HDevProcedure(const char* proc_name=NULL);
  HDevProcedure(const char* prog_name, const char* proc_name);
  HDevProcedure(const HDevProgram& prog, const char* proc_name);

  // Copy constructor
  HDevProcedure(const HDevProcedure& hdev_proc);
  HDevProcedure(const Data& data);
  // Assignment operation
  HDevProcedure& operator=(const HDevProcedure& proc);
  // Destructor
  ~HDevProcedure();

  // Load a procedure if not yet done during construction
  void         LoadProcedure(const char* file_name);
  void         LoadProcedure(const char* prog_name, const char* proc_name);
  void         LoadProcedure(const HDevProgram& prog, const char* proc_name);

  // Check whether the procedure was successfully loaded
  bool         IsLoaded() const;

  // Get the name of the procedure
  const char*  GetName() const;

  // Get the short description of the procedure
  const char*  GetShortDescription() const;

  // Create a program call for execution
  HDevProcedureCall CreateCall() const;

  // Get name of input/output object/control parameters
  Halcon::HTuple GetInputIconicParamNames()  const;
  Halcon::HTuple GetOutputIconicParamNames() const;
  Halcon::HTuple GetInputCtrlParamNames()    const;
  Halcon::HTuple GetOutputCtrlParamNames()   const;

  // Get number of input/output object/control parameters
  int            GetInputIconicParamCount()  const;
  int            GetOutputIconicParamCount() const;
  int            GetInputCtrlParamCount()    const;
  int            GetOutputCtrlParamCount()   const;

  // Get name of input/output object/control parameters
  const char*    GetInputIconicParamName(int par_idx)  const;
  const char*    GetOutputIconicParamName(int par_idx) const;
  const char*    GetInputCtrlParamName(int par_idx)    const;
  const char*    GetOutputCtrlParamName(int par_idx)   const;
};




/*****************************************************************************
 *****************************************************************************
 ** class HDevProcedureCall
 **===========================================================================
 ** Class for executing an HDevelop procedure and managing the parameter
 ** values
 *****************************************************************************
 *****************************************************************************/
class  LIntExport HDevProcedureCall
{
  HDEV_PD;

public:

  // Create an empty HDevelop procedure call instance
  HDevProcedureCall();
  // Create HDevelop procedure call instance
  HDevProcedureCall(const HDevProcedure& hdev_proc);
  // Copy constructor
  HDevProcedureCall(const HDevProcedureCall& hdev_proc_call);
  HDevProcedureCall(const Data& data);
  // Assignment operation
  HDevProcedureCall& operator=(const HDevProcedureCall& hdev_proc_call);
  // Destructor
  ~HDevProcedureCall();

  // Get the procedure
  HDevProcedure GetProcedure() const;

  // Execute program
  void Execute();

  // Clear program and reset callstack
  //  - this method stops the execution of the program after the current
  //    program line
  void Reset();

  // Set input object/control parameter
  void SetInputIconicParamObject(int par_idx, const Halcon::Hobject& obj);
  void SetInputIconicParamObject(int par_idx, const Halcon::HObject& obj);
  void SetInputIconicParamObject(int par_idx, const Halcon::HObjectArray& obj);
  void SetInputIconicParamObject(const char* par_name,
                                 const Halcon::Hobject& obj);
  void SetInputIconicParamObject(const char* par_name,
                                 const Halcon::HObject& obj);
  void SetInputIconicParamObject(const char* par_name,
                                 const Halcon::HObjectArray& obj);
  void SetInputCtrlParamTuple(int par_idx, const Halcon::HTuple& tuple);
  void SetInputCtrlParamTuple(const char* par_name,
                              const Halcon::HTuple& tuple);

  // Get the objects / values of the parameters by name or by index
  //   (indices of the variables run from 1 to count)
  Halcon::Hobject GetOutputIconicParamObject(int par_idx)          const;
  Halcon::Hobject GetOutputIconicParamObject(const char* par_name) const;
  Halcon::HTuple  GetOutputCtrlParamTuple(int par_idx)             const;
  Halcon::HTuple  GetOutputCtrlParamTuple(const char* par_name)    const;

  // These methods are provided for efficiency:
  // the results are copied directly into the tuple variable provided by
  // the user without additional copying 
  void GetOutputCtrlParamTuple(int par_idx, Halcon::HTuple* tuple) const;
  void GetOutputCtrlParamTuple(const char* par_name,
                               Halcon::HTuple* tuple)              const;
};




/*****************************************************************************
 *****************************************************************************
 ** class HDevEngineException
 **===========================================================================
 ** Class for HDevelop engine exceptions
 *****************************************************************************
 *****************************************************************************/
class LIntExport HDevEngineException
{
  HDEV_PD;

public:

  // Exception categories
  enum  ExceptionCategory
  {
    Exception,            // Generic
    ExceptionInpNotInit,  // Error input parameters not initialized
    ExceptionCall,        // Error HALCON or HDevelop operator call
    ExceptionFile         // Error opening or reading HDevelop file
  };

  // Create HDevelop engine exception
  HDevEngineException(const char* message,
                      ExceptionCategory category=Exception,
                      const char* exec_proc_name="",
                      int prog_line_num=-1,
                      const char* prog_line_name="",
                      Herror h_err_nr=H_MSG_VOID); 
  HDevEngineException(const HDevEngineException& exc);
  HDevEngineException(const Data& data);
  HDevEngineException& operator = (const HDevEngineException& exc);
  virtual ~HDevEngineException();

  // Error text
  const char*       Message()           const;
  // Category of exception
  ExceptionCategory Category()          const;
  const char*       CategoryText()      const;
  // Name of executed procedure
  const char*       ExecProcedureName() const;
  // Number of executed procedure or operator program line
  int               ProgLineNum()       const;
  // Name of executed procedure or operator program line 
  const char*       ProgLineName()      const;
  // HALCON error number
  Herror            HalconErrNum()      const;

};



/*****************************************************************************
 *****************************************************************************
 ** class HDevOperatorImplCpp
 **===========================================================================
 ** Class for the implemention of HDevelop internal operators
 *****************************************************************************
 *****************************************************************************/
class LIntExport HDevOperatorImplCpp
{
  HDEV_PD;

public:

  HDevOperatorImplCpp();
  // Copy constructor
  HDevOperatorImplCpp(const HDevOperatorImplCpp& hdev_op_impl);
  HDevOperatorImplCpp(const Data& data);
  // Assignment operation
  HDevOperatorImplCpp& operator=(const HDevOperatorImplCpp& hdev_op_impl);
  // Destructor
  virtual ~HDevOperatorImplCpp();

  virtual int DevClearWindow();
  virtual int DevCloseWindow();
  virtual int DevSetWindow(const Halcon::HTuple& win_id);
  virtual int DevDisplay(const Halcon::Hobject& obj);
  virtual int DevSetWindowExtents(const Halcon::HTuple& row,
                                  const Halcon::HTuple& col,
                                  const Halcon::HTuple& width,
                                  const Halcon::HTuple& height);
  virtual int DevSetDraw(const Halcon::HTuple& draw);
  virtual int DevSetShape(const Halcon::HTuple& shape);
  virtual int DevSetColored(const Halcon::HTuple& colored);
  virtual int DevSetColor(const Halcon::HTuple& color);
  virtual int DevSetLut(const Halcon::HTuple& lut);
  virtual int DevSetPaint(const Halcon::HTuple& paint);
  virtual int DevSetPart(const Halcon::HTuple& row1,
                         const Halcon::HTuple& col1,
                         const Halcon::HTuple& row2,
                         const Halcon::HTuple& col2);
  virtual int DevSetLineWidth(const Halcon::HTuple& width);
  virtual int DevOpenWindow(const Halcon::HTuple& row,
                            const Halcon::HTuple& col,
                            const Halcon::HTuple& width,
                            const Halcon::HTuple& height,
                            const Halcon::HTuple& background,
                            Halcon::HTuple* win_id);
};

}; // namespace HDevEngineCpp


#endif // #ifndef H_DEV_ENGINE_CPP_H
