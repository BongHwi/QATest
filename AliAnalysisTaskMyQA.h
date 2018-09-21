/* Copyright(c) 1998-1999, ALICE Experiment at CERN, All rights reserved. */
/* See cxx source for full Copyright notice */
/* $Id$ */

#ifndef AliAnalysisTaskMyQA_H
#define AliAnalysisTaskMyQA_H

#include "AliAnalysisTaskSE.h"

class AliAnalysisTaskMyQA : public AliAnalysisTaskSE  
{
    public:
                                AliAnalysisTaskMyQA();
                                AliAnalysisTaskMyQA(const char *name);
        virtual                 ~AliAnalysisTaskMyQA();

        virtual void            UserCreateOutputObjects();
        virtual void            UserExec(Option_t* option);
        virtual void            Terminate(Option_t* option);

    private:
        AliESDEvent*            fESD;           //! input event
        TList*                  fOutputList;    //! output list
        TH1F*                   htotalEvent;        //! dummy histogram
        TH1F*                   htriggered_INELg0_tracklet;//! dummy histogram
        //TH1F*                   htriggered_INELg0_VOM;//! dummy histogram
        TH1F*                   htriggered_CINT7_tracklet;//! dummy histogram
        TH1F*                   htriggered_CINT7_VOM;//! dummy histogram
        TH1F*                   htriggered_AliMult_VOM;//! dummy histogram
        TH1F*                   fMultDist;//! dummy histogram
        TH1F*                   fMultDist_pp;//! dummy histogram

        AliAnalysisTaskMyQA(const AliAnalysisTaskMyQA&); // not implemented
        AliAnalysisTaskMyQA& operator=(const AliAnalysisTaskMyQA&); // not implemented

        ClassDef(AliAnalysisTaskMyQA, 1);
};

#endif
