
void drawWF(std::string fileName, int iPos){
  TFile* inF = TFile::Open(fileName.c_str(), "read");
  int colorsExt[9] = {kRed+1, kBlue,  kGreen+2, kAzure+7, kMagenta, kOrange+7, kCyan, kWhite, kBlack};
  std::vector<int> colors;
  for(int posVec = 0; posVec<6; ++posVec){
    colors.push_back(colorsExt[posVec]);
  }

  std::cout << " fileName = " << fileName << std::endl;
  std::cout << " iPos = " << iPos << std::endl;

  int nScan[5] = {16, 10, 10, 11, 2};

  TCanvas** cWF = new TCanvas*[6];  
  TH1F* chHistoWF_All[6][20];

  for(int iw=0; iw<6; ++iw){
    if(iw == 2) continue;
    cWF[iw] = new TCanvas();
    //    std::cout << " itw = " << itw << std::endl;
    for(int iScan=1; iScan<(nScan[iPos]+1); ++iScan){
      //      std::cout << " iScan = " << iScan << std::endl;
      chHistoWF_All[iw][iScan-1] = (TH1F*)inF->Get(Form("histoWF_Ch%d_Scan_%d",iw, iScan));
      //      chHistoWF_All[iw]->SetDirectory(0);

      chHistoWF_All[iw][iScan-1]->SetLineColor(colors.at(iw));
      chHistoWF_All[iw][iScan-1]->SetLineWidth(2);  
      chHistoWF_All[iw][iScan-1]->SetLineStyle(1);  

      cWF[iw]->cd();
      chHistoWF_All[iw][iScan-1]->GetYaxis()->SetRangeUser(0,4096);
      if(iw == 3) chHistoWF_All[iw][iScan-1]->GetYaxis()->SetRangeUser(-4096, 0);
      if(iScan == 1) chHistoWF_All[iw][iScan-1]->Draw();
      else chHistoWF_All[iw][iScan-1]->Draw("same");
    }
  }

  //    return;


  TLegend** leg = new TLegend[6];
  for (int iw=0;iw<6;iw++) {
    if(iw == 2) continue;
    leg[iw] = new TLegend(0.88,0.65,0.98,0.85);
    leg[iw]->SetFillColor(0);
    leg[iw]->SetTextFont(41);

    leg[iw]->AddEntry(chHistoWF_All[iw][0],Form("Ch%d Scan%d",iw,iPos),"l"); 
  }

  for (int iw=0;iw<6;++iw) {
    if(iw == 2) continue;
    cWF[iw]->cd();
    leg[iw]->Draw("same");
  }

}


void compareScan(){
  int colorsExt[9] = {kRed+1, kBlue,  kGreen+2, kAzure+7, kMagenta, kOrange+7, kCyan, kWhite, kBlack};
  std::vector<int> colors;
  for(int posVec = 0; posVec<6; ++posVec){
    colors.push_back(colorsExt[posVec]);
  }

  std::vector<std::string> nameMCP;
  nameMCP.push_back("MiB1");
  nameMCP.push_back("MiB2");
  nameMCP.push_back("ScB");
  nameMCP.push_back("Planacon");
  nameMCP.push_back("MiB3");
  nameMCP.push_back("RomaX");

  std::string PC_CH0[5] = {"ON", "ON", "ON", "ON", "ON"};
  std::string PC_CH1[5] = {"ON", "OFF", "i", "OFF", "OFF"};
  std::string PC_CH3[5] = {"OFF", "ON", "OFF", "OFF", "OFF"};
  std::string PC_CH4[5] = {"OFF", "ON", "ON", "OFF", "OFF"};
  std::string PC_CH5[5] = {"ON", "ON", "ON", "ON", "ON"};



  TCanvas** cBas = new TCanvas*[6];  
  TCanvas** cSig = new TCanvas*[6];  
  
  TH1F* chHistoBase_All[6][5];
  TH1F* chHistoSignal_All[6][5];
  TFile* inF;
  
  for(int iw=0; iw<6; ++iw){
      if(iw == 2) continue;
      cBas[iw] = new TCanvas();
      //      cSig[iw] = new TCanvas();
  }

  for(int iS=0; iS<5; ++iS){
    inF = TFile::Open(Form("Scan%d_outHistos.root",iS+1), "read");
    for(int iw=0; iw<6; ++iw){
      if(iw == 2) continue;

      chHistoBase_All[iw][iS] = (TH1F*)inF->Get(Form("histoBase_All_Ch%d",iw));
      chHistoBase_All[iw][iS]->SetDirectory(0);
      chHistoSignal_All[iw][iS] = (TH1F*)inF->Get(Form("histoSignal_All_Ch%d",iw));
      chHistoSignal_All[iw][iS]->SetDirectory(0);
      //      inF->Close();

      chHistoBase_All[iw][iS]->Rebin(10);
      chHistoBase_All[iw][iS]->Scale(1./chHistoBase_All[iw][iS]->Integral());
      chHistoBase_All[iw][iS]->SetLineColor(colors.at(iw)+2.*iS);
      chHistoBase_All[iw][iS]->SetLineWidth(2);  
      chHistoBase_All[iw][iS]->SetLineStyle(1);  

      chHistoSignal_All[iw][iS]->Rebin(10);
      chHistoSignal_All[iw][iS]->Scale(1./chHistoSignal_All[iw][iS]->Integral());
      chHistoSignal_All[iw][iS]->SetLineColor(colors.at(iw)+2.*iS);
      chHistoSignal_All[iw][iS]->SetLineWidth(2);  
      chHistoSignal_All[iw][iS]->SetLineStyle(1);  

      cBas[iw]->cd();
      if(iS == 0) chHistoBase_All[iw][iS]->Draw();
      else if(iw == 1 && iS == 2) {cBas[5]->cd(); chHistoBase_All[iw][iS]->Draw("same");}
      else chHistoBase_All[iw][iS]->Draw("same");

//       cSig[iw]->cd();
      if(iw == 1 && iS == 2) {cBas[5]->cd(); chHistoSignal_All[iw][iS]->Draw("same");}
      else chHistoSignal_All[iw][iS]->Draw("same");
//      if(iS == 0) chHistoSignal_All[iw][iS]->DrawNormalized();
//       else chHistoSignal_All[iw][iS]->DrawNormalized("same");
    }
  }

  //  return;


  TLegend** leg = new TLegend[6];
  for (int iw=0;iw<6;iw++) {
    if(iw == 2) continue;
    leg[iw] = new TLegend(0.88,0.65,0.98,0.85);
    leg[iw]->SetFillColor(0);
    leg[iw]->SetTextFont(41);
  }

    leg[0]->AddEntry(chHistoBase_All[0][0],(nameMCP.at(0)+PC_CH0[0]+"scan1").c_str(),"l");
    leg[0]->AddEntry(chHistoBase_All[0][1],(nameMCP.at(0)+PC_CH0[1]+"scan2").c_str(),"l");
    leg[0]->AddEntry(chHistoBase_All[0][2],(nameMCP.at(0)+PC_CH0[2]+"scan3").c_str(),"l");
    leg[0]->AddEntry(chHistoBase_All[0][3],(nameMCP.at(0)+PC_CH0[3]+"scan4").c_str(),"l");
    leg[0]->AddEntry(chHistoBase_All[0][4],(nameMCP.at(0)+PC_CH0[4]+"scan5").c_str(),"l");

    leg[1]->AddEntry(chHistoBase_All[1][0],(nameMCP.at(1)+PC_CH1[0]+"scan1").c_str(),"l");
    leg[1]->AddEntry(chHistoBase_All[1][1],(nameMCP.at(1)+PC_CH1[1]+"scan2").c_str(),"l");
    leg[1]->AddEntry(chHistoBase_All[1][3],(nameMCP.at(1)+PC_CH1[3]+"scan4").c_str(),"l");
    leg[1]->AddEntry(chHistoBase_All[1][4],(nameMCP.at(1)+PC_CH1[4]+"scan5").c_str(),"l");
   
    leg[3]->AddEntry(chHistoBase_All[3][0],(nameMCP.at(3)+PC_CH3[0]+"scan1").c_str(),"l");
    leg[3]->AddEntry(chHistoBase_All[3][1],(nameMCP.at(3)+PC_CH3[1]+"scan2").c_str(),"l");
    leg[3]->AddEntry(chHistoBase_All[3][2],(nameMCP.at(3)+PC_CH3[2]+"scan3").c_str(),"l");
    leg[3]->AddEntry(chHistoBase_All[3][3],(nameMCP.at(3)+PC_CH3[3]+"scan4").c_str(),"l");
    leg[3]->AddEntry(chHistoBase_All[3][4],(nameMCP.at(3)+PC_CH3[4]+"scan5").c_str(),"l");

    leg[4]->AddEntry(chHistoBase_All[4][0],(nameMCP.at(4)+PC_CH4[0]+"scan1").c_str(),"l");
    leg[4]->AddEntry(chHistoBase_All[4][1],(nameMCP.at(4)+PC_CH4[1]+"scan2").c_str(),"l");
    leg[4]->AddEntry(chHistoBase_All[4][2],(nameMCP.at(4)+PC_CH4[2]+"scan3").c_str(),"l");
    leg[4]->AddEntry(chHistoBase_All[4][3],(nameMCP.at(4)+PC_CH4[3]+"scan4").c_str(),"l");
    leg[4]->AddEntry(chHistoBase_All[4][4],(nameMCP.at(4)+PC_CH4[4]+"scan5").c_str(),"l");

    leg[5]->AddEntry(chHistoBase_All[5][0],(nameMCP.at(5)+PC_CH5[0]+"scan1").c_str(),"l");
    leg[5]->AddEntry(chHistoBase_All[5][1],(nameMCP.at(5)+PC_CH5[1]+"scan2").c_str(),"l");
    leg[5]->AddEntry(chHistoBase_All[1][2],(nameMCP.at(5)+PC_CH1[2]+"scan3").c_str(),"l"); 
    leg[5]->AddEntry(chHistoBase_All[5][2],(nameMCP.at(5)+PC_CH5[2]+"scan3").c_str(),"l");
    leg[5]->AddEntry(chHistoBase_All[5][3],(nameMCP.at(5)+PC_CH5[3]+"scan4").c_str(),"l");
    leg[5]->AddEntry(chHistoBase_All[5][4],(nameMCP.at(5)+PC_CH5[4]+"scan5").c_str(),"l");


  //  return;

  for (int iw=0;iw<6;++iw) {
    if(iw == 2) continue;
    cBas[iw]->cd();
    leg[iw]->Draw("same");
//     cSig[iw]->cd();
//     leg[iw]->Draw("same");
  }

}



