////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//   Compiling the code:   ./Make.sh jetToTauFakeRate.cc
//   Running the code:     ./jetToTauFakeRate.exe OutPut.root   Input.root
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "TreeReader.h"
#include <string>
#include <ostream>

int main(int argc, char** argv) {
    using namespace std;
    
    std::string out = *(argv + 1);
    
    cout << "\n\n\n OUTPUT NAME IS:    " << out << endl;     //PRINTING THE OUTPUT File name
    TFile *fout = TFile::Open(out.c_str(), "RECREATE");
    
    std::string input = *(argv + 2);
    cout << "\n\n\n InPUT NAME IS:    " << input << endl;     //PRINTING THE Input File name
    TFile * myFile = new TFile(input.c_str());
    
    TH1F *    histoDenumerator = new TH1F ("histoDenumerator","histoDenumerator", 300, 0, 300);
    TH1F *    histoNumerator = new TH1F ("histoNumerator","histoNumerator", 300, 0, 300);
    
    
    TTree *Run_Tree = (TTree*) myFile->Get("EventTree");
    cout.setf(ios::fixed, ios::floatfield);
    
    Run_Tree->SetBranchAddress("nTau", &nTau);
    Run_Tree->SetBranchAddress("tauPt"  ,&tauPt);
    Run_Tree->SetBranchAddress("tauEta"  ,&tauEta);
    Run_Tree->SetBranchAddress("tauPhi"  ,&tauPhi);
    Run_Tree->SetBranchAddress("tauMass"  ,&tauMass);
    Run_Tree->SetBranchAddress("tauDxy",&tauDxy);
    Run_Tree->SetBranchAddress("tauByTightMuonRejection3", &tauByTightMuonRejection3);
    Run_Tree->SetBranchAddress("tauByMVA5LooseElectronRejection", &tauByMVA5LooseElectronRejection);
    Run_Tree->SetBranchAddress("tauByLooseCombinedIsolationDeltaBetaCorr3Hits",&tauByLooseCombinedIsolationDeltaBetaCorr3Hits);
    
    Run_Tree->SetBranchAddress("nMu", &nMu);
    Run_Tree->SetBranchAddress("muPt"  ,&muPt);
    Run_Tree->SetBranchAddress("muEta"  ,&muEta);
    Run_Tree->SetBranchAddress("muPhi"  ,&muPhi);
    Run_Tree->SetBranchAddress("muIsMediumID",&muIsMediumID);
    Run_Tree->SetBranchAddress("muPFChIso", &muPFChIso);
    Run_Tree->SetBranchAddress("muPFPhoIso", &muPFPhoIso);
    Run_Tree->SetBranchAddress("muPFNeuIso", &muPFNeuIso);
    Run_Tree->SetBranchAddress("muPFPUIso", &muPFPUIso);
    Run_Tree->SetBranchAddress("muD0",&muD0);
    Run_Tree->SetBranchAddress("muDz",&muDz);
    
    Run_Tree->SetBranchAddress("pfMET",&pfMET);
    Run_Tree->SetBranchAddress("pfMETPhi",&pfMETPhi);
    
    Run_Tree->SetBranchAddress("HLTEleMuX", &HLTEleMuX);
    
    float MuMass= 0.10565837;
    float eleMass= 0.000511;
    
    Int_t nentries_wtn = (Int_t) Run_Tree->GetEntries();
    cout<<"nentries_wtn===="<<nentries_wtn<<"\n";
    for (Int_t i = 0; i < nentries_wtn; i++) {
        Run_Tree->GetEntry(i);
        if (i % 1000 == 0) fprintf(stdout, "\r  Processed events: %8d of %8d ", i, nentries_wtn);
        fflush(stdout);
        
        TLorentzVector Mu4Momentum, Tau4Momentum;
        
        //Loop over MuTau events
        for  (int imu=0 ; imu < nMu; imu++){
            
            float IsoMu=muPFChIso->at(imu)/muPt->at(imu);
            if ( (muPFNeuIso->at(imu) + muPFPhoIso->at(imu) - 0.5* muPFPUIso->at(imu) )  > 0.0)
            IsoMu= ( muPFChIso->at(imu)/muPt->at(imu) + muPFNeuIso->at(imu) + muPFPhoIso->at(imu) - 0.5* muPFPUIso->at(imu))/muPt->at(imu);
            
            Mu4Momentum.SetPtEtaPhiM(muPt->at(imu),muEta->at(imu),muPhi->at(imu),MuMass);
            
            float MuMetTranverseMass= TMass_F(muPt->at(imu), muPt->at(imu)*cos(muPhi->at(imu)),muPt->at(imu)*sin(muPhi->at(imu)) ,  pfMET, pfMETPhi);
            
            bool Select_W_event= ((HLTEleMuX >> 29 & 1) == 1 || (HLTEleMuX >> 30 & 1) == 1 ) && muPt->at(imu) > 25 && fabs(muEta->at(imu)) < 2.1 && (muIsMediumID->at(imu) > 0) && IsoMu < 0.10 && fabs(muD0->at(imu)) < 0.045 && fabs(muDz->at(imu)) < 0.2 && MuMetTranverseMass > 50;
            
            if (!Select_W_event) continue;
            
            for  (int itau=0 ; itau < nTau; itau++){
                Tau4Momentum.SetPtEtaPhiM(tauPt->at(itau),tauEta->at(itau),tauPhi->at(itau),tauMass->at(itau));
                
                bool TauPtCut = tauPt->at(itau) > 20  && fabs(tauEta->at(itau)) < 2.3 ;
                bool TauPreSelection = tauByTightMuonRejection3->at(itau) > 0 && tauByMVA5LooseElectronRejection->at(itau) > 0 && fabs(tauDxy->at(itau)) < 0.05 ;
                
                // Fill Denumereator
                if (Mu4Momentum.DeltaR(Tau4Momentum) > 0.5 && TauPtCut && TauPreSelection)
                histoDenumerator->Fill(tauPt->at(itau));
                // Fill Numerator
                if (Mu4Momentum.DeltaR(Tau4Momentum) > 0.5 && TauPtCut && TauPreSelection && tauByLooseCombinedIsolationDeltaBetaCorr3Hits->at(itau) > 0.5)
                histoNumerator->Fill(tauPt->at(itau));
                
            }
        }
    }
    fout->cd();
    histoNumerator->Write();
    histoDenumerator->Write();
    fout->Close();
}
