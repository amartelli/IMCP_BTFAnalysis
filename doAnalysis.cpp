// g++  -o doAnalysis `root-config --cflags --glibs` doAnalysis.cpp 
// ./doAnalysis WaveForms_BTF/ScanX* X 
// if X == 3 use ./doAnalysis WaveForms_BTF/ScanX* X a

#include <cstdio>
#include <cstdlib>
#include <stdint.h>

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include <algorithm>
#include <sstream> 
#include <cstdio>
#include <cstdlib>
#include <stdint.h>
#include <iomanip>
#include <ctime>
#include <map>
#include <math.h>

#include "TROOT.h"
#include "TFile.h"
#include "TSystem.h"
#include "TStyle.h"
#include "TChain.h"
#include "TF1.h"
#include "TH1F.h"
#include "TH1D.h"
#include "TProfile.h"
#include "TMath.h"
#include "TString.h"
#include "TClass.h"
#include "TApplication.h"

#include "analysis_tools.h"
#include "InitTree_BTF.h"



int main (int argc, char** argv)
{  
  gROOT->ProcessLine("#include <vector>");

  std::string inputName = std::string(argv[1]);
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

  const int Ch_ref1 = atoi((tokens_name.at(1)).c_str());
  const int Ch_ref2 = atoi((tokens_name.at(3)).c_str());
  const int Ch_1 = atoi((tokens_name.at(5)).c_str());
  const int Ch_2 = atoi((tokens_name.at(7)).c_str());
  const int Ch_3 = atoi((tokens_name.at(9)).c_str());

  std::vector<TString> nameMCP;
  nameMCP.push_back("MiB1");
  nameMCP.push_back("MiB2");
  if(argc > 3) nameMCP.at(1) = "Roma1";
  nameMCP.push_back("ScB");
  nameMCP.push_back("Planacon");
  nameMCP.push_back("MiB3");
  nameMCP.push_back("Roma2");
    
  std::vector<std::string> pcMCP;                                     
  for(unsigned int ii=0; ii<nameMCP.size(); ++ii) pcMCP.push_back("");
  pcMCP.at(Ch_ref1) = tokens_name.at(2);
  pcMCP.at(Ch_ref2) = tokens_name.at(4);
  pcMCP.at(Ch_1) = tokens_name.at(6);
  pcMCP.at(Ch_2) = tokens_name.at(8);
  pcMCP.at(Ch_3) = tokens_name.at(10);
  pcMCP.at(Ch_3).erase(pcMCP.at(Ch_3).size()-4, pcMCP.at(Ch_3).size()-1);
    
  // treshold setup Scan-dependent
  init();
  const int iScan = atoi(argv[2])-1;
  float Ch_th[6]={0,0,0,0,0,0};
  Ch_th[Ch_ref1] = _th[iScan][Ch_ref1];
  Ch_th[Ch_ref2] = _th[iScan][Ch_ref2];
  Ch_th[Ch_1] = _th[iScan][Ch_1];
  Ch_th[Ch_2] = _th[iScan][Ch_2];
  Ch_th[Ch_3] = _th[iScan][Ch_3];
    
  // Out tree
  TString outName = "outAnalysis_"+tokens_name.at(0)+".root";
  TFile* outROOT = TFile::Open(outName.Data(),"recreate");  
  outROOT->cd();
  TTree* outTree = new TTree("analysis_tree", "analysis_tree");
  outTree->SetDirectory(0);

  float coinc_Ch1 = 0, coinc_Ch2 = 0, coinc_Ch3 = 0, coinc_ref1 = 0;
  float amp_max_Ch1 = 0, amp_max_Ch2 = 0, amp_max_Ch3 = 0, amp_max_ref1 = 0;
  float charge_Ch1 = 0, charge_Ch2 = 0, charge_Ch3 = 0, charge_ref1 = 0;
  float baseline_Ch1 = 0, baseline_Ch2 = 0, baseline_Ch3 = 0, baseline_ref1 = 0;
  int sci_front_adc = 0, run_id = 0;

  outTree->Branch("coinc_"+nameMCP.at(Ch_1),&coinc_Ch1,"coinc_"+nameMCP.at(Ch_1)+"/F");
  outTree->Branch("coinc_"+nameMCP.at(Ch_2),&coinc_Ch2,"coinc_"+nameMCP.at(Ch_2)+"/F");
  outTree->Branch("coinc_"+nameMCP.at(Ch_3),&coinc_Ch3,"coinc_"+nameMCP.at(Ch_3)+"/F");
  outTree->Branch("coinc_"+nameMCP.at(Ch_ref1),&coinc_ref1,"coinc_"+nameMCP.at(Ch_ref1)+"/F");
  outTree->Branch("amp_max_"+nameMCP.at(Ch_1),&amp_max_Ch1,"amp_max_"+nameMCP.at(Ch_1)+"/F");
  outTree->Branch("amp_max_"+nameMCP.at(Ch_2),&amp_max_Ch2,"amp_max_"+nameMCP.at(Ch_2)+"/F");
  outTree->Branch("amp_max_"+nameMCP.at(Ch_3),&amp_max_Ch3,"amp_max_"+nameMCP.at(Ch_3)+"/F");
  outTree->Branch("amp_max_"+nameMCP.at(Ch_ref1),&amp_max_ref1,"amp_max_"+nameMCP.at(Ch_ref1)+"/F");
  outTree->Branch("charge_"+nameMCP.at(Ch_1),&charge_Ch1,"charge_"+nameMCP.at(Ch_1)+"/F");
  outTree->Branch("charge_"+nameMCP.at(Ch_2),&charge_Ch2,"charge_"+nameMCP.at(Ch_2)+"/F");
  outTree->Branch("charge_"+nameMCP.at(Ch_3),&charge_Ch3,"charge_"+nameMCP.at(Ch_3)+"/F");
  outTree->Branch("charge_"+nameMCP.at(Ch_ref1),&charge_ref1,"charge_"+nameMCP.at(Ch_ref1)+"/F");
  outTree->Branch("baseline_"+nameMCP.at(Ch_1),&baseline_Ch1,"baseline_"+nameMCP.at(Ch_1)+"/F");
  outTree->Branch("baseline_"+nameMCP.at(Ch_2),&baseline_Ch2,"baseline_"+nameMCP.at(Ch_2)+"/F");
  outTree->Branch("baseline_"+nameMCP.at(Ch_3),&baseline_Ch3,"baseline_"+nameMCP.at(Ch_3)+"/F");
  outTree->Branch("baseline_"+nameMCP.at(Ch_ref1),&baseline_ref1,"baseline_"+nameMCP.at(Ch_ref1)+"/F");
  outTree->Branch("sci_front_adc",&sci_front_adc,"sci_front_adc/I");
  outTree->Branch("run_id",&run_id,"run_id/I");

  //Output dat
  std::ofstream data1(("Data_plateau/"+tokens_name.at(0)+"_"+nameMCP.at(Ch_1)+"_pc_"+pcMCP.at(Ch_1)+".dat").Data());
  std::ofstream data2(("Data_plateau/"+tokens_name.at(0)+"_"+nameMCP.at(Ch_2)+"_pc_"+pcMCP.at(Ch_2)+".dat").Data());
  std::ofstream data3(("Data_plateau/"+tokens_name.at(0)+"_"+nameMCP.at(Ch_3)+"_pc_"+pcMCP.at(Ch_3)+".dat").Data());

  //Run over files
  int nFiles = 1;
  ifstream log(argv[1], ios::in);
  while(log >> nFiles){

    vector<float> digiCh[9];
    float timeCF[9], timeAM[9];
    float baseline[9];
    float intBase[9], intSignal[9], ampMax[9];
    int count[5] = {0,0,0,0,0}, spare[5] = {0,0,0,0,0}, spare2[5] = {0,0,0,0,0};
    int tot_tr1 = 0, tot_tr0 = 0, trig = 0;
    int HV1 = 0, HV2 = 0, HV3 = 0, iRun = 0, goodEvt = 1;

    TChain* chain = new TChain("eventRawData");
    InitTree(chain);

    for(int iFiles=0; iFiles<nFiles; iFiles++){
      log >> iRun;
      char iRun_str[40];
      sprintf(iRun_str, "../DATA/run_IMCP_%d_*.root", iRun);
      chain->Add(iRun_str);
      cout << "Reading:  ../DATA/run_IMCP_" << iRun << endl;
    }
    log >> HV1 >> HV2 >> HV3;
    
    for(int iEntry=0; iEntry<chain->GetEntries(); iEntry++){
      
      for(int iCh=0; iCh<9; iCh++) digiCh[iCh].clear();
      
      chain->GetEntry(iEntry);

      //DAQ bug workaround
      if(iRun < 145) goodEvt = 10;
      else goodEvt = 1;
      if(evtNumber % goodEvt == 0)   {

	//SciFront ADC value and set the e- multiplicity 
	trig = 0;
	for(int iCh=0; iCh<nAdcChannels; iCh++){
	  if(adcBoard[iCh] == 1 && adcChannel[iCh] == 0){
	    sci_front_adc = adcData[iCh];
	    if(500 < sci_front_adc && sci_front_adc < 1500) trig = 1;
	    if(sci_front_adc > 1500) trig = 2;
	  }
	}
	
	for(int iSample=0; iSample<nDigiSamples; iSample++){
	  if(digiChannel[iSample] == 3) digiCh[digiChannel[iSample]].push_back(-digiSampleValue[iSample]);
	  else digiCh[digiChannel[iSample]].push_back(digiSampleValue[iSample]);
	}

	for(int iCh=0; iCh<6; iCh++){
	  baseline[iCh] = SubtractBaseline(5, 25, &digiCh[iCh]);
	  intBase[iCh] = ComputeIntegral(26, 46, &digiCh[iCh]);

	  timeCF[iCh] = TimeConstFrac(47, 500, &digiCh[iCh], 0.5);
	  timeAM[iCh] = TimeConstFrac(47, 500, &digiCh[iCh], 1);
	  int t1 = (int)(timeCF[iCh]/0.2) - 3;
	  int t2 = (int)(timeCF[iCh]/0.2) + 17;

	  if(t1 > 50 && t1 < 1024 && t2 > 50 && t2 < 1024){
	    ampMax[iCh] = AmpMax(t1, t2, &digiCh[iCh]);
	    intSignal[iCh] = ComputeIntegral(t1, t2, &digiCh[iCh]);
	  }	    
	  else{
	    ampMax[iCh] = AmpMax(0, 1024, &digiCh[iCh]);
	    intSignal[iCh] = ComputeIntegral(50, 70, &digiCh[iCh]);
	  }
	} // loop over Ch

	if(intSignal[Ch_ref1] < Ch_th[Ch_ref1] && intSignal[Ch_ref2] < Ch_th[Ch_ref2] && trig == 1){
	  ++tot_tr1;                                    

	  //Ref
	  time_ref1 = timeCF[Ch_ref1];
	  amp_max_ref1 = ampMax[Ch_ref1];
	  charge_ref1 = intSignal[Ch_ref1];
	  baseline_ref1 = intBase[Ch_ref1];

	  time_ref2 = timeCF[Ch_ref2] -timeCF[Ch_ref1];
	  amp_max_ref2 = ampMax[Ch_ref2];
	  charge_ref2 = intSignal[Ch_ref2];
	  baseline_ref2 = intBase[Ch_ref2];

	  //Ch1
	  if(intSignal[Ch_1] < Ch_th[Ch_1]){ 
	    ++count[1];
	    coinc_Ch1 = timeCF[Ch_ref1] - timeCF[Ch_1];
	  }

	  if(intBase[Ch_1] < Ch_th[Ch_1])  ++spare[1];

	  amp_max_Ch1 = ampMax[Ch_1];
	  charge_Ch1 = intSignal[Ch_1];
	  baseline_Ch1 = intBase[Ch_1];

	  //Ch2
	  if(intSignal[Ch_2] < Ch_th[Ch_2]){
	    ++count[2];
	    coinc_Ch2 = timeCF[Ch_ref1] - timeCF[Ch_2];
	  }	    
	  if(intBase[Ch_2] < Ch_th[Ch_2]) ++spare[2];
	  
	  time_Ch2 = timeCF[Ch_ref1] - timeCF[Ch_2];
	  charge_Ch2 = intSignal[Ch_2];
	  amp_max_Ch2 = ampMax[Ch_2];
	  baseline_Ch2 = intBase[Ch_2];
            
	  //Ch3
	  if(intSignal[Ch_3] < Ch_th[Ch_3]){
	    ++count[3];
	    coinc_Ch3 = timeCF[Ch_ref1] - timeCF[Ch_3];
	  }
	  
	  if(intBase[Ch_3] < Ch_th[Ch_3]) ++spare[3];
	  
	  time_Ch3 = timeCF[Ch_ref1] - timeCF[Ch_3];
	  charge_Ch3 = intSignal[Ch_3];
	  amp_max_Ch3 = ampMax[Ch_3];
	  baseline_Ch3 = intBase[Ch_3];

	  run_id = iRun;
	  outTree->Fill();    
	} // Coincidence && trig 1
	
	//Fake rate with multiplicity == 0
	if(intSignal[Ch_ref1] >= Ch_th[Ch_ref1] && intSignal[Ch_ref2] >= Ch_th[Ch_ref2] && trig==0){
	  ++tot_tr0;
	  if(intSignal[Ch_1] < Ch_th[Ch_1]) ++spare2[1];
	  if(intSignal[Ch_2] < Ch_th[Ch_2]) ++spare2[2];
	  if(intSignal[Ch_3] < Ch_th[Ch_3]) ++spare2[3];
	}
      }// good events
    } //loop over entries


    //Info
    std::cout << "--------------------------" << std::endl;
    std::cout << "number of events:  " << chain->GetEntries()/10 << std::endl;
    std::cout << "Double:  " << tot_tr1 << std::endl;
    std::cout << "No e- :  " << tot_tr0 << std::endl;
    std::cout << "--------------------------" << std::endl;
    std::cout << "Ch_ref1: " << Ch_ref1 << " Ch_ref2: " << Ch_ref2 << std::endl;
    std::cout << "Measuring Eff for => Ch_1: " << Ch_1 << " Ch_2: " << Ch_2 << " Ch_3: " << Ch_3 << std::endl;
    std::cout << "Ch_1:  " << count[1] << "  " << spare[1] << "  " << spare2[1] << std::endl;
    std::cout << "Ch_2:  " << count[2] << "  " << spare[2] << "  " << spare2[2] << std::endl;
    std::cout << "Ch_3:  " << count[3] << "  " << spare[3] << "  " << spare2[3] << std::endl;
    std::cout << "--------------------------" << std::endl;
    std::cout << "HV1 = " << HV1 << " HV2 = " << HV2 << " HV3 = " << HV3 << std::endl;
    std::cout << "--------------------------" << std::endl;
    double eff1 = ((double)count[1]-(double)spare[1])/(double)tot_tr1;
    double eff2 = ((double)count[2]-(double)spare[2])/(double)tot_tr1;
    double eff3 = ((double)count[3]-(double)spare[3])/(double)tot_tr1;

    double eff1E = TMath::Sqrt((eff1*(1-eff1))/tot_tr1);
    double eff2E = TMath::Sqrt((eff2*(1-eff2))/tot_tr1);
    double eff3E = TMath::Sqrt((eff3*(1-eff3))/tot_tr1);

    std::cout << "Ch_1 eff = " << eff1 << " +/- " << eff1E << std::endl;
    std::cout << "Ch_2 eff = " << eff2 << " +/- " << eff2E << std::endl;
    std::cout << "Ch_3 eff = " << eff3 << " +/- " << eff3E << std::endl;

    // Output dat
    float adj_x0=1;
    if(TString(tokens_name[0]).Contains("Cu")) adj_x0=0.64;
    data1 << HV1*adj_x0 << " " <<  eff1 << " " << 0 << " " << eff1E << std::endl;
    data2 << HV2*adj_x0 << " " <<  eff2 << " " << 0 << " " << eff2E << std::endl;
    data3 << HV3*adj_x0 << " " << eff3 << " " << 0 << " " << eff3E << std::endl;


    chain->Delete();
  }

  data1.close();
  data2.close();
  data3.close();

  outTree->Write();
  outROOT->Close();
  std::cout << " FINE " << std::endl;
}

        