void doPlots_BaseLine(){
  int colorsExt[9] = {kRed+1, kBlue, kBlack, kGreen+2, kAzure+7, kOrange+7, kCyan, kBlack};
  std::vector<int> colors;
  for(int posVec = 0; posVec<7; ++posVec){
    colors.push_back(colorsExt[posVec]);
  }

  std::vector<std::string> nameMCP;
  nameMCP.push_back("MiB1");
  nameMCP.push_back("MiB2");
  nameMCP.push_back("Sci");
  nameMCP.push_back("Planacon");
  nameMCP.push_back("MiB3");
  nameMCP.push_back("Roma_ampli");
  nameMCP.push_back("Roma_Noampli");

  TFile** inF = new TFile[4];

  TH1F* hBaseLineAll[7];
  TH1F* hBaseLineAll_Of[7];
  TH1F* hBaseLineAll_On[7];
  TH1F* hSignalAll[7];
  TH1F* hSignalAll_On[7];
  TH1F* hSignalAll_Of[7];

  TGraph* gContB[7];
  TGraph* gContB_Of[7];
  TGraph* gContB_On[7];
  TGraph* gContS[7];
  TGraph* gContS_On[7];
  TGraph* gContS_Of[7];
  TGraph* gSigmaB[7];

  TGraph* gContSoB[7];
  TGraph* gContSoB_On[7];
  TGraph* gContSoB_Of[7];

  for(int iS=0; iS<4; ++iS){
    std::cout << " >>> iS = " << iS << std::endl;
    inF[iS] = TFile::Open(Form("Scan%d_outHistos.root",iS+1), "read");
    for (int iw=0;iw<7;iw++){
      if(iw == 2) continue;
      gContB[iw] = new TGraph();
      gContB[iw]->SetMarkerColor(colors.at(iw)); 
      gContB[iw]->SetLineColor(colors.at(iw)); 
      gContB[iw]->SetMarkerStyle(21); 
      gContB[iw]->SetMarkerSize(0.6); 

      gContSoB[iw] = new TGraph();
      gContSoB[iw]->SetMarkerColor(colors.at(iw)); 
      gContSoB[iw]->SetLineColor(colors.at(iw)); 
      gContSoB[iw]->SetMarkerStyle(21); 
      gContSoB[iw]->SetMarkerSize(0.6); 

      gContSoB_On[iw] = new TGraph();
      gContSoB_On[iw]->SetMarkerColor(colors.at(iw)); 
      gContSoB_On[iw]->SetLineColor(colors.at(iw)); 
      gContSoB_On[iw]->SetMarkerStyle(21); 
      gContSoB_On[iw]->SetMarkerSize(0.6); 

      gContSoB_Of[iw] = new TGraph();
      gContSoB_Of[iw]->SetMarkerColor(colors.at(iw)); 
      gContSoB_Of[iw]->SetLineColor(colors.at(iw)); 
      gContSoB_Of[iw]->SetMarkerStyle(21); 
      gContSoB_Of[iw]->SetMarkerSize(0.6); 

      gContB_Of[iw] = new TGraph();
      gContB_Of[iw]->SetMarkerColor(colors.at(iw)); 
      gContB_Of[iw]->SetLineColor(colors.at(iw)); 
      gContB_Of[iw]->SetMarkerStyle(21); 
      gContB_Of[iw]->SetMarkerSize(0.6); 

      gContB_On[iw] = new TGraph();
      gContB_On[iw]->SetMarkerColor(colors.at(iw)); 
      gContB_On[iw]->SetLineColor(colors.at(iw)); 
      gContB_On[iw]->SetMarkerStyle(21); 
      gContB_On[iw]->SetMarkerSize(0.6); 

      gContS[iw] = new TGraph();
      gContS[iw]->SetMarkerColor(colors.at(iw)); 
      gContS[iw]->SetLineColor(colors.at(iw)); 
      gContS[iw]->SetMarkerStyle(21); 
      gContS[iw]->SetMarkerSize(0.6); 

      gContS_On[iw] = new TGraph();
      gContS_On[iw]->SetMarkerColor(colors.at(iw)); 
      gContS_On[iw]->SetLineColor(colors.at(iw)); 
      gContS_On[iw]->SetMarkerStyle(21); 
      gContS_On[iw]->SetMarkerSize(0.6); 

      gContS_Of[iw] = new TGraph();
      gContS_Of[iw]->SetMarkerColor(colors.at(iw)); 
      gContS_Of[iw]->SetLineColor(colors.at(iw)); 
      gContS_Of[iw]->SetMarkerStyle(21); 
      gContS_Of[iw]->SetMarkerSize(0.6); 
 
      gSigmaB[iw] = new TGraph();
      gSigmaB[iw]->SetMarkerColor(colors.at(iw)); 
      gSigmaB[iw]->SetLineColor(colors.at(iw)); 
      gSigmaB[iw]->SetMarkerStyle(21); 
      gSigmaB[iw]->SetMarkerSize(0.6); 

      if(iw == 6) continue;
      //      std::cout << " >>> iw = " << iw << std::endl;

      if(iS == 0){ 
	hBaseLineAll[iw] = (TH1F*)inF[iS]->Get(Form("histoBase_All_Ch%d",iw));
	hBaseLineAll[iw]->SetName((nameMCP.at(iw)).c_str());
	hBaseLineAll[iw]->SetLineWidth(2);
	hBaseLineAll[iw]->SetLineColor(colors.at(iw));
	//	std::cout << " >>> if = " << hBaseLineAll[iw]->GetName() << std::endl;
	hSignalAll[iw] = (TH1F*)inF[iS]->Get(Form("histoSignal_All_Ch%d",iw));
	hSignalAll[iw]->SetName((nameMCP.at(iw)).c_str());
	hSignalAll[iw]->SetLineWidth(2);
	hSignalAll[iw]->SetLineColor(colors.at(iw));
	//	std::cout << " >>> if = " << hBaseLineAll[iw]->GetName() << std::endl;

	if(iw == 0 || iw == 1 || iw == 5){
	  hSignalAll_On[iw] = (TH1F*)inF[iS]->Get(Form("histoSignal_All_Ch%d",iw));
	  hSignalAll_On[iw]->SetName((nameMCP.at(iw)).c_str());
	  hSignalAll_On[iw]->SetLineWidth(2);
	  hSignalAll_On[iw]->SetLineColor(colors.at(iw));

	  hBaseLineAll_On[iw] = (TH1F*)inF[iS]->Get(Form("histoBase_All_Ch%d",iw));
	  hBaseLineAll_On[iw]->SetName((nameMCP.at(iw)).c_str());
	  hBaseLineAll_On[iw]->SetLineWidth(2);
	  hBaseLineAll_On[iw]->SetLineColor(colors.at(iw));
	}
	if(iw == 3 || iw == 4){
	  hSignalAll_Of[iw] = (TH1F*)inF[iS]->Get(Form("histoSignal_All_Ch%d",iw));
	  hSignalAll_Of[iw]->SetName((nameMCP.at(iw)).c_str());
	  hSignalAll_Of[iw]->SetLineWidth(2);
	  hSignalAll_Of[iw]->SetLineColor(colors.at(iw));

	  hBaseLineAll_Of[iw] = (TH1F*)inF[iS]->Get(Form("histoBase_All_Ch%d",iw));
	  hBaseLineAll_Of[iw]->SetName((nameMCP.at(iw)).c_str());
	  hBaseLineAll_Of[iw]->SetLineWidth(2);
	  hBaseLineAll_Of[iw]->SetLineColor(colors.at(iw));
	}

      }
      else if(iS == 2 && iw == 1){
	hBaseLineAll[5]->Add((TH1F*)inF[iS]->Get(Form("histoBase_All_Ch%d",iw)),1);
	hBaseLineAll[5]->SetName((nameMCP.at(5)).c_str());
	hBaseLineAll[5]->SetLineWidth(2);
	hBaseLineAll[5]->SetLineColor(colors.at(5));
	//
	hSignalAll[5]->Add((TH1F*)inF[iS]->Get(Form("histoSignal_All_Ch%d",iw)),1);
	hSignalAll[5]->SetName((nameMCP.at(5)).c_str());
	hSignalAll[5]->SetLineWidth(2);
	hSignalAll[5]->SetLineColor(colors.at(5));

 	hSignalAll_Of[5] = (TH1F*)inF[iS]->Get(Form("histoSignal_All_Ch%d",iw)); 
 	hSignalAll_Of[5]->SetName((nameMCP.at(5)).c_str());
 	hSignalAll_Of[5]->SetLineWidth(2);
 	hSignalAll_Of[5]->SetLineColor(colors.at(5));

	hBaseLineAll_Of[5] = (TH1F*)inF[iS]->Get(Form("histoBase_All_Ch%d",iw));
	hBaseLineAll_Of[5]->SetName((nameMCP.at(5)).c_str());
	hBaseLineAll_Of[5]->SetLineWidth(2);
	hBaseLineAll_Of[5]->SetLineColor(colors.at(5));
      }
      else if(iS == 3 && iw == 5){
	hBaseLineAll[6] = (TH1F*)inF[iS]->Get(Form("histoBase_All_Ch%d",iw));
	hBaseLineAll[6]->SetName((nameMCP.at(6)).c_str());
	hBaseLineAll[6]->SetLineWidth(2);
	hBaseLineAll[6]->SetLineColor(colors.at(6));
	//
	hSignalAll[6] = (TH1F*)inF[iS]->Get(Form("histoSignal_All_Ch%d",iw));
	hSignalAll[6]->SetName((nameMCP.at(6)).c_str());
	hSignalAll[6]->SetLineWidth(2);
	hSignalAll[6]->SetLineColor(colors.at(6));

 	hSignalAll_On[6] = (TH1F*)inF[iS]->Get(Form("histoSignal_All_Ch%d",iw));
 	hSignalAll_On[6]->SetName((nameMCP.at(6)).c_str());
 	hSignalAll_On[6]->SetLineWidth(2);
 	hSignalAll_On[6]->SetLineColor(colors.at(6));

 	hBaseLineAll_On[6] = (TH1F*)inF[iS]->Get(Form("histoBase_All_Ch%d",iw));
 	hBaseLineAll_On[6]->SetName((nameMCP.at(6)).c_str());
 	hBaseLineAll_On[6]->SetLineWidth(2);
 	hBaseLineAll_On[6]->SetLineColor(colors.at(6));
      }
      else{
	//	std::cout << " >>> else = " << nameMCP.at(iw) << std::endl;
	hBaseLineAll[iw]->Add((TH1F*)inF[iS]->Get(Form("histoBase_All_Ch%d",iw)),1);
	hBaseLineAll[iw]->SetName((nameMCP.at(iw)).c_str());
	hBaseLineAll[iw]->SetLineWidth(2);
	hBaseLineAll[iw]->SetLineColor(colors.at(iw));
	//
	hSignalAll[iw]->Add((TH1F*)inF[iS]->Get(Form("histoSignal_All_Ch%d",iw)),1);
	hSignalAll[iw]->SetName((nameMCP.at(iw)).c_str());
	hSignalAll[iw]->SetLineWidth(2);
	hSignalAll[iw]->SetLineColor(colors.at(iw));

	if(iS == 1){
	  if(iw == 0 || iw == 5){
	    hSignalAll_On[iw]->Add((TH1F*)inF[iS]->Get(Form("histoSignal_All_Ch%d",iw)),1);
	    hSignalAll_On[iw]->SetName((nameMCP.at(iw)).c_str());
	    hSignalAll_On[iw]->SetLineWidth(2);
	    hSignalAll_On[iw]->SetLineColor(colors.at(iw));

	    hBaseLineAll_On[iw]->Add((TH1F*)inF[iS]->Get(Form("histoBase_All_Ch%d",iw)),1);
	    hBaseLineAll_On[iw]->SetName((nameMCP.at(iw)).c_str());
	    hBaseLineAll_On[iw]->SetLineWidth(2);
	    hBaseLineAll_On[iw]->SetLineColor(colors.at(iw));
	    }
	  if(iw == 3 || iw == 4){
	    hSignalAll_On[iw] = (TH1F*)inF[iS]->Get(Form("histoSignal_All_Ch%d",iw));
	    hSignalAll_On[iw]->SetName((nameMCP.at(iw)).c_str());
	    hSignalAll_On[iw]->SetLineWidth(2);
	    hSignalAll_On[iw]->SetLineColor(colors.at(iw));

	    hBaseLineAll_On[iw] = (TH1F*)inF[iS]->Get(Form("histoBase_All_Ch%d",iw));
	    hBaseLineAll_On[iw]->SetName((nameMCP.at(iw)).c_str());
	    hBaseLineAll_On[iw]->SetLineWidth(2);
	    hBaseLineAll_On[iw]->SetLineColor(colors.at(iw));
	  }
	  if(iw == 1){
	    hSignalAll_Of[iw] = (TH1F*)inF[iS]->Get(Form("histoSignal_All_Ch%d",iw));
	    hSignalAll_Of[iw]->SetName((nameMCP.at(iw)).c_str());
	    hSignalAll_Of[iw]->SetLineWidth(2);
	    hSignalAll_Of[iw]->SetLineColor(colors.at(iw));

	    hBaseLineAll_Of[iw] = (TH1F*)inF[iS]->Get(Form("histoBase_All_Ch%d",iw));
	    hBaseLineAll_Of[iw]->SetName((nameMCP.at(iw)).c_str());
	    hBaseLineAll_Of[iw]->SetLineWidth(2);
	    hBaseLineAll_Of[iw]->SetLineColor(colors.at(iw));
	  }
	}
	if(iS == 2){
	  if(iw == 0 || iw == 4 || iw == 5){
	    hSignalAll_On[iw]->Add((TH1F*)inF[iS]->Get(Form("histoSignal_All_Ch%d",iw)),1);
	    hSignalAll_On[iw]->SetName((nameMCP.at(iw)).c_str());
	    hSignalAll_On[iw]->SetLineWidth(2);
	    hSignalAll_On[iw]->SetLineColor(colors.at(iw));

	    hBaseLineAll_On[iw]->Add((TH1F*)inF[iS]->Get(Form("histoBase_All_Ch%d",iw)),1);
            hBaseLineAll_On[iw]->SetName((nameMCP.at(iw)).c_str());
            hBaseLineAll_On[iw]->SetLineWidth(2);
            hBaseLineAll_On[iw]->SetLineColor(colors.at(iw));
	  }
	  if(iw == 3){
	    hSignalAll_Of[iw]->Add((TH1F*)inF[iS]->Get(Form("histoSignal_All_Ch%d",iw)),1);
	    hSignalAll_Of[iw]->SetName((nameMCP.at(iw)).c_str());
	    hSignalAll_Of[iw]->SetLineWidth(2);
	    hSignalAll_Of[iw]->SetLineColor(colors.at(iw));

	    hBaseLineAll_Of[iw]->Add((TH1F*)inF[iS]->Get(Form("histoBase_All_Ch%d",iw)),1);
	    hBaseLineAll_Of[iw]->SetName((nameMCP.at(iw)).c_str());
	    hBaseLineAll_Of[iw]->SetLineWidth(2);
	    hBaseLineAll_Of[iw]->SetLineColor(colors.at(iw));
	  }
	}
	if(iS == 3){
	  if(iw == 0){
	    hSignalAll_On[iw]->Add((TH1F*)inF[iS]->Get(Form("histoSignal_All_Ch%d",iw)),1);
	    hSignalAll_On[iw]->SetName((nameMCP.at(iw)).c_str());
	    hSignalAll_On[iw]->SetLineWidth(2);
	    hSignalAll_On[iw]->SetLineColor(colors.at(iw));

	    hBaseLineAll_On[iw]->Add((TH1F*)inF[iS]->Get(Form("histoBase_All_Ch%d",iw)),1);
	    hBaseLineAll_On[iw]->SetName((nameMCP.at(iw)).c_str());
	    hBaseLineAll_On[iw]->SetLineWidth(2);
	    hBaseLineAll_On[iw]->SetLineColor(colors.at(iw));
	  }
	  if(iw == 1 || iw == 3 || iw == 4){
	    hSignalAll_Of[iw]->Add((TH1F*)inF[iS]->Get(Form("histoSignal_All_Ch%d",iw)),1);
	    hSignalAll_Of[iw]->SetName((nameMCP.at(iw)).c_str());
	    hSignalAll_Of[iw]->SetLineWidth(2);
	    hSignalAll_Of[iw]->SetLineColor(colors.at(iw));

	    hBaseLineAll_Of[iw]->Add((TH1F*)inF[iS]->Get(Form("histoBase_All_Ch%d",iw)),1);
	    hBaseLineAll_Of[iw]->SetName((nameMCP.at(iw)).c_str());
	    hBaseLineAll_Of[iw]->SetLineWidth(2);
	    hBaseLineAll_Of[iw]->SetLineColor(colors.at(iw));
	  }
	}
      }
    }
  }

  for(int iw=0;iw<7;iw++){
    std::cout << "iw = " << iw << std::endl;
    if(iw == 2 ) continue;
    std::cout << " hSignalAll_On[iw]->GetEntries() = " << hSignalAll_On[iw]->GetEntries() << std::endl; 
    std::cout << " hBaseLineAll_On[iw]->GetEntries() = " << hBaseLineAll_On[iw]->GetEntries() << std::endl; 
    if( iw == 0 || iw == 6) continue;
    std::cout << " hSignalAll_Of[iw]->GetEntries() = " << hSignalAll_Of[iw]->GetEntries() << std::endl; 
    std::cout << " hBaseLineAll_Of[iw]->GetEntries() = " << hBaseLineAll_Of[iw]->GetEntries() << std::endl; 
  }
  std::cout << " FINE " << std::endl;
 
  TLegend* leg = new TLegend(0.88,0.65,0.98,0.85);
  leg->SetFillColor(0);
  leg->SetTextFont(41);
  for (int iw=0;iw<7;iw++) {
    if(iw == 2) continue;
    leg->AddEntry(hBaseLineAll[iw],nameMCP.at(iw).c_str(),"l");

  }

 
  TCanvas* c = new TCanvas();
  hBaseLineAll[1]->Draw();
  hSignalAll[1]->SetLineStyle(2);
  hSignalAll[1]->Draw("same");

  TLine* tl = new TLine(-180., 0., -180, 100);
  tl->SetLineColor(kRed);
  tl->Draw("same");

  TLegend* leg2 = new TLegend(0.88,0.65,0.98,0.85);
  leg2->SetFillColor(0);
  leg2->SetTextFont(41);
  leg2->SetTextSize(0.04);
  leg2->AddEntry(hBaseLineAll[1], "Mib2 baseline", "l");
  leg2->AddEntry(hSignalAll[1], "Mib2 signal", "l");
  leg2->Draw("same");

//   TCanvas* c = new TCanvas();
//   hBaseLineAll[0]->Draw();
//   for (int iw=1;iw<7;iw++){
//     if(iw == 2) continue;
//     hBaseLineAll[iw]->Draw("same");
//     hSignalAll[iw]->Draw("same");
//   }
//   leg->Draw("same");


  int binMax = 500;
  int binSub = 450;
  for(int iBin=0; iBin<binMax; ++iBin){
    for (int iw=0;iw<7;iw++){
      if(iw == 2) continue;
      double fake = -1;
      double num = hBaseLineAll[iw]->Integral(1, hBaseLineAll[iw]->FindBin(iBin-binSub));
      double den = hBaseLineAll[iw]->Integral(1, hBaseLineAll[iw]->GetNbinsX());
      if(den != 0) fake = num/den;
      gContB[iw]->SetPoint(iBin, iBin-binSub, fake);

      double fakeS = -1;
      num = hSignalAll[iw]->Integral(1, hSignalAll[iw]->FindBin(iBin-binSub));
      den = hSignalAll[iw]->Integral(1, hSignalAll[iw]->GetNbinsX());
      if(den != 0) fakeS = num/den;
      gContS[iw]->SetPoint(iBin, iBin-binSub, fakeS);

      if(fake != 0) gContSoB[iw]->SetPoint(iBin, iBin-binSub, fakeS/fake);
      //
      
      fakeS = -1;
      num = hSignalAll_On[iw]->Integral(1, hSignalAll_On[iw]->FindBin(iBin-binSub));
      den = hSignalAll_On[iw]->Integral(1, hSignalAll_On[iw]->GetNbinsX());
      if(den != 0) fakeS = num/den;
      gContS_On[iw]->SetPoint(iBin, iBin-binSub, fakeS);
      //

      fake = -1;
      num = hBaseLineAll_On[iw]->Integral(1, hBaseLineAll_On[iw]->FindBin(iBin-binSub));
      den = hBaseLineAll_On[iw]->Integral(1, hBaseLineAll_On[iw]->GetNbinsX());
      if(den != 0) fake = num/den;
      gContB_On[iw]->SetPoint(iBin, iBin-binSub, fake);
      //

      if(fake != 0) gContSoB_On[iw]->SetPoint(iBin, iBin-binSub, fakeS/fake);


      if(iw != 0 && iw != 6){
	fakeS = -1;
	num = hSignalAll_Of[iw]->Integral(1, hSignalAll_Of[iw]->FindBin(iBin-binSub));
	den = hSignalAll_Of[iw]->Integral(1, hSignalAll_Of[iw]->GetNbinsX());
	if(den != 0) fakeS = num/den;
	gContS_Of[iw]->SetPoint(iBin, iBin-binSub, fakeS);

	fake = -1;
	num = hBaseLineAll_Of[iw]->Integral(1, hBaseLineAll_Of[iw]->FindBin(iBin-binSub));
	den = hBaseLineAll_Of[iw]->Integral(1, hBaseLineAll_Of[iw]->GetNbinsX());
	if(den != 0) fake = num/den;
	gContB_Of[iw]->SetPoint(iBin, iBin-binSub, fake);

	if(fake != 0) gContSoB_Of[iw]->SetPoint(iBin, iBin-binSub, fakeS/fake);
      }


      //      std::cout << " >>> iw = " << iw << std::endl;
      //      std::cout << " >>> iBin = " << iBin << std::endl;

      TF1 *gGauss = new TF1("gGauss","[1]/sqrt(2*TMath::Pi()*[0]*[0])*exp(-(x-[2])*(x-[2])/(2*[0]*[0]))",-100,100);
      gGauss->SetParName(0,"sigma");
      gGauss->SetParName(1,"amplitude");
      gGauss->SetParName(2,"mean");
      gGauss->SetNpx(10000);
      gGauss->SetParameters(2., 500., 0.);
      hBaseLineAll[iw]->Fit("gGauss", "RQN");

      gSigmaB[iw]->SetPoint(iBin, iBin-binSub, (-1.*float(iBin-binSub)-gGauss->GetParameter(2))/gGauss->GetParameter(0));
      gGauss->Delete();
    }
  }

  std::cout << " >>> OK " << std::endl;

  TCanvas* cB = new TCanvas();
  gPad->SetGrid();
  gContB[0]->GetXaxis()->SetTitle("Integral (ADC)");
  gContB[0]->GetYaxis()->SetTitle("B contamination");
  //  gCont_SoverB_on[0]->GetXaxis()->SetRangeUser(-300,0.);
  gContB[0]->GetYaxis()->SetRangeUser(0.,1.);
  gContB[0]->Draw("apl");
  for (int iw=1;iw<7;iw++){
    if(iw == 2) continue;
    gContB[iw]->Draw("pl,same");
  }
  leg->Draw("same");

  TCanvas* cS = new TCanvas();
  gPad->SetGrid();
  gContS[0]->GetXaxis()->SetTitle("Integral (ADC)");
  gContS[0]->GetYaxis()->SetTitle("S accepted");
  gContS[0]->GetYaxis()->SetRangeUser(0.,1.);
  gContS[0]->Draw("apl");
  for (int iw=1;iw<7;iw++){
    if(iw == 2) continue;
    gContS[iw]->Draw("pl,same");
  }
  leg->Draw("same");

  TCanvas* cSoB = new TCanvas();
  gPad->SetGrid();
  gContSoB[0]->GetXaxis()->SetTitle("Integral (ADC)");
  gContSoB[0]->GetYaxis()->SetTitle("SoB accepted");
  gContSoB[0]->GetYaxis()->SetRangeUser(0.,1.);
  gContSoB[0]->Draw("apl");
  for (int iw=1;iw<7;iw++){
    if(iw == 2) continue;
    gContSoB[iw]->Draw("pl,same");
  }
  leg->Draw("same");


  TCanvas* cS_On = new TCanvas();                                  
  gPad->SetGrid();                                                 
  gContS_On[0]->GetXaxis()->SetTitle("Integral (ADC)");            
  gContS_On[0]->GetYaxis()->SetTitle("S accepted (PC on)");        
  gContS_On[0]->GetYaxis()->SetRangeUser(0.,1.);      
  gContS_On[0]->Draw("apl");                                       
  for (int iw=1;iw<7;iw++){                                        
    if(iw == 2) continue;                                          
    gContS_On[iw]->Draw("pl,same");                                
  }                                                                
  leg->Draw("same");                                              

  TCanvas* cB_On = new TCanvas();                                  
  gPad->SetGrid();                                                 
  gContB_On[0]->GetXaxis()->SetTitle("Integral (ADC)");            
  gContB_On[0]->GetYaxis()->SetTitle("B contamination (PC on)");        
  gContB_On[0]->GetYaxis()->SetRangeUser(0.,500.);      
  gContB_On[0]->Draw("apl");                                       
  for (int iw=1;iw<7;iw++){                                        
    if(iw == 2) continue;                                          
    gContB_On[iw]->Draw("pl,same");                                
  }                                                                
  leg->Draw("same");                                               

  TCanvas* cSoB_On = new TCanvas();                                  
  gPad->SetGrid();                                                 
  gContSoB_On[0]->GetXaxis()->SetTitle("Integral (ADC)");            
  gContSoB_On[0]->GetYaxis()->SetTitle("SoB (PC on)");        
  gContSoB_On[0]->GetYaxis()->SetRangeUser(0.,500.);      
  gContSoB_On[0]->Draw("apl");                                       
  for (int iw=1;iw<7;iw++){                                        
    if(iw == 2) continue;                                          
    gContSoB_On[iw]->Draw("pl,same");                                
  }                                                                
  leg->Draw("same");                                               


  TCanvas* cS_Of = new TCanvas();                                  
  gPad->SetGrid();                                                 
  gContS_Of[1]->GetXaxis()->SetTitle("Integral (ADC)");            
  gContS_Of[1]->GetYaxis()->SetTitle("S accepted (PC off)");       
  gContS_Of[1]->Draw("apl");                                       
  for (int iw=3;iw<6;iw++){                                        
     if(iw == 2) continue;                                          
     gContS_Of[iw]->Draw("pl,same");                                
  }                                                                
  leg->Draw("same");                                               

  TCanvas* cB_Of = new TCanvas();                                  
  gPad->SetGrid();                                                 
  gContB_Of[1]->GetXaxis()->SetTitle("Integral (ADC)");            
  gContB_Of[1]->GetYaxis()->SetTitle("B contamination (PC off)");       
  gContB_Of[1]->Draw("apl");                                       
  for (int iw=3;iw<6;iw++){                                        
     if(iw == 2) continue;                                          
     gContB_Of[iw]->Draw("pl,same");                                
  }                                                                
  leg->Draw("same");                                               

  TCanvas* cSoB_Of = new TCanvas();                                  
  gPad->SetGrid();                                                 
  gContSoB_Of[1]->GetXaxis()->SetTitle("Integral (ADC)");            
  gContSoB_Of[1]->GetYaxis()->SetTitle("SoB (PC off)");       
  gContSoB_Of[1]->Draw("apl");                                       
  for (int iw=3;iw<6;iw++){                                        
     if(iw == 2) continue;                                          
     gContSoB_Of[iw]->Draw("pl,same");                                
  }                                                                
  leg->Draw("same");                                               


  TCanvas* cSB = new TCanvas();
  gPad->SetGrid();
  gSigmaB[0]->GetXaxis()->SetTitle("Integral (ADC)");
  gSigmaB[0]->GetYaxis()->SetTitle("#sigma B");
//   gCont_SoverB_on[0]->GetXaxis()->SetRangeUser(-300,0.);
//   gCont_SoverB_on[0]->GetYaxis()->SetRangeUser(0.,500.);
  gSigmaB[0]->Draw("apl");
  for (int iw=1;iw<7;iw++){
    if(iw == 2) continue;
    gSigmaB[iw]->Draw("pl,same");
  }
  leg->Draw("same");


}

