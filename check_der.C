void check_der(int layer = 0, int strip = 76, int event = 2){
  gStyle->SetOptStat(0);
  gSystem->Load("libPiGaudiData.so");
  TFile *file = new TFile("der.root");
  TTree *der = (TTree*)file->Get("der");

  TClonesArray *Waveforms = new TClonesArray("PIDERWaveform");
  der->SetBranchAddress("atarwf",&Waveforms);
  der->GetEntry(event-1);

  TH2F *h2 = new TH2F("2d","2d",1024,-256,256,100,0,100);
  TH1F *h1 = new TH1F("1d","1d",1024,-256,256);
  
  for (Int_t i=0;i!=Waveforms->GetEntries(); i++){
    PIDERWaveform *waveform =  (PIDERWaveform*)Waveforms->At(i);// <<std::endl;
  // PIDERWaveform *waveform  = (PIDERWaveform*)(Waveforms[0]);
    int channel = waveform->GetChannel() ;
    int layer_id = (channel -1) /100 % 100;
    int strip_id = (channel -1)%100;

    if (layer_id != layer) continue;
    
    std::vector<Float_t>& amplitudes = waveform->GetAmplitudes();
    for (Int_t j=0;j!= amplitudes.size();j++){
      h2->SetBinContent(j+1,strip_id+1,amplitudes.at(j));
    }
    if (strip_id == strip){
      for (Int_t j=0;j!= amplitudes.size();j++){
	h1->SetBinContent(j+1,amplitudes.at(j));
      }
    }
  }

  TCanvas *c1 = new TCanvas("c1","c1",1200,600);
  c1->Divide(2,1);
  c1->cd(1);
  h2->Draw("colz");
  c1->cd(2);
  h1->Draw();
}
