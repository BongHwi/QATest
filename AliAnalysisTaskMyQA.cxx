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


#include <iostream>
#include <math.h>

#include "TChain.h"
#include "TH1F.h"
#include "TList.h"
#include "AliAnalysisTask.h"
#include "AliAnalysisManager.h"
#include "AliAnalysisUtils.h"

#include "AliESDEvent.h"
#include "AliESDInputHandler.h"

#include "AliMCEventHandler.h"
#include "AliMCEvent.h"
#include "AliMCParticle.h"
#include "AliStack.h"

#include "AliAnalysisTaskMyQA.h"
#include "AliMultSelection.h"
#include "AliMultiplicity.h"
#include "AliESDtrackCuts.h"

class AliAnalysisTaskMyQA;

using namespace std;

ClassImp(AliAnalysisTaskMyQA)

AliAnalysisTaskMyQA::AliAnalysisTaskMyQA() : AliAnalysisTaskSE(), 
    fESD(0), 
    fOutputList(0), 
    htotalEvent(0), 
    htriggered_INELg0_tracklet(0),
    htriggered_CINT7_tracklet(0),
    htriggered_CINT7_VOM(0),
    htriggered_AliMult_VOM(0),
    fMultDist(0),
    fMultDist_pp(0)
{

}
//_____________________________________________________________________________
AliAnalysisTaskMyQA::AliAnalysisTaskMyQA(const char* name) : AliAnalysisTaskSE(name),
    fESD(0),
    fOutputList(0), 
    htotalEvent(0), 
    htriggered_INELg0_tracklet(0),
    htriggered_CINT7_tracklet(0),
    htriggered_CINT7_VOM(0),
    htriggered_AliMult_VOM(0),
    fMultDist(0),
    fMultDist_pp(0)
{
    // constructor
    DefineInput(0, TChain::Class());
    DefineOutput(1, TList::Class());
}
//_____________________________________________________________________________
AliAnalysisTaskMyQA::~AliAnalysisTaskMyQA()
{
    // destructor
    if(fOutputList) {
        delete fOutputList;
    }
}
//_____________________________________________________________________________
void AliAnalysisTaskMyQA::UserCreateOutputObjects()
{
    fOutputList = new TList();
    fOutputList->SetOwner(kTRUE);

    htotalEvent = new TH1F("htotalEvent","Number of Events",10,0,10);
    htotalEvent->GetXaxis()->SetBinLabel(1,"All Events");
    htotalEvent->GetXaxis()->SetBinLabel(2,"IsINELg0");
    htotalEvent->GetXaxis()->SetBinLabel(3,"tracklet in |Eta|<1");
    htotalEvent->GetXaxis()->SetBinLabel(4,"CINT7 triggered");
    htotalEvent->GetXaxis()->SetBinLabel(5,"AliMultiSelection");
    fOutputList->Add(htotalEvent);

    htriggered_INELg0_tracklet = new TH1F("htriggered_INELg0_tracklet","Triggered event in INEL>0",11,0,11);
    htriggered_INELg0_tracklet->GetXaxis()->SetBinLabel(1,"0 to Inf (MB)");
    htriggered_INELg0_tracklet->GetXaxis()->SetBinLabel(2,"0 to 5");
    htriggered_INELg0_tracklet->GetXaxis()->SetBinLabel(3,"6 to 10");
    htriggered_INELg0_tracklet->GetXaxis()->SetBinLabel(4,"11 to 15");
    htriggered_INELg0_tracklet->GetXaxis()->SetBinLabel(5,"16 to 20");
    htriggered_INELg0_tracklet->GetXaxis()->SetBinLabel(6,"21 to 25");
    htriggered_INELg0_tracklet->GetXaxis()->SetBinLabel(7,"26 to 30");
    htriggered_INELg0_tracklet->GetXaxis()->SetBinLabel(8,"31 to 35");
    htriggered_INELg0_tracklet->GetXaxis()->SetBinLabel(9,"36 to 40");
    htriggered_INELg0_tracklet->GetXaxis()->SetBinLabel(10,"41 to 50");
    htriggered_INELg0_tracklet->GetXaxis()->SetBinLabel(11,"51 more");
    fOutputList->Add(htriggered_INELg0_tracklet);

    /*
    htriggered_INELg0_VOM = new TH1F("htriggered_INELg0_VOM","Triggered event in INEL>0",11,0,11);
    htriggered_INELg0_VOM->GetXaxis()->SetBinLabel(1,"0.0 - 100.0% (MB)");
    htriggered_INELg0_VOM->GetXaxis()->SetBinLabel(2,"70.0 - 100.0%");
    htriggered_INELg0_VOM->GetXaxis()->SetBinLabel(3,"50.0 - 70.0%");
    htriggered_INELg0_VOM->GetXaxis()->SetBinLabel(4,"40.0 - 50.0%");
    htriggered_INELg0_VOM->GetXaxis()->SetBinLabel(5,"30.0 - 40.0%");
    htriggered_INELg0_VOM->GetXaxis()->SetBinLabel(6,"20.0 - 30.0%");
    htriggered_INELg0_VOM->GetXaxis()->SetBinLabel(7,"15.0 - 20.0%");
    htriggered_INELg0_VOM->GetXaxis()->SetBinLabel(8,"10.0 - 15.0%");
    htriggered_INELg0_VOM->GetXaxis()->SetBinLabel(9,"5.0 - 10.0%");
    htriggered_INELg0_VOM->GetXaxis()->SetBinLabel(10,"1.0 - 5.0%");
    htriggered_INELg0_VOM->GetXaxis()->SetBinLabel(11,"0.0 - 1.0%");
    fOutputList->Add(htriggered_INELg0_VOM);
    */

    htriggered_CINT7_tracklet = new TH1F("htriggered_CINT7_tracklet","Triggered event in INEL>0",11,0,11);
    htriggered_CINT7_tracklet->GetXaxis()->SetBinLabel(1,"0 to Inf (MB)");
    htriggered_CINT7_tracklet->GetXaxis()->SetBinLabel(2,"0 to 5");
    htriggered_CINT7_tracklet->GetXaxis()->SetBinLabel(3,"6 to 10");
    htriggered_CINT7_tracklet->GetXaxis()->SetBinLabel(4,"11 to 15");
    htriggered_CINT7_tracklet->GetXaxis()->SetBinLabel(5,"16 to 20");
    htriggered_CINT7_tracklet->GetXaxis()->SetBinLabel(6,"21 to 25");
    htriggered_CINT7_tracklet->GetXaxis()->SetBinLabel(7,"26 to 30");
    htriggered_CINT7_tracklet->GetXaxis()->SetBinLabel(8,"31 to 35");
    htriggered_CINT7_tracklet->GetXaxis()->SetBinLabel(9,"36 to 40");
    htriggered_CINT7_tracklet->GetXaxis()->SetBinLabel(10,"41 to 50");
    htriggered_CINT7_tracklet->GetXaxis()->SetBinLabel(11,"51 more");
    fOutputList->Add(htriggered_CINT7_tracklet);

    htriggered_CINT7_VOM = new TH1F("htriggered_CINT7_VOM","Triggered event in INEL>0",11,0,11);
    htriggered_CINT7_VOM->GetXaxis()->SetBinLabel(1,"0.0 - 100.0% (MB)");
    htriggered_CINT7_VOM->GetXaxis()->SetBinLabel(2,"70.0 - 100.0%");
    htriggered_CINT7_VOM->GetXaxis()->SetBinLabel(3,"50.0 - 70.0%");
    htriggered_CINT7_VOM->GetXaxis()->SetBinLabel(4,"40.0 - 50.0%");
    htriggered_CINT7_VOM->GetXaxis()->SetBinLabel(5,"30.0 - 40.0%");
    htriggered_CINT7_VOM->GetXaxis()->SetBinLabel(6,"20.0 - 30.0%");
    htriggered_CINT7_VOM->GetXaxis()->SetBinLabel(7,"15.0 - 20.0%");
    htriggered_CINT7_VOM->GetXaxis()->SetBinLabel(8,"10.0 - 15.0%");
    htriggered_CINT7_VOM->GetXaxis()->SetBinLabel(9,"5.0 - 10.0%");
    htriggered_CINT7_VOM->GetXaxis()->SetBinLabel(10,"1.0 - 5.0%");
    htriggered_CINT7_VOM->GetXaxis()->SetBinLabel(11,"0.0 - 1.0%");
    fOutputList->Add(htriggered_CINT7_VOM);

    htriggered_AliMult_VOM = new TH1F("htriggered_AliMult_VOM","Triggered event in INEL>0",11,0,11);
    htriggered_AliMult_VOM->GetXaxis()->SetBinLabel(1,"0.0 - 100.0% (MB)");
    htriggered_AliMult_VOM->GetXaxis()->SetBinLabel(2,"70.0 - 100.0%");
    htriggered_AliMult_VOM->GetXaxis()->SetBinLabel(3,"50.0 - 70.0%");
    htriggered_AliMult_VOM->GetXaxis()->SetBinLabel(4,"40.0 - 50.0%");
    htriggered_AliMult_VOM->GetXaxis()->SetBinLabel(5,"30.0 - 40.0%");
    htriggered_AliMult_VOM->GetXaxis()->SetBinLabel(6,"20.0 - 30.0%");
    htriggered_AliMult_VOM->GetXaxis()->SetBinLabel(7,"15.0 - 20.0%");
    htriggered_AliMult_VOM->GetXaxis()->SetBinLabel(8,"10.0 - 15.0%");
    htriggered_AliMult_VOM->GetXaxis()->SetBinLabel(9,"5.0 - 10.0%");
    htriggered_AliMult_VOM->GetXaxis()->SetBinLabel(10,"1.0 - 5.0%");
    htriggered_AliMult_VOM->GetXaxis()->SetBinLabel(11,"0.0 - 1.0%");
    fOutputList->Add(htriggered_AliMult_VOM);

    fMultDist = new TH1F("fMultDist","Multiplicity Distribution of PP",200,0,200);
    fMultDist->GetXaxis()->SetTitle("Multiplicity Percentile");
    fOutputList->Add(fMultDist);

    fMultDist_pp = new TH1F("fMultDist_pp","Multiplicity Distribution of PP",200,0,200);
    fMultDist_pp->GetXaxis()->SetTitle("Multiplicity Percentile");
    fOutputList->Add(fMultDist_pp);

    PostData(1, fOutputList);
}
//_____________________________________________________________________________
void AliAnalysisTaskMyQA::UserExec(Option_t *)
{
    //std::cout << "Event!" << std::endl;
    fESD = dynamic_cast<AliESDEvent*> (InputEvent());
    if(!fESD) return;

    AliMCEvent  *mcEvent        = 0x0;
    AliVEventHandler* eventHandler = AliAnalysisManager::GetAnalysisManager()->GetMCtruthEventHandler();
    if(eventHandler){
        AliMCEventHandler* mcEventHandler = dynamic_cast<AliMCEventHandler*>(eventHandler);
        if(mcEventHandler) mcEvent = static_cast<AliMCEventHandler*>(AliAnalysisManager::GetAnalysisManager()->GetMCtruthEventHandler())->MCEvent();
    }
    if(!mcEvent) return;
    
    AliStack*    mcstack = mcEvent->Stack();
    if(!mcstack) return;

    htotalEvent->Fill(0); // Total N of event

    Bool_t IsINELg0 = false;
    for (Int_t it = 0; it < mcstack->GetNprimary(); it++) {
        AliMCParticle *mcInputTrack = (AliMCParticle*)mcEvent->GetTrack(it);
        if (mcInputTrack->IsPhysicalPrimary() && TMath::Abs(mcInputTrack->Charge()) && TMath::Abs(mcInputTrack->Eta())<1 ) IsINELg0 = true;
    }

    // INEL > 0
    if(IsINELg0){
        //std::cout << "It's INELg0 event!" << std::endl;
        htotalEvent->Fill(1); // Total N of INELg0 event

        const AliMultiplicity* mult = fESD->GetMultiplicity();
        Int_t fSpdT_origin = mult->GetNumberOfTracklets();
        //std::cout << "# of total tracklet : "<< fSpdT_origin << std::endl;

        Int_t fNSpdT = 0;
        for (Int_t i=0; i<fSpdT_origin; ++i) if(TMath::Abs(mult->GetEta(i)) < 0.8) fNSpdT++;
        //std::cout << "# of passed tracklet : "<< fNSpdT << std::endl;
        
        // |Eta| < 1
        if(fNSpdT > 0){
            htotalEvent->Fill(2); // Total N of triggered event.

            if(fNSpdT > 0) htriggered_INELg0_tracklet->Fill(0); // INEL>0

            if(fNSpdT > 51) htriggered_INELg0_tracklet->Fill(10); // INEL>0
            else if(fNSpdT > 41) htriggered_INELg0_tracklet->Fill(9); // INEL>0
            else if(fNSpdT > 36) htriggered_INELg0_tracklet->Fill(8); // INEL>0
            else if(fNSpdT > 31) htriggered_INELg0_tracklet->Fill(7); // INEL>0
            else if(fNSpdT > 26) htriggered_INELg0_tracklet->Fill(6); // INEL>0
            else if(fNSpdT > 21) htriggered_INELg0_tracklet->Fill(5); // INEL>0
            else if(fNSpdT > 16) htriggered_INELg0_tracklet->Fill(4); // INEL>0
            else if(fNSpdT > 11) htriggered_INELg0_tracklet->Fill(3); // INEL>0
            else if(fNSpdT > 6) htriggered_INELg0_tracklet->Fill(2); // INEL>0
            else if(fNSpdT > 0) htriggered_INELg0_tracklet->Fill(1); // INEL>0
            
            // CINT7 Triggered event.
            Bool_t isSelectedkINT7 =(((AliInputEventHandler*)(AliAnalysisManager::GetAnalysisManager()->GetInputEventHandler()))->IsEventSelected() & AliVEvent::kINT7);
            if(isSelectedkINT7){

                htotalEvent->Fill(3); // Total N of triggered event.

                if(fNSpdT > 0) htriggered_CINT7_tracklet->Fill(0); // INEL>0 in CINT7
                
                if(fNSpdT > 51) htriggered_CINT7_tracklet->Fill(10); // INEL>0 in CINT7
                else if(fNSpdT > 41) htriggered_CINT7_tracklet->Fill(9); // INEL>0 in CINT7
                else if(fNSpdT > 36) htriggered_CINT7_tracklet->Fill(8); // INEL>0 in CINT7
                else if(fNSpdT > 31) htriggered_CINT7_tracklet->Fill(7); // INEL>0 in CINT7
                else if(fNSpdT > 26) htriggered_CINT7_tracklet->Fill(6); // INEL>0 in CINT7
                else if(fNSpdT > 21) htriggered_CINT7_tracklet->Fill(5); // INEL>0 in CINT7
                else if(fNSpdT > 16) htriggered_CINT7_tracklet->Fill(4); // INEL>0 in CINT7
                else if(fNSpdT > 11) htriggered_CINT7_tracklet->Fill(3); // INEL>0 in CINT7
                else if(fNSpdT > 6) htriggered_CINT7_tracklet->Fill(2); // INEL>0 in CINT7
                else if(fNSpdT > 0) htriggered_CINT7_tracklet->Fill(1); // INEL>0 in CINT7


                // AliMultSelection
                AliMultSelection *MultSelection = (AliMultSelection*) fESD->FindListObject("MultSelection");
                Float_t lPerc = 300; //nonsense

                //Quality check                                                                          // it should be same with 1.
                lPerc = MultSelection->GetMultiplicityPercentile("V0M");
                fMultDist->Fill(lPerc);

                if(lPerc < 101) htriggered_CINT7_VOM->Fill(0); // INEL>0 in CINT7
                
                if(lPerc > 70) htriggered_CINT7_VOM->Fill(1); // INEL>0 in CINT7
                else if(lPerc > 50) htriggered_CINT7_VOM->Fill(2); // INEL>0 in CINT7
                else if(lPerc > 40) htriggered_CINT7_VOM->Fill(3); // INEL>0 in CINT7
                else if(lPerc > 30) htriggered_CINT7_VOM->Fill(4); // INEL>0 in CINT7
                else if(lPerc > 20) htriggered_CINT7_VOM->Fill(5); // INEL>0 in CINT7
                else if(lPerc > 15) htriggered_CINT7_VOM->Fill(6); // INEL>0 in CINT7
                else if(lPerc > 10) htriggered_CINT7_VOM->Fill(7); // INEL>0 in CINT7
                else if(lPerc > 5) htriggered_CINT7_VOM->Fill(8); // INEL>0 in CINT7
                else if(lPerc > 1) htriggered_CINT7_VOM->Fill(9); // INEL>0 in CINT7
                else if(lPerc > 0) htriggered_CINT7_VOM->Fill(10); // INEL>0 in CINT7
                
                if(MultSelection->IsEventSelected()){
                    htotalEvent->Fill(4); // Total N of Multi selected event
                    fMultDist_pp->Fill(lPerc);

                    if(lPerc < 101) htriggered_AliMult_VOM->Fill(0); // INEL>0 in CINT7
                    
                    if(lPerc > 70) htriggered_AliMult_VOM->Fill(1); // INEL>0 in CINT7
                    else if(lPerc > 50) htriggered_AliMult_VOM->Fill(2); // INEL>0 in CINT7
                    else if(lPerc > 40) htriggered_AliMult_VOM->Fill(3); // INEL>0 in CINT7
                    else if(lPerc > 30) htriggered_AliMult_VOM->Fill(4); // INEL>0 in CINT7
                    else if(lPerc > 20) htriggered_AliMult_VOM->Fill(5); // INEL>0 in CINT7
                    else if(lPerc > 15) htriggered_AliMult_VOM->Fill(6); // INEL>0 in CINT7
                    else if(lPerc > 10) htriggered_AliMult_VOM->Fill(7); // INEL>0 in CINT7
                    else if(lPerc > 5) htriggered_AliMult_VOM->Fill(8); // INEL>0 in CINT7
                    else if(lPerc > 1) htriggered_AliMult_VOM->Fill(9); // INEL>0 in CINT7
                    else if(lPerc > 0) htriggered_AliMult_VOM->Fill(10); // INEL>0 in CINT7
                    
                }// IsEventSelected in AliMultSelection
            }//eta < 1
        }//CINT7
    }//INEL>0

    PostData(1, fOutputList);
}
//_____________________________________________________________________________
void AliAnalysisTaskMyQA::Terminate(Option_t *)
{
}
//_____________________________________________________________________________