///////////

void doPLots_BaseLine_Envelope(std::string fileName){
  TFile* inF = TFile::Open(fileName.c_str(), "read");

  int colorsExt[9] = {kRed+1, kPink, kBlue, kGreen+2, kAzure+7, kOrange+7, kCyan, kWhite, kBlack};
  std::vector<int> colors;
  for(int posVec = 0; posVec<6; ++posVec){
    colors.push_back(colorsExt[posVec]);
  }

  int nScan = 17; // First
  int nScan = 12; // Last

  std::vector<std::string> nameMCP;
  nameMCP.push_back("MiB1");
  nameMCP.push_back("MiB2");
  nameMCP.push_back("ScB");
  nameMCP.push_back("Planacon");
  nameMCP.push_back("MiB3");
  nameMCP.push_back("RomaX");

  //  TGraph* gCont[6];

  TCanvas** cBs = new TCanvas*[6];
  TCanvas** cCh = new TCanvas*[6];

  TH1F* hIntegral_Bs[9][20];
  TH1F* hIntegral_Ch[9][20];

  char h0[10];
  char ha[10];
  for (int iw=0;iw<6;iw++){
    if(iw == 2)continue;
  //  for (int iw=0;iw<1;iw++){
    cBs[iw] = new TCanvas();
    cCh[iw] = new TCanvas();

    //  for (int iScan=1;iScan<nScan;iw++){
    for (int iScan=1;iScan<10;iScan++){
      sprintf (h0,"IntBs_Ch%d_Scan_%d",iw, iScan);
      hIntegral_Bs[iw][iScan-1] = (TH1F*)inF->Get(h0);
      hIntegral_Bs[iw][iScan-1]->SetDirectory(0);
      hIntegral_Bs[iw][iScan-1]->SetLineColor(colors.at(iw));
      hIntegral_Bs[iw][iScan-1]->GetXaxis()->SetTitle("Bs integral");
      hIntegral_Bs[iw][iScan-1]->SetLineWidth(2);
      hIntegral_Bs[iw][iScan-1]->Rebin(50);

      cBs[iw]->cd();
      if(iScan == 1) hIntegral_Bs[iw][iScan-1]->Draw();
      else hIntegral_Bs[iw][iScan-1]->Draw("same");

      sprintf (ha,"IntCh_Ch%d_Scan_%d",iw, iScan);
      hIntegral_Ch[iw][iScan-1] = (TH1F*)inF->Get(ha);
      hIntegral_Ch[iw][iScan-1]->SetDirectory(0);
      hIntegral_Ch[iw][iScan-1]->SetLineColor(colors.at(iw));
      hIntegral_Ch[iw][iScan-1]->GetXaxis()->SetTitle("Ch integral");
      hIntegral_Ch[iw][iScan-1]->SetLineWidth(2);
      hIntegral_Ch[iw][iScan-1]->Rebin(50);

      cCh[iw]->cd();
      if(iScan == 1) hIntegral_Ch[iw][iScan-1]->Draw();
      else hIntegral_Ch[iw][iScan-1]->Draw("same");
    }
//     gCont[iw] = new TGraph();
//     gCont[iw]->SetMarkerColor(colors.at(iw)); 
//     gCont[iw]->SetLineColor(colors.at(iw)); 
//     gCont[iw]->SetMarkerStyle(21); 
//     gCont[iw]->SetMarkerSize(0.6); 
  }

  return;
  TLegend* leg = new TLegend(0.88,0.65,0.98,0.85);
  leg->SetFillColor(0);
  leg->SetTextFont(41);
  for (int iw=0;iw<6;iw++) {
    if(iw == 2) continue;
    leg->AddEntry(hIntegral_Bs[iw][0],nameMCP.at(iw).c_str(),"l");
  }

  for(int iw=0;iw<6;iw++){
    //for(int iw=0;iw<1;iw++){
    cBs[iw]->cd();
    leg->Draw("same");
    cCh[iw]->cd();
    leg->Draw("same");
  }

  /*
  for(int iBin=0; iBin<80; ++iBin){
    for (int iw=0;iw<6;iw++){
      if(iw == 2) continue;
      double fake = -1;
      double num = hBaseLineAll[iw]->Integral(hBaseLineAll[iw]->FindBin(iBin), hBaseLineAll[iw]->GetNbinsX());
      double den = hBaseLineAll[iw]->Integral(1, hBaseLineAll[iw]->GetNbinsX());
      if(den != 0) fake = num/den;
      
      gCont[iw]->SetPoint(iBin, iBin, fake*100.);
    }
  }
  */
}


