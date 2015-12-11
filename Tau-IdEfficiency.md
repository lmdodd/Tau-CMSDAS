Tau Id Efficiency
=================

MC only- exercise 


How likely is it that we find a real tau?

1) Find all generator level taus 

2) Find out how often generator-level taus are matched to a tau

3) Try all eta ranges- Remember to always make an eta cut for your taus! Need the tracker!

4) Try a pt cut for the generator level taus, this could be added to the ```TauPreSelection``` for example! The gen Pt of the tau will be higher than the reconstructed pt of the tau, because a hadronically decaying tau will have a neutrino in the decay products which is not seen. Plot the efficiency as a function of the genPt of the tau. 

5) See the efficieniecy of different working points! Add in other tauIDs from the TreeReader.h 

6) the choice of tau Id should be based on *both* fake rate and efficiency



The ratio(Efficiency) we want is the number of real taus passing an ID with all real taus in the denominator. 


Create a new file named tauEfficiency.cc
paste at the top 
```
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//   Compiling the code:   ./Make.sh tauEfficiency.cc
//   Running the code:     ./tauEfficiency.exe OutPut.root   Input.root
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
 
    //add the histrograms of All gen-matched taus, and all gen-matched taus passing numerator
    TH1F *    histoDenumerator = new TH1F ("histoDenumerator","histoDenumerator", 300, 0, 300);
    TH1F *    histoNumerator = new TH1F ("histoNumerator","histoNumerator", 300, 0, 300);
    
    
    TTree *Run_Tree = (TTree*) myFile->Get("EventTree");
    cout.setf(ios::fixed, ios::floatfield);




}

```

You want to read specific branches from the provided Ntuples, so add the following lines to access the branches you want after opening the file i.e. after 'cout.setf(ios::fixed, ios::floatfield);' 

``` 
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

```

Now you want to process all events so add a loop to process all events after you've declared the branches. 

```
    Int_t nentries_wtn = (Int_t) Run_Tree->GetEntries();
    cout<<"nentries_wtn===="<<nentries_wtn<<"\n";
    for (Int_t i = 0; i < nentries_wtn; i++) {
        Run_Tree->GetEntry(i);
        if (i % 1000 == 0) fprintf(stdout, "\r  Processed events: %8d of %8d ", i, nentries_wtn);
        fflush(stdout);
 
       ////////////////////////////////////////////////
       //Important analysis stuff will happen here!!!//
       ////////////////////////////////////////////////


   } //End Processing all entries
```

In the nentries loop paste 
```
	TLorentzVector MC4Momentum, Tau4Momentum;


	for  (int itau=0 ; itau < nTau; itau++){
		Tau4Momentum.SetPtEtaPhiM(tauPt->at(itau),tauEta->at(itau),tauPhi->at(itau),tauMass->at(itau));

		bool TauPtCut = tauPt->at(itau) > 20  && fabs(tauEta->at(itau)) < 2.3 ;
		bool TauPreSelection = fabs(tauDxy->at(itau)) < 0.05 ;

		//Loop Over Generator-Level Tau events
	
		////////////////////////////////////////////////
                //Loop Over Generator-Level Taus////////////////
                ////////////////////////////////////////////////
	}//end reconstructed tau loop
```

Now insert the following lines into Generator-Level tau loop. This fills two hisstograms for the numerator and denominator.

```
		for  (int imc=0 ; imc < nMC; imc++){

			MC4Momentum.SetPtEtaPhiM(mcPt->at(imc),mcEta->at(imc),mcPhi->at(imc),mcMass->at(imc));

			bool Select_GenTau= abs(mcPID->at(imc))==15&&MC4Momentum.DeltaR(Tau4Momentum) < 0.2; 

			if (!Select_GenTau) continue;

			if (TauPtCut && TauPreSelection)
				histoDenumerator->Fill(tauPt->at(itau));
			if (TauPtCut && TauPreSelection && tauByLooseCombinedIsolationDeltaBetaCorr3Hits->at(itau) > 0.5)
				histoNumerator->Fill(tauPt->at(itau));

			break; //Exit the tau loop, a match was found!
		}
```

Now your code is all set up. You can ```/Make.sh tauEfficiency.cc``` and then ```./tauEfficiency.exe outputEfficiency.root tot_job_spring15_ggNtuple_WJetsToLNu_amcatnlo_pythia8_25ns_miniAOD.root ``` 

When you have ```outputEfficiency.root``` you can run ```python plotEfficiency.py``` to create ```tauEfficiency.pdf```

