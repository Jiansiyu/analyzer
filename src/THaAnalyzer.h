#ifndef ROOT_THaAnalyzer
#define ROOT_THaAnalyzer

//////////////////////////////////////////////////////////////////////////
//
// THaAnalyzer
// 
// THaAnalyzer is the base class for a "Hall A analyzer" class.
// An analyzer defines the basic actions to perform during analysis.
// THaAnalyzer is the default analyzer that is used if no user class is
// defined.  It performs a standard analysis consisting of
//
//   1. Decoding/Calibrating
//   2. Track Reconstruction
//   3. Physics variable processing
//
// At the end of each step, testing and histogramming are done for
// the appropriate block defined in the global test/histogram lists.
//
//////////////////////////////////////////////////////////////////////////

#include "TString.h"

class THaEvent;
class THaRun;
class THaOutput;
class TList;
class TFile;
class TIter;
class TDatime;
class THaCut;

class THaAnalyzer {

public:
  THaAnalyzer( );
  virtual ~THaAnalyzer();

  virtual const char*    GetOutFileName()    const { return fOutFileName.Data(); }
  virtual const char*    GetCutFileName()    const { return fCutFileName.Data(); }
  virtual const char*    GetOdefFileName()   const { return fOdefFileName.Data(); }
  virtual const TFile*   GetOutFile()        const { return fFile; }
  virtual void           SetEvent( THaEvent* event )     { fEvent = event; }
  virtual void           SetOutFile( const char* name )  { fOutFileName = name; }
  virtual void           SetCutFile( const char* name )  { fCutFileName = name; }
  virtual void           SetOdefFile( const char* name ) { fOdefFileName = name; }

  virtual void           Close();
  virtual Int_t          Process( THaRun& run );
  
protected:
  static const char* const kMasterCutName;

  enum EStage      { kRawDecode, kDecode, kCoarseRecon, kReconstruct, kPhysics,
		     kMaxStage };
  enum ESkipReason { kEvFileTrunc, kCodaErr, kRawDecodeTest, kDecodeTest, 
		     kCoarseReconTest, kReconstructTest, kPhysicsTest, 
		     kMaxSkip };
  // Statistics counters and message texts
  struct Skip_t {
    const char* reason;
    Int_t       count;
  };
  // Test and histogram blocks
  struct Stage_t {
    const char*   name;
    ESkipReason   skipkey;
    TList*        cut_list;
    TList*        hist_list;
    THaCut*       master_cut;
  };
    
  TFile*         fFile;            //The ROOT output file.
  THaOutput*     fOutput;          //Flexible ROOT output (tree, histograms)
  TString        fOutFileName;     //Name of output ROOT file.
  TString        fCutFileName;     //Name of cut definition file to load
  TString        fOdefFileName;    //Name of output definition file
  THaEvent*      fEvent;           //The event structure to be written to file.
  Stage_t*       fStages;          //Cuts/histograms for each analysis stage [kMaxStage]
  UInt_t         fNev;             //Current event number
  Skip_t*        fSkipCnt;         //Counters for reasons to skip events [kMaxSkip]

  virtual bool   EvalStage( EStage n );
  virtual void   InitCuts();
  virtual Int_t  InitModules( TIter& iter, TDatime& time, Int_t erroff,
			      const char* baseclass = NULL );

  ClassDef(THaAnalyzer,0)  //Hall A Analyzer Standard Event Loop
};


#endif






