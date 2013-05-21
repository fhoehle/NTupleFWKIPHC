makeMCPUdistr(){

TFile * myFile = new TFile("PU3DMC.root", "recreate");
myFile->cd();


TH1D *  histoMCPU = new TH1D("histoMCPU", "histoMCPU", 25, -0.5, 24.5 );

Double_t probdistFlat10[25] = {
    0.0698146584,
    0.0698146584,
    0.0698146584,
    0.0698146584,
    0.0698146584,
    0.0698146584,
    0.0698146584,
    0.0698146584,
    0.0698146584,
    0.0698146584,
    0.0698146584,
    0.0630151648,
    0.0526654164,
    0.0402754482,
    0.0292988928,
    0.0194384503,
    0.0122016783,
    0.007207042,
    0.004003637,
    0.0020278322,
    0.0010739954,
    0.0004595759,
    0.0002229748,
    0.0001028162,
    4.58337152809607E-05
  };
  
  for(unsigned int i=1; i<=25; i++){
  
  histoMCPU->SetBinContent(i,probdistFlat10[i-1] );
  
  
  }
  histoMCPU->Write();


}
