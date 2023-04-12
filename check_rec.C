void check_rec(int layer = 0, int strip = 76, int event = 2){
  gStyle->SetOptStat(0);
  gSystem->Load("libPiGaudiData.so");
  TFile *file = new TFile("rec.root");
  TTree *rec = (TTree*)file->Get("rec");

  TClonesArray *hits = new TClonesArray("PIRECAtarSimple");
  rec->SetBranchAddress("atar",&hits);
  rec->GetEntry(event-1);

  TH2F *h2 = new TH2F("2d","2d",1024*5,-256,256,100,0,100);
  TH1F *h1 = new TH1F("1d","1d",1024*5,-256,256);
  TH1F *h_reco = new TH1F("reco","reco",1024*5,-256,256);
  
  Int_t nl1 = 0;
  TGraph **g_l1 = new TGraph*[100];
  Int_t nhits = 0;
  TF1 **f1 = new TF1*[100];
  
  for (Int_t i=0;i!=hits->GetEntries(); i++){
    PIRECAtarSimple* hit = (PIRECAtarSimple*)hits->At(i);
    int channel = hit->GetChannel();
    int layer_id = (channel -1) /100 % 100;
    int strip_id = (channel -1)%100;

    if (layer_id != layer) continue;

    std::cout << "Channel: " << channel << " Layer: " << layer_id << " Strip: " << strip_id << std::endl;
    
    std::vector<Float_t>& decon_amplitudes = hit->GetDeconAmplitudes();

    for (Int_t j=0;j!=decon_amplitudes.size();j++){
      h2->SetBinContent(j+1,strip_id+1,decon_amplitudes.at(j));
    }

    if (strip_id == strip){
      const std::vector<std::vector<Float_t> >& l1_times = hit->GetL1Times();
      std::vector<std::vector<Float_t> >& l1_amplitudes = hit->GetL1Amplitudes();
      nl1 = l1_times.size();
      for (size_t j=0;j!=l1_times.size();j++){
	g_l1[j] = new TGraph();
	for (size_t k=0;k!=l1_times.at(j).size();k++){
	  g_l1[j]->SetPoint(k,l1_times.at(j).at(k), l1_amplitudes.at(j).at(k));
	}
      }

      const std::vector<Float_t>& heights = hit->Get_Gauss_Peak_Height();
      const std::vector<Float_t>& poss = hit->Get_Gauss_Peak_Pos();
      const std::vector<Float_t>& widths = hit->Get_Gauss_Peak_Width();
      const std::vector<Float_t>& areas = hit->Get_Gauss_Area();
      const std::vector<Float_t>& T0s = hit->Get_L1_T0();
      const std::vector<Float_t>& l1_areas = hit->Get_L1_Area();

      const std::vector<Float_t>& chi2 = hit->Get_Chi2();
      const std::vector<Float_t>& dchi2 = hit->Get_dChi2();
      
      
      for (Int_t j=0;j!=heights.size();j++){
	f1[j] = new TF1(Form("f_%d",j),"gaus",poss[j]-50,poss[j]+50);
	Double_t par[3];
	par[0] = heights[j];
	par[1] = poss[j];
	par[2] = widths[j];
	f1[j]->SetParameters(par);
	std::cout << par[1] << " " << T0s[j] << " " << areas[j] << " " << l1_areas[j] << " " << chi2[j] << " " << dchi2[j] << std::endl;
	nhits ++;
      }

      
      
      for (Int_t j=0;j!= decon_amplitudes.size();j++){
	h1->SetBinContent(j+1,decon_amplitudes.at(j));
      }
    }
    
  }

  TCanvas *c1 = new TCanvas("c1","c1",1200,600);
  c1->Divide(2,1);
  c1->cd(1);
  h2->Draw("colz");
  h2->SetXTitle("Time (ns)");
  h2->SetYTitle("Channel");
  h2->SetTitle(Form("Layer: %d",layer));
  c1->cd(2);
  h1->Draw();
  h1->SetLineColor(1);
  h1->GetXaxis()->SetRangeUser(-10,70);
  h1->SetXTitle("Time (ns)");
  h1->SetTitle(Form("Channel: %d",strip));
  for (size_t i=0;i!=nl1;i++){
    g_l1[i]->Draw("Lsame");
    g_l1[i]->SetLineColor(2);
    g_l1[i]->SetLineWidth(2);
  }
  for (size_t i=0;i!=nhits;i++){
    f1[i]->Draw("same");
    f1[i]->SetLineColor(4);
  }
  TLegend *le1 = new TLegend(0.6,0.6,0.89,0.89);
  le1->AddEntry(h1,"Decon. Waveform","l");
  le1->AddEntry(g_l1[0],"L1 fitting waveform","l");
  le1->AddEntry(f1[0],"Hit Reco.","l");
  le1->Draw();
}
