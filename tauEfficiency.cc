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
    
    Run_Tree->SetBranchAddress("nMC", &nMC);
    Run_Tree->SetBranchAddress("mcPID", &mcPID);
    Run_Tree->SetBranchAddress("mcPt", &mcPt);
    Run_Tree->SetBranchAddress("mcMass", &mcMass);
    Run_Tree->SetBranchAddress("mcEta", &mcEta);
    Run_Tree->SetBranchAddress("mcPhi", &mcPhi);

    Run_Tree->SetBranchAddress("nTau", &nTau);
    Run_Tree->SetBranchAddress("tauPt"  ,&tauPt);
    Run_Tree->SetBranchAddress("tauEta"  ,&tauEta);
    Run_Tree->SetBranchAddress("tauPhi"  ,&tauPhi);
    Run_Tree->SetBranchAddress("tauMass"  ,&tauMass);
    Run_Tree->SetBranchAddress("tauDxy",&tauDxy);
    Run_Tree->SetBranchAddress("tauByTightMuonRejection3", &tauByTightMuonRejection3);
    Run_Tree->SetBranchAddress("tauByMVA5LooseElectronRejection", &tauByMVA5LooseElectronRejection);
    Run_Tree->SetBranchAddress("tauByLooseCombinedIsolationDeltaBetaCorr3Hits",&tauByLooseCombinedIsolationDeltaBetaCorr3Hits);

    Run_Tree->SetBranchAddress("pfMET",&pfMET);
    Run_Tree->SetBranchAddress("pfMETPhi",&pfMETPhi);
    
    
    float MuMass= 0.10565837;
    float eleMass= 0.000511;
    
    Int_t nentries_wtn = (Int_t) Run_Tree->GetEntries();
    cout<<"nentries_wtn===="<<nentries_wtn<<"\n";
    for (Int_t i = 0; i < nentries_wtn; i++) {
        Run_Tree->GetEntry(i);
        if (i % 1000 == 0) fprintf(stdout, "\r  Processed events: %8d of %8d ", i, nentries_wtn);
        fflush(stdout);
        
        TLorentzVector MC4Momentum, Tau4Momentum;
        
        //Loop over MCTau events
        for  (int imc=0 ; imc < nMC; imc++){
            
            MC4Momentum.SetPtEtaPhiM(mcPt->at(imc),mcEta->at(imc),mcPhi->at(imc),mcMass->at(imc));
            
            bool Select_GenTau= abs(mcPID->at(imc))==15; 
            
            if (!Select_GenTau) continue;
            
            for  (int itau=0 ; itau < nTau; itau++){
                Tau4Momentum.SetPtEtaPhiM(tauPt->at(itau),tauEta->at(itau),tauPhi->at(itau),tauMass->at(itau));
                
                bool TauPtCut = tauPt->at(itau) > 20  && fabs(tauEta->at(itau)) < 2.3 ;
		bool TauPreSelection = fabs(tauEta->at(itau)) < 2.3 ;
		//bool TauPreSelection = fabs(tauDxy->at(itau)) < 0.05 ;
		//bool TauPreSelection = tauByTightMuonRejection3->at(itau) > 0 && tauByMVA5LooseElectronRejection->at(itau) > 0 && fabs(tauDxy->at(itau)) < 0.05 ;

		if (MC4Momentum.DeltaR(Tau4Momentum) < 0.2){
			if (MC4Momentum.DeltaR(Tau4Momentum) < 0.2 && TauPtCut && TauPreSelection)
				histoDenumerator->Fill(tauPt->at(itau));
			if (MC4Momentum.DeltaR(Tau4Momentum) < 0.2 && TauPtCut && TauPreSelection && tauByLooseCombinedIsolationDeltaBetaCorr3Hits->at(itau) > 0.5)
				histoNumerator->Fill(tauPt->at(itau));

			break; //exit the tau loop, a match was found!
		}

	    }
	}
    }
    fout->cd();
    histoNumerator->Write();
    histoDenumerator->Write();
    fout->Close();
}