/////////////////////////////////////////////////////////////////////////////////

void doPLots_Integral(){

  int colorsExt[9] = {kRed+1, kBlue, kGreen+2, kAzure+7, kOrange+7, kCyan, kBlack};
  std::vector<int> colors;
  for(int posVec = 0; posVec<6; ++posVec){
    colors.push_back(colorsExt[posVec]);
  }

  std::vector<std::string> nameMCP;
  nameMCP.push_back("MiB1");
  nameMCP.push_back("MiB2");
  nameMCP.push_back("Planacon");
  nameMCP.push_back("MiB3");
  nameMCP.push_back("Roma_ampli");
  nameMCP.push_back("Roma_Noampli");

  TFile** inF = new TFile[4];

  TH1F* chHistoBase_All_PCon[6];
  TH1F* chHistoBase_All_PCoff[6];
  TH1F* chHistoSignal_All_PCon[6];
  TH1F* chHistoSignal_All_PCoff[6];


  for(int iS=0; iS<4; ++iS){
    inF[iS] = TFile::Open(Form("Scan%d_outHistos.root",iS+1), "read");
  }
  chHistoBase_All_PCon[0] = (TH1F*)inF[0]->Get(Form("histoBase_All_Ch%d",0));
  chHistoBase_All_PCon[0]->Add((TH1F*)inF[1]->Get(Form("histoBase_All_Ch%d",0)), 1);
  chHistoBase_All_PCon[0]->Add((TH1F*)inF[2]->Get(Form("histoBase_All_Ch%d",0)), 1);
  chHistoBase_All_PCon[0]->Add((TH1F*)inF[3]->Get(Form("histoBase_All_Ch%d",0)), 1);
  chHistoBase_All_PCon[0]->SetName("MiB 1 PC on B");
  chHistoBase_All_PCon[0]->SetLineWidth(2);
  chHistoBase_All_PCon[0]->SetLineColor(colors.at(0));
  std::cout << "chHistoBase_All_PCon[0]->GetEntries() = " << chHistoBase_All_PCon[0]->GetEntries() << std::endl;
  //
  chHistoBase_All_PCon[1] = (TH1F*)inF[0]->Get(Form("histoBase_All_Ch%d",1));
  chHistoBase_All_PCon[1]->SetName("Mib 2 PC on B");
  //   chHistoBase_All_PCon[1]->Add((TH1F*)inF[1]->Get(Form("histoBase_All_Ch%d",4)), 1);
  //   chHistoBase_All_PCon[1]->Add((TH1F*)inF[2]->Get(Form("histoBase_All_Ch%d",4)), 1);
  //  chHistoBase_All_PCon[1]->SetName("Mib 2+3 PC on B");
  chHistoBase_All_PCon[1]->SetLineWidth(2);
  chHistoBase_All_PCon[1]->SetLineColor(colors.at(1));
  std::cout << "chHistoBase_All_PCon[1]->GetEntries() = " << chHistoBase_All_PCon[1]->GetEntries() << std::endl;
  //
  chHistoBase_All_PCoff[1] = (TH1F*)inF[1]->Get(Form("histoBase_All_Ch%d",1));
  chHistoBase_All_PCoff[1]->Add((TH1F*)inF[3]->Get(Form("histoBase_All_Ch%d",1)), 1);
  chHistoBase_All_PCoff[1]->SetName("Mib 2 PC off B");
  //   chHistoBase_All_PCoff[1]->Add((TH1F*)inF[0]->Get(Form("histoBase_All_Ch%d",4)), 1);
  //   chHistoBase_All_PCoff[1]->Add((TH1F*)inF[3]->Get(Form("histoBase_All_Ch%d",4)), 1);
  //   chHistoBase_All_PCoff[1]->SetName("Mib 2+3 PC off B");
  chHistoBase_All_PCoff[1]->SetLineWidth(2);
  chHistoBase_All_PCoff[1]->SetLineColor(colors.at(1));
  std::cout << "chHistoBase_All_PCoff[1]->GetEntries() = " << chHistoBase_All_PCoff[1]->GetEntries() << std::endl;
  //
  //  chHistoBase_All_PCoff[2] = (TH1F*)inF[0]->Get(Form("histoBase_All_Ch%d",3));
  //  chHistoBase_All_PCoff[2]->Add((TH1F*)inF[2]->Get(Form("histoBase_All_Ch%d",3)), 1);
  chHistoBase_All_PCoff[2] = (TH1F*)inF[0]->Get(Form("histoBase_All_Ch%d",3));
  chHistoBase_All_PCoff[2]->Add((TH1F*)inF[2]->Get(Form("histoBase_All_Ch%d",3)), 1);
  chHistoBase_All_PCoff[2]->Add((TH1F*)inF[3]->Get(Form("histoBase_All_Ch%d",3)), 1);
  chHistoBase_All_PCoff[2]->SetName("Plana PC off B");
  chHistoBase_All_PCoff[2]->SetLineWidth(2);
  chHistoBase_All_PCoff[2]->SetLineColor(colors.at(2));
  std::cout << "chHistoBase_All_PCoff[2]->GetEntries() = " << chHistoBase_All_PCoff[2]->GetEntries() << std::endl;
  //
  chHistoBase_All_PCon[2] = (TH1F*)inF[1]->Get(Form("histoBase_All_Ch%d",3));
  chHistoBase_All_PCon[2]->SetName("Plana PC on B");
  chHistoBase_All_PCon[2]->SetLineWidth(2);
  chHistoBase_All_PCon[2]->SetLineColor(colors.at(2));
  std::cout << "chHistoBase_All_PCon[2]->GetEntries() = " << chHistoBase_All_PCon[2]->GetEntries() << std::endl;
  //
  chHistoBase_All_PCoff[3] = (TH1F*)inF[0]->Get(Form("histoBase_All_Ch%d",4));
  chHistoBase_All_PCoff[3]->Add((TH1F*)inF[3]->Get(Form("histoBase_All_Ch%d",4)), 1);
  chHistoBase_All_PCoff[3]->SetName("Mib 3 PC off B");
  chHistoBase_All_PCoff[3]->SetLineWidth(2);
  chHistoBase_All_PCoff[3]->SetLineColor(colors.at(3));
  std::cout << "chHistoBase_All_PCoff[3]->GetEntries() = " << chHistoBase_All_PCoff[3]->GetEntries() << std::endl;
  //
  chHistoBase_All_PCon[3] = (TH1F*)inF[1]->Get(Form("histoBase_All_Ch%d",4));
  chHistoBase_All_PCon[3]->Add((TH1F*)inF[2]->Get(Form("histoBase_All_Ch%d",4)), 1);
  chHistoBase_All_PCon[3]->SetName("MiB 3 PC on B");
  chHistoBase_All_PCon[3]->SetLineWidth(2);
  chHistoBase_All_PCon[3]->SetLineColor(colors.at(3));
  std::cout << "chHistoBase_All_PCon[3]->GetEntries() = " << chHistoBase_All_PCon[3]->GetEntries() << std::endl;
  //
  chHistoBase_All_PCon[4] = (TH1F*)inF[0]->Get(Form("histoBase_All_Ch%d",5));
  chHistoBase_All_PCon[4]->Add((TH1F*)inF[1]->Get(Form("histoBase_All_Ch%d",5)), 1);
  chHistoBase_All_PCon[4]->Add((TH1F*)inF[2]->Get(Form("histoBase_All_Ch%d",5)), 1);
  chHistoBase_All_PCon[4]->SetName("Roma PC on B");
  chHistoBase_All_PCon[4]->SetLineWidth(2);
  chHistoBase_All_PCon[4]->SetLineColor(colors.at(4));
  std::cout << "chHistoBase_All_PCon[4]->GetEntries() = " << chHistoBase_All_PCon[4]->GetEntries() << std::endl;
  //
  chHistoBase_All_PCoff[4] = (TH1F*)inF[3]->Get(Form("histoBase_All_Ch%d",1));
  chHistoBase_All_PCoff[4]->SetName("Roma PC off B");
  chHistoBase_All_PCoff[4]->SetLineWidth(2);
  chHistoBase_All_PCoff[4]->SetLineColor(colors.at(4));
  std::cout << "chHistoBase_All_PCoff[4]->GetEntries() = " << chHistoBase_All_PCoff[4]->GetEntries() << std::endl;
  //
  chHistoBase_All_PCon[5] = (TH1F*)inF[3]->Get(Form("histoBase_All_Ch%d",5));
  chHistoBase_All_PCon[5]->SetName("Roma PC on noAmpli B");
  chHistoBase_All_PCon[5]->SetLineWidth(2);
  chHistoBase_All_PCon[5]->SetLineColor(colors.at(5));
  std::cout << "chHistoBase_All_PCon[5]->GetEntries() = " << chHistoBase_All_PCon[5]->GetEntries() << std::endl;
  ////////////////
  chHistoSignal_All_PCon[0] = (TH1F*)inF[0]->Get(Form("histoSignal_All_Ch%d",0));
  chHistoSignal_All_PCon[0]->Add((TH1F*)inF[1]->Get(Form("histoSignal_All_Ch%d",0)), 1);
  chHistoSignal_All_PCon[0]->Add((TH1F*)inF[2]->Get(Form("histoSignal_All_Ch%d",0)), 1);
  chHistoSignal_All_PCon[0]->Add((TH1F*)inF[3]->Get(Form("histoSignal_All_Ch%d",0)), 1);
  chHistoSignal_All_PCon[0]->SetName("MiB 1 PC on S ");
  chHistoSignal_All_PCon[0]->SetLineWidth(2);
  chHistoSignal_All_PCon[0]->SetLineColor(colors.at(0));
  std::cout << "chHistoSignal_All_PCon[0]->GetEntries() = " << chHistoSignal_All_PCon[0]->GetEntries() << std::endl;
  //  //  chHistoSignal_All_PCon[1] = (TH1F*)inF[0]->Get(Form("histoSignal_All_Ch%d",1));
  chHistoSignal_All_PCon[1] = (TH1F*)inF[0]->Get(Form("histoSignal_Ch%d_Scan_%d",1,9));   // Max 1 Min 9
  chHistoSignal_All_PCon[1]->SetName("Mib 2 PC on S");
  //   chHistoSignal_All_PCon[1]->Add((TH1F*)inF[1]->Get(Form("histoSignal_Ch%d_Scan_%d",4,10)),1);
  //   chHistoSignal_All_PCon[1]->Add((TH1F*)inF[2]->Get(Form("histoSignal_All_Ch%d",4)), 1);
  //   chHistoSignal_All_PCon[1]->SetName("Mib 2+3 PC on S");
  chHistoSignal_All_PCon[1]->SetLineWidth(2);
  chHistoSignal_All_PCon[1]->SetLineColor(colors.at(1));
  std::cout << "chHistoSignal_All_PCon[1]->GetEntries() = " << chHistoSignal_All_PCon[1]->GetEntries() << std::endl;
  //
  //  chHistoSignal_All_PCoff[1] = (TH1F*)inF[1]->Get(Form("histoSignal_All_Ch%d",1));
  //  chHistoSignal_All_PCoff[1]->Add((TH1F*)inF[3]->Get(Form("histoSignal_All_Ch%d",1)), 1);
  chHistoSignal_All_PCoff[1] = (TH1F*)inF[1]->Get(Form("histoSignal_Ch%d_Scan_%d",1,10));   // Max 1 Min 10
  chHistoSignal_All_PCoff[1]->Add((TH1F*)inF[3]->Get(Form("histoSignal_Ch%d_Scan_%d",1,1)), 1);   // Max 11 Min 1
  chHistoSignal_All_PCoff[1]->SetName("Mib 2 PC off S");
  //   chHistoSignal_All_PCoff[1]->Add((TH1F*)inF[0]->Get(Form("histoSignal_Ch%d_Scan_%d",4,8)), 1);
  //   chHistoSignal_All_PCoff[1]->Add((TH1F*)inF[3]->Get(Form("histoSignal_Ch%d_Scan_%d",4,1)), 1);
  //   chHistoSignal_All_PCoff[1]->SetName("Mib 2+3 PC off S");
  chHistoSignal_All_PCoff[1]->SetLineWidth(2);
  chHistoSignal_All_PCoff[1]->SetLineColor(colors.at(1));
  std::cout << "chHistoSignal_All_PCoff[1]->GetEntries() = " << chHistoSignal_All_PCoff[1]->GetEntries() << std::endl;
  //
  //   chHistoSignal_All_PCoff[2] = (TH1F*)inF[0]->Get(Form("histoSignal_All_Ch%d",3));
  //   chHistoSignal_All_PCoff[2]->Add((TH1F*)inF[2]->Get(Form("histoSignal_All_Ch%d",3)), 1);
  //   chHistoSignal_All_PCoff[2]->Add((TH1F*)inF[3]->Get(Form("histoSignal_All_Ch%d",3)), 1);
  chHistoSignal_All_PCoff[2] = (TH1F*)inF[0]->Get(Form("histoSignal_Ch%d_Scan_%d",3,16));  // Max 1 Min 16
  chHistoSignal_All_PCoff[2]->Add((TH1F*)inF[2]->Get(Form("histoSignal_Ch%d_Scan_%d",3,6)), 1);  // Max 10 Min 6
  chHistoSignal_All_PCoff[2]->Add((TH1F*)inF[3]->Get(Form("histoSignal_Ch%d_Scan_%d",3,1)), 1);  // Max 11 Min 1
  chHistoSignal_All_PCoff[2]->SetName("Plana PC off S");
  std::cout << "chHistoSignal_All_PCoff[2]->GetEntries() = " << chHistoSignal_All_PCoff[2]->GetEntries() << std::endl;
  //
  //  chHistoSignal_All_PCon[2] = (TH1F*)inF[1]->Get(Form("histoSignal_All_Ch%d",3));
  chHistoSignal_All_PCon[2] = (TH1F*)inF[1]->Get(Form("histoSignal_Ch%d_Scan_%d",3,10));  // Max 1 Min 10
  chHistoSignal_All_PCon[2]->SetName("Plana PC on S");
  chHistoSignal_All_PCon[2]->SetLineWidth(2);
  chHistoSignal_All_PCon[2]->SetLineColor(colors.at(2));
  std::cout << "chHistoSignal_All_PCon[2]->GetEntries() = " << chHistoSignal_All_PCon[2]->GetEntries() << std::endl;
  //
  //   chHistoSignal_All_PCoff[3] = (TH1F*)inF[0]->Get(Form("histoSignal_All_Ch%d",4));
  //   chHistoSignal_All_PCoff[3]->Add((TH1F*)inF[3]->Get(Form("histoSignal_All_Ch%d",4)), 1);
  chHistoSignal_All_PCoff[3] = (TH1F*)inF[0]->Get(Form("histoSignal_Ch%d_Scan_%d",4,8));   // Max 16 Min 8
  chHistoSignal_All_PCoff[3]->Add((TH1F*)inF[3]->Get(Form("histoSignal_Ch%d_Scan_%d",4,1)), 1);  // Max 11 Min 1
  chHistoSignal_All_PCoff[3]->SetName("Mib 3 PC off S");
  chHistoSignal_All_PCoff[3]->SetLineWidth(2);
  chHistoSignal_All_PCoff[3]->SetLineColor(colors.at(3));
  std::cout << "chHistoSignal_All_PCoff[3]->GetEntries() = " << chHistoSignal_All_PCoff[3]->GetEntries() << std::endl;
  //
  //   chHistoSignal_All_PCon[3] = (TH1F*)inF[1]->Get(Form("histoSignal_All_Ch%d",4));
  //   chHistoSignal_All_PCon[3]->Add((TH1F*)inF[2]->Get(Form("histoSignal_All_Ch%d",4)), 1);
  chHistoSignal_All_PCon[3] = (TH1F*)inF[1]->Get(Form("histoSignal_Ch%d_Scan_%d",4,10)); // Max 1 Min 10
  chHistoSignal_All_PCon[3]->Add((TH1F*)inF[2]->Get(Form("histoSignal_All_Ch%d",4)), 1);  // tensione fissa
  chHistoSignal_All_PCon[3]->SetName("MiB 3 PC on S");
  chHistoSignal_All_PCon[3]->SetLineWidth(2);
  chHistoSignal_All_PCon[3]->SetLineColor(colors.at(3));
  std::cout << "chHistoSignal_All_PCon[3]->GetEntries() = " << chHistoSignal_All_PCon[3]->GetEntries() << std::endl;
  //
  //   chHistoSignal_All_PCon[4] = (TH1F*)inF[0]->Get(Form("histoSignal_All_Ch%d",5));
  //   chHistoSignal_All_PCon[4]->Add((TH1F*)inF[1]->Get(Form("histoSignal_All_Ch%d",5)), 1);
  //   chHistoSignal_All_PCon[4]->Add((TH1F*)inF[2]->Get(Form("histoSignal_All_Ch%d",5)), 1);
  chHistoSignal_All_PCon[4] = (TH1F*)inF[0]->Get(Form("histoSignal_All_Ch%d",5));
  chHistoSignal_All_PCon[4]->Add((TH1F*)inF[1]->Get(Form("histoSignal_All_Ch%d",5)), 1);
  chHistoSignal_All_PCon[4]->Add((TH1F*)inF[2]->Get(Form("histoSignal_Ch%d_Scan_%d",5,9)), 1); // Max 1 Min 9
  chHistoSignal_All_PCon[4]->SetName("Roma PC on ");
  chHistoSignal_All_PCon[4]->SetLineWidth(2);
  chHistoSignal_All_PCon[4]->SetLineColor(colors.at(4));
  std::cout << "chHistoSignal_All_PCon[4]->GetEntries() = " << chHistoSignal_All_PCon[4]->GetEntries() << std::endl;
  //
  //  chHistoSignal_All_PCoff[4] = (TH1F*)inF[2]->Get(Form("histoSignal_All_Ch%d",1));
  chHistoSignal_All_PCoff[4] = (TH1F*)inF[2]->Get(Form("histoSignal_Ch%d_Scan_%d",1,9)); // Max 1 Min 9
  chHistoSignal_All_PCoff[4]->SetName("Roma PC off S");
  chHistoSignal_All_PCoff[4]->SetLineWidth(2);
  chHistoSignal_All_PCoff[4]->SetLineColor(colors.at(4));
  std::cout << "chHistoSignal_All_PCoff[4]->GetEntries() = " << chHistoSignal_All_PCoff[4]->GetEntries() << std::endl;
  //
  chHistoSignal_All_PCon[5] = (TH1F*)inF[3]->Get(Form("histoSignal_All_Ch%d",5));
  chHistoSignal_All_PCon[5]->SetName("Roma PC on noAmpli S");
  chHistoSignal_All_PCon[5]->SetLineWidth(2);
  chHistoSignal_All_PCon[5]->SetLineColor(colors.at(5));
  std::cout << "chHistoSignal_All_PCon[5]->GetEntries() = " << chHistoSignal_All_PCon[5]->GetEntries() << std::endl;

  TGraph* gCont_SoverB_on[6];
  TGraph* gCont_SoverB_off[6];

  for(int iw=0; iw<6; ++iw){
      gCont_SoverB_on[iw] = new TGraph();
      gCont_SoverB_on[iw]->SetMarkerColor(colors.at(iw)); 
      gCont_SoverB_on[iw]->SetLineColor(colors.at(iw)); 
      gCont_SoverB_on[iw]->SetMarkerStyle(21); 
      gCont_SoverB_on[iw]->SetMarkerSize(0.6); 

      gCont_SoverB_off[iw] = new TGraph();
      gCont_SoverB_off[iw]->SetMarkerColor(colors.at(iw)); 
      gCont_SoverB_off[iw]->SetLineColor(colors.at(iw)); 
      gCont_SoverB_off[iw]->SetMarkerStyle(21); 
      gCont_SoverB_off[iw]->SetMarkerSize(0.6); 
  }

  TLegend* leg = new TLegend(0.88,0.65,0.98,0.85);
  leg->SetFillColor(0);
  leg->SetTextFont(41);
  for (int iw=0;iw<6;iw++) {
    leg->AddEntry(chHistoBase_All_PCon[iw],nameMCP.at(iw).c_str(),"l");
  }

  int binMax = 5000;
  int binSub = 4500;
  for(int iBin=0; iBin<binMax; ++iBin){
    for (int iw=0;iw<6;iw++){

      double ratioB = -1;
      double numB = chHistoBase_All_PCon[iw]->Integral(1, chHistoBase_All_PCon[iw]->FindBin(iBin - binSub));
      double denB = chHistoBase_All_PCon[iw]->Integral(1, chHistoBase_All_PCon[iw]->GetNbinsX());
      if(denB != 0) ratioB = numB/denB;

      double ratioS = -1;
      double numS = chHistoSignal_All_PCon[iw]->Integral(1, chHistoSignal_All_PCon[iw]->FindBin(iBin - binSub));
      double denS = chHistoSignal_All_PCon[iw]->Integral(1, chHistoSignal_All_PCon[iw]->GetNbinsX());
      if(denS != 0) ratioS = numS/denS;

      if(ratioB != 0 && denB != -1) {
	gCont_SoverB_on[iw]->SetPoint(iBin, iBin-binSub, ratioS/ratioB);
      }
      else gCont_SoverB_on[iw]->SetPoint(iBin, iBin-binSub, -1);

      //      std::cout << " ON ch " << nameMCP.at(iw) << " ratioB = " << ratioB << " ratioS = " << ratioS << std::endl;
      ////// OFF
	ratioB = -1;
	ratioS = -1;
      if(iw != 0 && iw != 5){
	numB = chHistoBase_All_PCoff[iw]->Integral(1, chHistoBase_All_PCoff[iw]->FindBin(iBin - binSub));
	denB = chHistoBase_All_PCoff[iw]->Integral(1, chHistoBase_All_PCoff[iw]->GetNbinsX());
	if(denB != 0) ratioB = numB/denB;
	
	numS = chHistoSignal_All_PCoff[iw]->Integral(1, chHistoSignal_All_PCoff[iw]->FindBin(iBin - binSub));
	denS = chHistoSignal_All_PCoff[iw]->Integral(1, chHistoSignal_All_PCoff[iw]->GetNbinsX());
	if(denS != 0) ratioS = numS/denS;
      }
      if(ratioB != 0 && denB != -1)  gCont_SoverB_off[iw]->SetPoint(iBin, iBin-binSub, ratioS/ratioB);
      else gCont_SoverB_off[iw]->SetPoint(iBin, iBin-binSub, -1);

      //      std::cout << " OFF ch " << nameMCP.at(iw) << " ratioB = " << ratioB << " ratioS = " << ratioS << std::endl;
    }
  }


  TCanvas* cON = new TCanvas();
  chHistoBase_All_PCon[0]->Draw();
  chHistoSignal_All_PCon[0]->Draw("same");
  for (int iw=1;iw<6;iw++){
    chHistoBase_All_PCon[iw]->Draw("same");
    chHistoSignal_All_PCon[iw]->Draw("same");
  }
  //  chHistoBase_All_PCon[5]->Draw();
  //  chHistoSignal_All_PCon[5]->Draw("same");
  leg->Draw("same");


  TCanvas* cOF = new TCanvas();
  chHistoBase_All_PCoff[1]->Draw();
  chHistoSignal_All_PCoff[1]->Draw("same");
  for (int iw=2;iw<5;iw++){
    chHistoBase_All_PCoff[iw]->Draw("same");
    chHistoSignal_All_PCoff[iw]->Draw("same");
  }
  leg->Draw("same");



  TCanvas* cRatio_ON = new TCanvas();
  gPad->SetGrid();
  gCont_SoverB_on[0]->GetXaxis()->SetTitle("Integral (ADC) PC on");
  gCont_SoverB_on[0]->GetYaxis()->SetTitle("S/B");
  gCont_SoverB_on[0]->GetXaxis()->SetRangeUser(-300,0.);
  gCont_SoverB_on[0]->GetYaxis()->SetRangeUser(0.,500.);
  gCont_SoverB_on[0]->Draw("apl");
  for (int iw=1;iw<6;iw++){
    gCont_SoverB_on[iw]->Draw("pl,same");
  }
  leg->Draw("same");

  TCanvas* cRatio_OFF = new TCanvas();
  gPad->SetGrid();
  gCont_SoverB_off[1]->GetXaxis()->SetTitle("Integral (ADC) PC off");
  gCont_SoverB_off[1]->GetYaxis()->SetTitle("S/B");
  gCont_SoverB_off[1]->GetXaxis()->SetRangeUser(-400,0.);
  gCont_SoverB_off[1]->GetYaxis()->SetRangeUser(0.,500.);
  gCont_SoverB_off[1]->Draw("apl");
  for (int iw=2;iw<5;iw++){
    gCont_SoverB_off[iw]->Draw("pl,same");
  }
  leg->Draw("same");

}


