{

 gROOT->ProcessLine(".L PlotStack.C+");
 gROOT->SetStyle("Plain");
 gStyle->SetPalette(1);
 gStyle->SetOptStat(0);


 for (int j=0; j<4; j++) {
 // loop over channels
  TString jchan;
  if (j==0) jchan="ee";
  else if (j==1) jchan="mumu";
  else if (j==2) jchan="emu";
  else if (j==3) jchan="all";

  for (int i=0; i<2; i++) {
  //  loop for SetLogy option 

   for (int k=0; k<2; k++) {
    // loop for ratio plot

    // after lepton pair cut
    PlotStack("Njets_",jchan,"", i, k);
    PlotStack("NBjets_",jchan,"", i, k);
    PlotStack("Inv",jchan,"MassPair", i, k);
   // PlotStack("Met_",jchan,"", i, k);
    PlotStack("Met_",jchan,"_afterjetcut", i, k);
    PlotStack("Met_",jchan,"_afterbtag1", i, k);
    PlotStack("Met_",jchan,"_afterbtag2", i, k);
    
    
    PlotStack("PtJet1_", jchan, "_aftermetcut", i, k);
    PlotStack("PtJet2_", jchan, "_aftermetcut", i, k);
    if(j==1){
    PlotStack("PtMu1_", jchan, "_aftermetcut", i, k);
    PlotStack("PtMu2_", jchan, "_aftermetcut", i, k);
    }

    if(j==0){
    PlotStack("PtEl1_", jchan, "_aftermetcut", i, k);
    PlotStack("PtEl2_", jchan, "_aftermetcut", i, k);
    }
    
    //if(j==0)PlotStack("HInvM_",jchan, "_pair_EE", i, k);
    //if(j==0)PlotStack("HInvM_",jchan, "_pair_EB", i, k);
    //if(j==0)PlotStack("Inv",jchan,"MassPairzoom", i, k);
    // after met cut
    PlotStack("Njets_",jchan,"_aftermetcut", i, k);
    
    PlotStack("Njets_",jchan,"_aftermetcutbutjetcut", i, k);
    
    PlotStack("NBjets_",jchan,"_aftermetcut", i, k);
    
    
    
    PlotStack("Inv",jchan,"MassPair_aftermetcut", i, k);
    PlotStack("Met_",jchan,"_aftermetcut", i, k);
    
    //PlotStack("DeltaPhiLMet_",jchan,"_aftermetcut", i, k);
    //PlotStack("PtJet1_", jchan, "_aftermetcut", i, k);
    //PlotStack("PtJet2_", jchan, "_aftermetcut", i, k);

    // plot met after btag
    PlotStack("Met_",jchan,"_afterbtag1", i, k);
    PlotStack("Njets_",jchan,"_aftermetbtag1", i, k);
    PlotStack("Met_",jchan,"_afterbtag2", i, k);
  
    PlotStack("Inv",jchan,"MassPair_afterjetcut", i, k);
    
    
    
    PlotStack("DeltaPhiLLept_",jchan,"_aftermetcut", i, k);
    PlotStack("DeltaPhiLLept_",jchan,"_afterbtag1", i, k);
    PlotStack("DeltaPhiLLept_",jchan,"_afterbtag2", i, k);
  
    
    PlotStack("NVxP_",jchan,"_aftertrigger", i, k);
    PlotStack("NVxP_",jchan,"_aftermetcut", i, k);
    PlotStack("NVxP_",jchan,"", i, k);
    
    
    PlotStack("ttbarTransverseMass_",jchan,"_afterbtag1", i, k);
    
   
   } // end loop k
  } // end loop i
 } // end loop jchan
 }
