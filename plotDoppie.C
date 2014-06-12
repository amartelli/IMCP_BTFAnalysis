//#include "TH1F.h"

{

  TFile* f1 = new TFile("Scan1_outHistos.root", "read");
  TFile* f2 = new TFile("Scan2_outHistos.root", "read");
  TFile* f3 = new TFile("Scan3_outHistos.root", "read");
  TFile* f4 = new TFile("Scan4_outHistos.root", "read");

  TH1F* h1 = (TH1F*)f1->Get("timeDiffHisto_Double");
  TH1F* h2 = (TH1F*)f2->Get("timeDiffHisto_Double");
  TH1F* h3 = (TH1F*)f3->Get("timeDiffHisto_Double");
  TH1F* h4 = (TH1F*)f4->Get("timeDiffHisto_Double");


  h1->Rebin(4);
  h2->Rebin(4);
  h3->Rebin(4);
  h4->Rebin(4);

  h1->GetXaxis()->SetRangeUser(h1->GetMean()-0.5, h1->GetMean()+0.5);
  h1->GetXaxis()->SetTitle("ns double coincidence");
  h1->SetLineColor(kRed);

  h2->GetXaxis()->SetRangeUser(h2->GetMean()-0.5, h2->GetMean()+0.5);
  h2->GetXaxis()->SetTitle("ns double coincidence");
  h2->SetLineColor(kBlue);

  h3->GetXaxis()->SetRangeUser(h3->GetMean()-0.5, h3->GetMean()+0.5);
  h3->GetXaxis()->SetTitle("ns double coincidence");
  h3->SetLineColor(kGreen+1);

  h4->GetXaxis()->SetRangeUser(h4->GetMean()-0.5, h4->GetMean()+0.5);
  h4->GetXaxis()->SetTitle("ns double coincidence");
  h4->SetLineColor(kCyan+2);

  TLegend* leg = new TLegend(0.88,0.65,0.98,0.85);
  leg->SetFillColor(0);
  leg->SetTextFont(41);
  leg->AddEntry(h1, "scan1","l");
  leg->AddEntry(h2, "scan2","l");
  leg->AddEntry(h3, "scan3","l");
  leg->AddEntry(h4, "scan4","l");

  TF1* gGauss = new TF1("gGauss","[3]+[1]/sqrt(2*TMath::Pi()*[0]*[0])*exp(-(x-[2])*(x-[2])/(2*[0]*[0]))",-50,50);
  gGauss->SetParName(0,"sigma");
  gGauss->SetParName(1,"amplitude");
  gGauss->SetParName(2,"mean");
  gGauss->SetParName(3,"trasl");
  gGauss->SetNpx(1000000);
  gGauss->SetParameters(0.05,100,0.25,0.1);

  gGauss->SetLineColor(kRed);
  h1->Fit("gGauss", "R");

  gGauss->SetParameter(2,h2->GetMean());
  gGauss->SetLineColor(kBlue);
  h2->Fit("gGauss", "R");

  gGauss->SetParameter(2,h3->GetMean());
  gGauss->SetLineColor(kGreen+2);
  h3->Fit("gGauss", "R");

  gGauss->SetParameter(2,h4->GetMean());
  gGauss->SetLineColor(kCyan+2);
  h4->Fit("gGauss", "R");


  TCanvas* c = new TCanvas();
  c->cd();
  h1->Draw();
  h2->Draw("same");
  h3->Draw("same");
  h4->Draw("same");
  leg->Draw("same");


}
