
//.L scanAll.C+
//scanAll("Data_plateau/input.tmp","Scan")

#include "TROOT.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TH1F.h"
#include "TString.h"
#include "TSystem.h"
#include "TTree.h"
#include "TFile.h"
#include "TGraphErrors.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector> 

void scanAll(std::string inputList, TString dataType) {

  //  find input files of type pla or Cu //
//    TString command = "ls " + dataFolder + "/" + dataType + "*.dat > input.tmp"; 
//    gSystem -> Exec(command); 

  // loop over the file and read data
  //  std::ifstream scanDir("input.tmp");
  std::ifstream scanDir(inputList.c_str());
  string line;

  // plateaufile
  TString inFile;
  float hv,eff,shv,sff; 
  int nFile=0; 
  TGraphErrors *g[15];  

  int colorsExt[9] = {kRed+1, kBlue, kGreen+2, kAzure+7, kOrange+7, kCyan, kBlack};
  std::vector<int> colors;
  for(int posVec = 0; posVec<7; ++posVec){
    colors.push_back(colorsExt[posVec]);
  }

  TLegend* leg = new TLegend(0.7,0.6,0.99,0.95);
  leg->SetFillColor(kWhite);
  
  // read all the plateau files in the directory
  while( scanDir.good() ){  
    scanDir >> inFile; // = (TString)line;
    cout << inFile << endl; 
    std::string inputName = std::string(std::string(inFile));
    if(inputName.size() == 0) continue;
    char split_char = '/';
    std::vector<std::string> tokens;
    std::istringstream split(inputName);
    for(std::string each; getline(split, each, split_char); 
        tokens.push_back(each));

    split_char = '_';
    std::vector<std::string> tokens_name;
    std::istringstream split_name(tokens.at(1));
    for(std::string each; getline(split_name, each, split_char); 
        tokens_name.push_back(each));

   tokens_name.at(tokens_name.size()-1).erase(tokens_name.at(tokens_name.size()-1).size()-4,tokens_name.at(tokens_name.size()-1).size()-1);
   
   std::string LegName = tokens_name.at(0)+"_"+tokens_name.at(1)+"_"+tokens_name.at(2)+"_"+tokens_name.at(3);
  
    g[nFile] = new TGraphErrors(); 
    g[nFile] -> SetMarkerStyle(20 + (nFile%2)*4 ); 
    g[nFile] -> SetMarkerColor(kRed+1); 
    if (nFile/2==1) g[nFile] -> SetMarkerColor(kGreen+1); 
    if (nFile/2==2) g[nFile] -> SetMarkerColor(kAzure+1); 
    if (nFile/2==3) g[nFile] -> SetMarkerColor(kOrange+2); 
    std::cout << " >>> nFile = " << nFile << std::endl;
    if(nFile != 6 && nFile != 7) g[nFile]->SetMarkerColor(colors.at(nFile%3));    
    else g[nFile]->SetMarkerColor(colors.at(4));    

    g[nFile]->SetMarkerStyle(20 + (nFile/3));    
  
    std::ifstream infile (inFile.Data(), std::ios::in);
    int ipt=0; 
    std::cout << "ipt = " << ipt << std::endl;
    while ( infile.good() ) { 
      infile >> hv >> eff >> shv >> sff;
      g[nFile]->SetPoint(ipt,hv,eff); 
      g[nFile]->SetPointError(ipt,shv,sff); 
      cout << nFile << " " << ipt << " " << hv << " " << eff << endl;
      ipt++;
    }
    
    leg->AddEntry(g[nFile],LegName.c_str(),"PL");
    
    nFile++;
  }

  g[0]->SetPoint(g[0]->GetN()+1,4000,2); 
  g[0]->SetPointError(g[0]->GetN()+1,0,0); 


  // do graph
  TCanvas *cc = new TCanvas("cc","cc",50.,50.,800.,500.); 
  cc -> cd();
  float xl=0.;
  float xh=15;
  TString xtit = "Absorber Thickness (X_{0})"; 
  if (dataType=="Scan") {
    xl=1200; 
    xh=3500.;
    xtit = "Bias voltage (V)"; 
  }
  if (dataType=="ScanCu") {
    xl=0.;
    xh=15;
    xtit = "Absorber Thickness (X_{0})"; 
  }

  TH1F *hf = (TH1F*)gPad->DrawFrame(xl,0.,xh,1.2); 
  //  hf->GetXaxis()->SetRangeUser(2200, xtit);   
  hf->GetXaxis()->SetTitle(xtit);   
  hf->GetYaxis()->SetTitle("Efficiency"); 
  for (int i=0;i<nFile;i++){
    g[i]->Draw("P"); 

  leg->Draw("same");
  }


  

}
 
