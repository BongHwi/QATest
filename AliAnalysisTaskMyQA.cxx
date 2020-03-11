/**************************************************************************
 * Copyright(c) 1998-1999, ALICE Experiment at CERN, All rights reserved. *
 *                                                                        *
 * Author: The ALICE Off-line Project.                                    *
 * Contributors are mentioned in the code where appropriate.              *
 *                                                                        *
 * Permission to use, copy, modify and distribute this software and its   *
 * documentation strictly for non-commercial purposes is hereby granted   *
 * without fee, provided that the above copyright notice appears in all   *
 * copies and that both the copyright notice and this permission notice   *
 * appear in the supporting documentation. The authors make no claims     *
 * about the suitability of this software for any purpose. It is          *
 * provided "as is" without express or implied warranty.                  *
 **************************************************************************/

/* AliAnaysisTaskMyTask
 *
 *
 *
 */

#include "AliAnalysisTaskMyQA.h"

#include <math.h>
#include "TMath.h"

#include <iostream>

#include "AliAODEvent.h"
#include "AliAODHandler.h"
#include "AliAODMCHeader.h"
#include "AliAODMCParticle.h"
#include "AliAODTrack.h"
#include "AliAnalysisManager.h"
#include "AliAnalysisTask.h"
#include "AliESDEvent.h"
#include "AliESDtrack.h"
#include "AliESDtrackCuts.h"
#include "AliEventCuts.h"
#include "AliMCEvent.h"
#include "AliMCEventHandler.h"
#include "AliMultSelectionTask.h"
#include "AliPIDResponse.h"
#include "AliStack.h"
#include "TChain.h"
#include "THistManager.h"

enum {
  kXiCode = 3312,          // Xi-
  kXiZeroCode = 3322,      // Xi0
  kLambdaCode = 3122,      // Lambda
  kProtonCode = 2212,      // Proton+
  kPionCode = 211,         // Pion+
  kXiStarCode = 3324,      // Xi(1530)0
  kXiStarMCode = 3314,     // Xi(1530)-
  kLambdaStarCode = 3124,  // Lambda1520
  kSigmaZeroCode = 3212    // Sigma0
};
class AliAnalysisTaskMyQA;

using namespace std;

ClassImp(AliAnalysisTaskMyQA)

    AliAnalysisTaskMyQA::AliAnalysisTaskMyQA()
    : AliAnalysisTaskSE(),
      fEvt(0),
      fHistos(nullptr),
      fMCArray(nullptr),
      fIsPrimaryMC(false) {}
//_____________________________________________________________________________
AliAnalysisTaskMyQA::AliAnalysisTaskMyQA(const char* name)
    : AliAnalysisTaskSE(name),
      fEvt(0),
      fHistos(nullptr),
      fMCArray(nullptr),
      fIsPrimaryMC(false) {
  // constructor
  DefineInput(0, TChain::Class());
  DefineOutput(1, TList::Class());
}
//_____________________________________________________________________________
AliAnalysisTaskMyQA::~AliAnalysisTaskMyQA() {}
//_____________________________________________________________________________
void AliAnalysisTaskMyQA::UserCreateOutputObjects() {
  fHistos = new THistManager("Sigma1385hists");
  fEventCuts.AddQAplotsToList(fHistos->GetListOfHistograms());

  fHistos->CreateTH1("hCheck", "", 5, -1.5, 3.5, "s");

  PostData(1, fHistos->GetListOfHistograms());
}
//_____________________________________________________________________________
void AliAnalysisTaskMyQA::UserExec(Option_t*) {
  AliVEvent* event = InputEvent();
  if (!event) {
    PostData(1, fHistos->GetListOfHistograms());
    AliInfo("Could not retrieve event");
    return;
  }

  event->IsA() == AliESDEvent::Class()
      ? fEvt = dynamic_cast<AliESDEvent*>(event)
      : fEvt = dynamic_cast<AliAODEvent*>(event);
  if (!fEvt) {
    PostData(1, fHistos->GetListOfHistograms());
    return;
  }
  bool IsEvtSelected = fEventCuts.AcceptEvent(event);

  if (!IsEvtSelected) {
    PostData(1, fHistos->GetListOfHistograms());
    return;
  }
  fMCArray = (TClonesArray*)fEvt->FindListObject("mcparticles");  // AOD Case

  for (Int_t it = 0; it < fMCArray->GetEntriesFast(); it++) {
    AliAODMCParticle* mcInputTrack = (AliAODMCParticle*)fMCArray->At(it);
    if (!mcInputTrack) {
      Error("UserExec", "Could not receive MC track %d", it);
      continue;
    }
    // fHistos->FillTH1("hCheck", -1);

    Int_t v0PdgCode = mcInputTrack->GetPdgCode();

    if (TMath::Abs(v0PdgCode) != kXiStarCode)
      continue;
    if (fIsPrimaryMC && !mcInputTrack->IsPrimary())
      continue;

    // Y cut
    if ((mcInputTrack->Y() > 0.5) ||
        (mcInputTrack->Y() < -0.5))
      continue;
    
    fHistos->FillTH1("hCheck", 0.);

    auto mcPart1 = (AliAODMCParticle*)fMCArray->At(
        abs(mcInputTrack->GetDaughterFirst()));
    auto mcPart2 = (AliAODMCParticle*)fMCArray->At(
        abs(mcInputTrack->GetDaughterLast()));
    
    // std::cout << "Check the daughters: " << TMath::Abs(mcPart1->GetPdgCode()) << ", " << TMath::Abs(mcPart2->GetPdgCode()) << std::endl;

    if (TMath::Abs(mcPart1->GetPdgCode()) == kXiCode)
        fHistos->FillTH1("hCheck", 1);
    if (TMath::Abs(mcPart1->GetPdgCode()) == kXiZeroCode)
        fHistos->FillTH1("hCheck", 2);
    if (TMath::Abs(mcPart2->GetPdgCode()) == kXiCode)
        fHistos->FillTH1("hCheck", 1);
    if (TMath::Abs(mcPart2->GetPdgCode()) == kXiZeroCode)
        fHistos->FillTH1("hCheck", 2);
  }

  PostData(1, fHistos->GetListOfHistograms());
}
//_____________________________________________________________________________
void AliAnalysisTaskMyQA::Terminate(Option_t*) {}
//_____________________________________________________________________________
