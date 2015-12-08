#include "TreeReader.h"
#include "WeightCalculator.h"
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
    TH1F * HistoTot = (TH1F*) myFile->Get("hcount");
    TH1F * HistohPU = (TH1F*) myFile->Get("hPU");
    TH1F * HistohPUTrue = (TH1F*) myFile->Get("hPUTrue");

    
    
        TH1F *    visibleMassOS = new TH1F ("visibleMassOS","visibleMassOS", 300, 0, 300);
        TH1F *    visibleMassSS = new TH1F ("visibleMassSS","visibleMassSS", 300, 0, 300);
    

        TTree *Run_Tree = (TTree*) myFile->Get("EventTree");
        
        cout.setf(ios::fixed, ios::floatfield);
        cout.precision(6);
        
        
        Run_Tree->SetBranchAddress("run", &run);
        Run_Tree->SetBranchAddress("lumis", &lumis);
        Run_Tree->SetBranchAddress("event", &event);
        
        Run_Tree->SetBranchAddress("nTau", &nTau);
        Run_Tree->SetBranchAddress("tauPt"  ,&tauPt);
        Run_Tree->SetBranchAddress("tauEta"  ,&tauEta);
        Run_Tree->SetBranchAddress("tauPhi"  ,&tauPhi);
        Run_Tree->SetBranchAddress("tauMass"  ,&tauMass);
        Run_Tree->SetBranchAddress("tauCharge"  ,&tauCharge);
        Run_Tree->SetBranchAddress("tauByTightMuonRejection3", &tauByTightMuonRejection3);
        Run_Tree->SetBranchAddress("tauByLooseCombinedIsolationDeltaBetaCorr3Hits",&tauByLooseCombinedIsolationDeltaBetaCorr3Hits);
        Run_Tree->SetBranchAddress("tauByMediumCombinedIsolationDeltaBetaCorr3Hits",&tauByMediumCombinedIsolationDeltaBetaCorr3Hits);
        Run_Tree->SetBranchAddress("tauByMVA5LooseElectronRejection", &tauByMVA5LooseElectronRejection);
        Run_Tree->SetBranchAddress("tauDxy",&tauDxy);
        
        Run_Tree->SetBranchAddress("nMu", &nMu);
        Run_Tree->SetBranchAddress("muPt"  ,&muPt);
        Run_Tree->SetBranchAddress("muEta"  ,&muEta);
        Run_Tree->SetBranchAddress("muPhi"  ,&muPhi);
        Run_Tree->SetBranchAddress("muIsoTrk", &muIsoTrk);
        Run_Tree->SetBranchAddress("muCharge",&muCharge);
        Run_Tree->SetBranchAddress("muIsMediumID",&muIsMediumID);
        Run_Tree->SetBranchAddress("muIsLooseID",&muIsLooseID);
        Run_Tree->SetBranchAddress("muPFChIso", &muPFChIso);
        Run_Tree->SetBranchAddress("muPFPhoIso", &muPFPhoIso);
        Run_Tree->SetBranchAddress("muPFNeuIso", &muPFNeuIso);
        Run_Tree->SetBranchAddress("muPFPUIso", &muPFPUIso);
        Run_Tree->SetBranchAddress("muD0",&muD0);
        Run_Tree->SetBranchAddress("muDz",&muDz);
        

        Run_Tree->SetBranchAddress("pfMET",&pfMET);
        Run_Tree->SetBranchAddress("pfMETPhi",&pfMETPhi);
        
        Run_Tree->SetBranchAddress("genWeight",&genWeight);
        
        Run_Tree->SetBranchAddress("HLTEleMuX", &HLTEleMuX);

        
        float MuMass= 0.10565837;
        float eleMass= 0.000511;
        
        float LumiWeight = 1;
        if (HistoTot) LumiWeight = weightCalc(HistoTot, input);
        cout<<"LumiWeight is "<<LumiWeight<<"\n";
        
        
        
        Int_t nentries_wtn = (Int_t) Run_Tree->GetEntries();
        cout<<"nentries_wtn===="<<nentries_wtn<<"\n";
        for (Int_t i = 0; i < nentries_wtn; i++) {
            Run_Tree->GetEntry(i);
            if (i % 10000 == 0) fprintf(stdout, "\r  Processed events: %8d of %8d ", i, nentries_wtn);
            fflush(stdout);
            
            
            float GetGenWeight=1;
            if (HistoTot) GetGenWeight=genWeight;
            
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            // Loop over Di-Mu events  We need to veto these events later
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            bool IsthereDiMuon= false;
            
            for  (int imu=0 ; imu < nMu; imu++){
                for  (int jmu=0 ; jmu < nMu; jmu++){
                    
                    
                    // Select first good muon
                    bool MuPtCut1 = muPt->at(imu) > 15 && fabs(muEta->at(imu)) < 2.4 ;
                    float IsoMu1=muPFChIso->at(imu)/muPt->at(imu);
                    if ( (muPFNeuIso->at(imu) + muPFPhoIso->at(imu) - 0.5* muPFPUIso->at(imu) )  > 0.0)
                        IsoMu1= ( muPFChIso->at(imu)/muPt->at(imu) + muPFNeuIso->at(imu) + muPFPhoIso->at(imu) - 0.5* muPFPUIso->at(imu))/muPt->at(imu);
                    bool MuIdIso1=(muIsLooseID->at(imu) > 0 && IsoMu1 < 0.30 && fabs(muD0->at(imu)) < 0.045 && fabs(muDz->at(imu)) < 0.2);
                    
                    
                    // Select second good muon
                    bool MuPtCut2 = muPt->at(jmu) > 15 && fabs(muEta->at(jmu)) < 2.4 ;
                    float IsoMu2=muPFChIso->at(jmu)/muPt->at(jmu);
                    if ( (muPFNeuIso->at(jmu) + muPFPhoIso->at(jmu) - 0.5* muPFPUIso->at(jmu) )  > 0.0)
                        IsoMu2= ( muPFChIso->at(jmu)/muPt->at(jmu) + muPFNeuIso->at(jmu) + muPFPhoIso->at(jmu) - 0.5* muPFPUIso->at(jmu))/muPt->at(jmu);
                    bool MuIdIso2=(muIsLooseID->at(jmu) > 0 && IsoMu2 < 0.30 && fabs(muD0->at(jmu)) < 0.045 && fabs(muDz->at(jmu)) < 0.2);
                    
                    
                    bool  OS = muCharge->at(imu) * muCharge->at(jmu) < 0;
                    
                    if(MuIdIso1 && MuIdIso2 && OS)
                        IsthereDiMuon=true;
                    
                }
            }
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            //  Loop Over events with one muon and one Taus
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            TLorentzVector Mu4Momentum, Tau4Momentum, Z4Momentum;
            
            for  (int imu=0 ; imu < nMu; imu++){
                for  (int itau=0 ; itau < nTau; itau++){
                    
                    // Check Single Muon Trigger  (Maybe we need to add Mu+Tau Trigger)
                    bool PassTrigger = ((HLTEleMuX >> 29 & 1) == 1 || (HLTEleMuX >> 30 & 1) == 1 );
                    
                    //  Muon Pt and Eta cuts
                    bool MuPtCut = muPt->at(imu) > 10 && fabs(muEta->at(imu)) < 2.1 ;
                    
                    // Check Muon Isolation   (delta beta correction is applied)
                    float IsoMu=muPFChIso->at(imu)/muPt->at(imu);
                    if ( (muPFNeuIso->at(imu) + muPFPhoIso->at(imu) - 0.5* muPFPUIso->at(imu) )  > 0.0)
                        IsoMu= ( muPFChIso->at(imu)/muPt->at(imu) + muPFNeuIso->at(imu) + muPFPhoIso->at(imu) - 0.5* muPFPUIso->at(imu))/muPt->at(imu);
                    
                    //Apply Muon Id and Isolation and dxy/dz cuts
                    bool MuIdIso=(muIsMediumID->at(imu) > 0 && IsoMu < 0.10 && fabs(muD0->at(imu)) < 0.045 && fabs(muDz->at(imu)) < 0.2);
                    
                    // Tau Pt and Eta cuts
                    bool TauPtCut = tauPt->at(itau) > 20  && fabs(tauEta->at(itau)) < 2.3 ;
                    
                    //Apply tau Id and Isolation and dxy
                    bool TauIdIso =  tauByMediumCombinedIsolationDeltaBetaCorr3Hits->at(itau) && tauByTightMuonRejection3->at(itau) > 0 && tauByMVA5LooseElectronRejection->at(itau) > 0 && fabs(tauDxy->at(itau)) < 0.05 ;
                    
                    // Check charge of the muon and Taus
                    bool  OS = muCharge->at(imu) * tauCharge->at(itau) < 0;
                    bool  SS = muCharge->at(imu) * tauCharge->at(itau) > 0;
                    
                    
                    // Get the 4-,omuntum on tau/mu/Z
                    Mu4Momentum.SetPtEtaPhiM(muPt->at(imu),muEta->at(imu),muPhi->at(imu),MuMass);
                    Tau4Momentum.SetPtEtaPhiM(tauPt->at(itau),tauEta->at(itau),tauPhi->at(itau),tauMass->at(itau));
                    Z4Momentum=Mu4Momentum+Tau4Momentum;
                    
                    
                    // Measuring the TMass to apply a cutr on that later on and Kill W+Jets events
                    float tmass= TMass_F(muPt->at(imu), muPt->at(imu)*cos(muPhi->at(imu)),muPt->at(imu)*sin(muPhi->at(imu)) ,  pfMET, pfMETPhi);
                    
                    
                    
                    //  Fill out the Visible mass for OS events that pass all events curt
                    if (OS && !IsthereDiMuon && MuPtCut && TauPtCut && MuIdIso && TauIdIso && tmass < 40 && Mu4Momentum.DeltaR(Tau4Momentum) > 0.5 && PassTrigger)
                        visibleMassOS->Fill(Z4Momentum.M(),LumiWeight*GetGenWeight);
                        
                        
                                //  Fill out the Visible mass for SS events that pass all events curt
                    if (SS && !IsthereDiMuon && MuPtCut && TauPtCut && MuIdIso && TauIdIso &&  tmass < 40 && Mu4Momentum.DeltaR(Tau4Momentum) > 0.5 && PassTrigger)
                        visibleMassSS->Fill(Z4Momentum.M(),LumiWeight*GetGenWeight);

      
                }
            }
            /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        
        
    }
    
    fout->cd();
    visibleMassOS->Write();
    visibleMassSS->Write();
    fout->Close();
    
    
}