void doAltro(){



  int nScan = 16;

  int goodCh[3] = {1, 3, 4}; 

  std::vector<std::string> nameMCP;
  nameMCP.push_back("MiB1");
  nameMCP.push_back("MiB2");
  nameMCP.push_back("ScB");
  nameMCP.push_back("Planacon");
  nameMCP.push_back("MiB3");
  nameMCP.push_back("RomaX");


  TFile* inF = TFile::Open("outHistos.root");
  TGraph* hRatio[9];

  int colorsExt[9] = {kRed+1, kBlue, kPink, kGreen+1, kCyan+2, kOrange+7, kAzure+7, kWhite, kBlack};
  std::vector<int> colors;
  for(int posVec = 0; posVec<6; ++posVec){
    colors.push_back(colorsExt[posVec]);
  }

  TLegend* leg = new TLegend(0.88,0.65,0.98,0.85);
  leg->SetFillColor(0);
  leg->SetTextFont(41);
  for (int iw=0;iw<6;iw++) {
    leg->AddEntry(hRatio[iw],nameMCP.at(iw).c_str(),"l");
  }

  TCanvas* c = new TCanvas();

  TH1F* hAmpMax[9];
  TH1F* hBaseLine[9];

  float thres = 20;

  for(int iSC=1; iSC<=nScan+1; ++iSC){
  //  for(int iSC=1; iSC<=2; ++iSC){
    for(int iCh=0; iCh<6; ++iCh){
      if(iCh == 2) continue;

      char h1[10];
      sprintf (h1,"AM_Ch%d_Scan%d",iCh, iSC);
      hAmpMax[iCh] = (TH1F*)inF->Get("h1");


      char h2[10];
      sprintf (h2,"Bs_Ch%d_Scan%d",iCh, iSC);
      hBaseLine[iCh] = (TH1F*)inF->Get("h2");
    
      std::cout << "preso " << hAmpMax[iCh]->FindBin(thres) << std::endl;
    float ratio = (hAmpMax[iCh]->Integral( hAmpMax[iCh]->FindBin(thres), hAmpMax[iCh]->MaximumBin()) /
		   hBaseLine[iCh]->Integral( hBaseLine[iCh]->FindBin(thres), hBaseLine[iCh]->MaximumBin()) );
    hRatio->SetPoint(iSc, iSc, ratio);
    hRatio[iw]->SetMarkerColor(colors.at(iw));
    
    c->cd();
    if(iScan == 1 && iCh == 0)    hRatio[iw]->Draw("apl");
    else hRatio[iw]->Draw("p,same");
    }
  }
 
  c->cd();
  leg->Draw("same");
}
