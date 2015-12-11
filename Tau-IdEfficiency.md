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


}

```

You want to read specific branches from the provided Ntuples, so add the following lines to access the branches you want 

``` 
```
open up your 

A Z->tautau with one good muon will provide a fairly good seleciton of hadronic taus. Ask for no electrons, and no extra jets. 
