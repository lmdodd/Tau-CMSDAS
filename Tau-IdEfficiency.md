Tau Id Efficiency
=================

MC only- exercise 


If we choose something to be a tau how often is it a tau?

1) Find all generator level taus 

2) Find out how often generator-level taus are matched to a tau

3) Try all eta ranges

3a) Remember to always make an eta cut for your taus! 

3b) try only tracker eta ranges- show much better

The ratio we want is the number of taus passing an ID with all taus in the denominator. 


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

You want to read specific branches from the provided Ntuples, so add the following lines to access the branches you want  

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

    Run_Tree->SetBranchAddress("pfMET",&pfMET);
    Run_Tree->SetBranchAddress("pfMETPhi",&pfMETPhi);

```

Now you want to process all events so add a loop to process all events 

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

within your event loop you want to find all generator-level MC taus

```
        //Loop over generator-level Tau events
        for  (int imc=0 ; imc < nMC; imc++){
            
            MC4Momentum.SetPtEtaPhiM(mcPt->at(imc),mcEta->at(imc),mcPhi->at(imc),mcMass->at(imc));
            
            bool Select_GenTau= abs(mcPID->at(imc))==15; 
            
            if (!Select_GenTau) continue;
   
        } //end loop of Generator-level Taus 
         
```

now if we did select a generator level taus we want to match to a reconstructed hadronic decay tau 



