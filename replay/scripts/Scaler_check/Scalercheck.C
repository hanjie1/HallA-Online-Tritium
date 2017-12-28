#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <stdexcept>
#include <cassert>

#include "TTree.h"
#include "TFile.h"
#include "TString.h"


using namespace std;

void Scalercheck(Int_t run_number=0)
{
  if(run_number==0){
     cout << "enter run_number: ";
     cin >> run_number;
   }
  char filename[300];
  char lastfile[300];
  static const char* ROOTPATH1="/chafs1/work1/tritium/Rootfiles/tritium_%d.root";
  static const char* ROOTPATH2="/chafs1/work1/tritium/Rootfiles/tritium_%d_%d.root";

  TChain *T = new TChain("T");

  bool fexit=false;
  for (Int_t nsplit=0;!fexit;nsplit++)
    {
      if(nsplit==0)sprintf(filename,ROOTPATH1,run_number,nsplit);
      else sprintf(filename,ROOTPATH2,run_number,nsplit);
      cout<<"Try to open Rootfile: "<<filename<<endl;
  
      fstream testfile;

      testfile.open(filename,ios_base::in);
      bool isopen=testfile.is_open();
      testfile.close();

      if(isopen)
       {
         T->Add(filename);
	 memcpy(lastfile,filename,sizeof(filename));
       }
      else 
       {  fexit = true;
	  if(nsplit==0)exit(0);
       }

    }

  TChain *T1 = new TChain("EndLeft");
  T1->Add(lastfile);

  cout<<"Find the last rootfile:  "<<lastfile<<endl;
  Int_t entry,slowscaler,endscaler,fastscaler;
  entry = T->GetEntries();
  slowscaler = T->GetMaximum("LeftL1A");   
  endscaler = T1->GetMaximum("EndLeftL1A");
  fastscaler = T->GetMaximum("evLeftL1A");

  ofstream scalerfile;
  scalerfile.open("scaler_check.txt",fstream::app);
  if(scalerfile.is_open())
   {
     scalerfile<<setiosflags(ios::left)<<setw(11)<<run_number<<setw(19)<<entry<<setw(22)<<slowscaler<<setw(22)
     <<endscaler<<setw(11)<<fastscaler<<endl;
     if(slowscaler!=endscaler || endscaler!=entry)scalerfile<<setiosflags(ios::left)<<"!!! This run scaler has issue !!!"<<endl;     
   }
  scalerfile.close();
  exit(0);
}
