/* Copyright(c) 1998-1999, ALICE Experiment at CERN, All rights reserved. */
/* See cxx source for full Copyright notice */
/* $Id$ */

#ifndef AliAnalysisTaskMyQA_H
#define AliAnalysisTaskMyQA_H

#include "AliAnalysisTaskSE.h"
#include "AliEventCuts.h"
class THistManager;

class AliAnalysisTaskMyQA : public AliAnalysisTaskSE {
 public:
  AliAnalysisTaskMyQA();
  AliAnalysisTaskMyQA(const char* name);
  virtual ~AliAnalysisTaskMyQA();

  virtual void UserCreateOutputObjects();
  virtual void UserExec(Option_t* option);
  virtual void Terminate(Option_t* option);

  AliEventCuts fEventCuts;  // Event cuts
 private:
  TList* fOutputList;  //! output list
  THistManager* fHistos;
  TClonesArray* fMCArray;  //!
  AliVEvent* fEvt;             //!

  bool fIsPrimaryMC;

  AliAnalysisTaskMyQA(const AliAnalysisTaskMyQA&);  // not implemented
  AliAnalysisTaskMyQA& operator=(
      const AliAnalysisTaskMyQA&);  // not implemented

  ClassDef(AliAnalysisTaskMyQA, 1);
};

#endif
