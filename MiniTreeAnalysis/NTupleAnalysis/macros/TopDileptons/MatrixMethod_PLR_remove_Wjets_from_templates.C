MatrixMethod_PLR_remove_Wjets_from_templates(){


  //TFile * file_IN = new TFile("FinalTemplates_withDYPowheg_1137_new.root","update");
  TFile * file_IN = new TFile("TCHE_3.3/combined.root","update");

  file_IN->cd();


  gDirectory->Delete("ee_Count_Wjets;1"); 
  gDirectory->Delete("ee_Count_Wjets;2"); 
  gDirectory->Delete("ee_Count_Wjets;3"); 
  gDirectory->Delete("ee_Count_Wjets;4"); 
  gDirectory->Delete("ee_Count_Wjets;5"); 
  gDirectory->Delete("ee_Count_Wjets;6"); 
  gDirectory->Delete("ee_Count_Wjets;7"); 
  gDirectory->Delete("ee_Count_Wjets;8");
  gDirectory->Delete("ee_Count_Wjets;9"); 
  gDirectory->Delete("ee_Count_Wjets;10"); 
  gDirectory->Delete("ee_Count_Wjets;11"); 
  gDirectory->Delete("ee_Count_Wjets;12"); 
  gDirectory->Delete("ee_Count_Wjets;13"); 
  gDirectory->Delete("ee_Count_Wjets;14"); 
  gDirectory->Delete("ee_Count_Wjets;15"); 
  gDirectory->Delete("ee_Count_Wjets;16"); 
  gDirectory->Delete("ee_Count_Wjets;17"); 
  gDirectory->Delete("ee_Count_Wjets;18"); 
  gDirectory->Delete("ee_Count_Wjets;19"); 
  gDirectory->Delete("ee_Count_Wjets;20"); 
  gDirectory->Delete("ee_Count_Wjets;21"); 
  gDirectory->Delete("ee_Count_Wjets;22"); 
  gDirectory->Delete("ee_Count_Wjets;23"); 
  gDirectory->Delete("ee_Count_Wjets;24"); 



  gDirectory->Delete("ee_Count_JES-plus_Wjets;1"); 
  gDirectory->Delete("ee_Count_JES-plus_Wjets;2"); 
  gDirectory->Delete("ee_Count_JES-plus_Wjets;3"); 
  gDirectory->Delete("ee_Count_JES-plus_Wjets;4"); 
  gDirectory->Delete("ee_Count_JES-plus_Wjets;5"); 
  gDirectory->Delete("ee_Count_JES-plus_Wjets;6"); 
  gDirectory->Delete("ee_Count_JES-plus_Wjets;7"); 
  gDirectory->Delete("ee_Count_JES-plus_Wjets;8");
  gDirectory->Delete("ee_Count_JES-plus_Wjets;9"); 
  gDirectory->Delete("ee_Count_JES-plus_Wjets;10"); 
  gDirectory->Delete("ee_Count_JES-plus_Wjets;11"); 
  gDirectory->Delete("ee_Count_JES-plus_Wjets;12"); 
  gDirectory->Delete("ee_Count_JES-plus_Wjets;13"); 
  gDirectory->Delete("ee_Count_JES-plus_Wjets;14"); 
  gDirectory->Delete("ee_Count_JES-plus_Wjets;15"); 
  gDirectory->Delete("ee_Count_JES-plus_Wjets;16"); 
  gDirectory->Delete("ee_Count_JES-plus_Wjets;17"); 
  gDirectory->Delete("ee_Count_JES-plus_Wjets;18"); 
  gDirectory->Delete("ee_Count_JES-plus_Wjets;19"); 
  gDirectory->Delete("ee_Count_JES-plus_Wjets;20"); 
  gDirectory->Delete("ee_Count_JES-plus_Wjets;21"); 
  gDirectory->Delete("ee_Count_JES-plus_Wjets;22"); 
  gDirectory->Delete("ee_Count_JES-plus_Wjets;23"); 
  gDirectory->Delete("ee_Count_JES-plus_Wjets;24"); 



  gDirectory->Delete("ee_Count_JES-minus_Wjets;1"); 
  gDirectory->Delete("ee_Count_JES-minus_Wjets;2"); 
  gDirectory->Delete("ee_Count_JES-minus_Wjets;3"); 
  gDirectory->Delete("ee_Count_JES-minus_Wjets;4"); 
  gDirectory->Delete("ee_Count_JES-minus_Wjets;5"); 
  gDirectory->Delete("ee_Count_JES-minus_Wjets;6"); 
  gDirectory->Delete("ee_Count_JES-minus_Wjets;7"); 
  gDirectory->Delete("ee_Count_JES-minus_Wjets;8");
  gDirectory->Delete("ee_Count_JES-minus_Wjets;9"); 
  gDirectory->Delete("ee_Count_JES-minus_Wjets;10"); 
  gDirectory->Delete("ee_Count_JES-minus_Wjets;11"); 
  gDirectory->Delete("ee_Count_JES-minus_Wjets;12"); 
  gDirectory->Delete("ee_Count_JES-minus_Wjets;13"); 
  gDirectory->Delete("ee_Count_JES-minus_Wjets;14"); 
  gDirectory->Delete("ee_Count_JES-minus_Wjets;15"); 
  gDirectory->Delete("ee_Count_JES-minus_Wjets;16"); 
  gDirectory->Delete("ee_Count_JES-minus_Wjets;17"); 
  gDirectory->Delete("ee_Count_JES-minus_Wjets;18"); 
  gDirectory->Delete("ee_Count_JES-minus_Wjets;19"); 
  gDirectory->Delete("ee_Count_JES-minus_Wjets;20"); 
  gDirectory->Delete("ee_Count_JES-minus_Wjets;21"); 
  gDirectory->Delete("ee_Count_JES-minus_Wjets;22"); 
  gDirectory->Delete("ee_Count_JES-minus_Wjets;23"); 
  gDirectory->Delete("ee_Count_JES-minus_Wjets;24"); 















  gDirectory->Delete("mumu_Count_Wjets;1"); 
  gDirectory->Delete("mumu_Count_Wjets;2"); 
  gDirectory->Delete("mumu_Count_Wjets;3"); 
  gDirectory->Delete("mumu_Count_Wjets;4"); 
  gDirectory->Delete("mumu_Count_Wjets;5"); 
  gDirectory->Delete("mumu_Count_Wjets;6"); 
  gDirectory->Delete("mumu_Count_Wjets;7"); 
  gDirectory->Delete("mumu_Count_Wjets;8");
  gDirectory->Delete("mumu_Count_Wjets;9"); 
  gDirectory->Delete("mumu_Count_Wjets;10"); 
  gDirectory->Delete("mumu_Count_Wjets;11"); 
  gDirectory->Delete("mumu_Count_Wjets;12"); 
  gDirectory->Delete("mumu_Count_Wjets;13"); 
  gDirectory->Delete("mumu_Count_Wjets;14"); 
  gDirectory->Delete("mumu_Count_Wjets;15"); 
  gDirectory->Delete("mumu_Count_Wjets;16"); 
  gDirectory->Delete("mumu_Count_Wjets;17"); 
  gDirectory->Delete("mumu_Count_Wjets;18"); 
  gDirectory->Delete("mumu_Count_Wjets;19"); 
  gDirectory->Delete("mumu_Count_Wjets;20"); 
  gDirectory->Delete("mumu_Count_Wjets;21"); 
  gDirectory->Delete("mumu_Count_Wjets;22"); 
  gDirectory->Delete("mumu_Count_Wjets;23"); 
  gDirectory->Delete("mumu_Count_Wjets;24"); 



  gDirectory->Delete("mumu_Count_JES-plus_Wjets;1"); 
  gDirectory->Delete("mumu_Count_JES-plus_Wjets;2"); 
  gDirectory->Delete("mumu_Count_JES-plus_Wjets;3"); 
  gDirectory->Delete("mumu_Count_JES-plus_Wjets;4"); 
  gDirectory->Delete("mumu_Count_JES-plus_Wjets;5"); 
  gDirectory->Delete("mumu_Count_JES-plus_Wjets;6"); 
  gDirectory->Delete("mumu_Count_JES-plus_Wjets;7"); 
  gDirectory->Delete("mumu_Count_JES-plus_Wjets;8");
  gDirectory->Delete("mumu_Count_JES-plus_Wjets;9"); 
  gDirectory->Delete("mumu_Count_JES-plus_Wjets;10"); 
  gDirectory->Delete("mumu_Count_JES-plus_Wjets;11"); 
  gDirectory->Delete("mumu_Count_JES-plus_Wjets;12"); 
  gDirectory->Delete("mumu_Count_JES-plus_Wjets;13"); 
  gDirectory->Delete("mumu_Count_JES-plus_Wjets;14"); 
  gDirectory->Delete("mumu_Count_JES-plus_Wjets;15"); 
  gDirectory->Delete("mumu_Count_JES-plus_Wjets;16"); 
  gDirectory->Delete("mumu_Count_JES-plus_Wjets;17"); 
  gDirectory->Delete("mumu_Count_JES-plus_Wjets;18"); 
  gDirectory->Delete("mumu_Count_JES-plus_Wjets;19"); 
  gDirectory->Delete("mumu_Count_JES-plus_Wjets;20"); 
  gDirectory->Delete("mumu_Count_JES-plus_Wjets;21"); 
  gDirectory->Delete("mumu_Count_JES-plus_Wjets;22"); 
  gDirectory->Delete("mumu_Count_JES-plus_Wjets;23"); 
  gDirectory->Delete("mumu_Count_JES-plus_Wjets;24"); 



  gDirectory->Delete("mumu_Count_JES-minus_Wjets;1"); 
  gDirectory->Delete("mumu_Count_JES-minus_Wjets;2"); 
  gDirectory->Delete("mumu_Count_JES-minus_Wjets;3"); 
  gDirectory->Delete("mumu_Count_JES-minus_Wjets;4"); 
  gDirectory->Delete("mumu_Count_JES-minus_Wjets;5"); 
  gDirectory->Delete("mumu_Count_JES-minus_Wjets;6"); 
  gDirectory->Delete("mumu_Count_JES-minus_Wjets;7"); 
  gDirectory->Delete("mumu_Count_JES-minus_Wjets;8");
  gDirectory->Delete("mumu_Count_JES-minus_Wjets;9"); 
  gDirectory->Delete("mumu_Count_JES-minus_Wjets;10"); 
  gDirectory->Delete("mumu_Count_JES-minus_Wjets;11"); 
  gDirectory->Delete("mumu_Count_JES-minus_Wjets;12"); 
  gDirectory->Delete("mumu_Count_JES-minus_Wjets;13"); 
  gDirectory->Delete("mumu_Count_JES-minus_Wjets;14"); 
  gDirectory->Delete("mumu_Count_JES-minus_Wjets;15"); 
  gDirectory->Delete("mumu_Count_JES-minus_Wjets;16"); 
  gDirectory->Delete("mumu_Count_JES-minus_Wjets;17"); 
  gDirectory->Delete("mumu_Count_JES-minus_Wjets;18"); 
  gDirectory->Delete("mumu_Count_JES-minus_Wjets;19"); 
  gDirectory->Delete("mumu_Count_JES-minus_Wjets;20"); 
  gDirectory->Delete("mumu_Count_JES-minus_Wjets;21"); 
  gDirectory->Delete("mumu_Count_JES-minus_Wjets;22"); 
  gDirectory->Delete("mumu_Count_JES-minus_Wjets;23"); 
  gDirectory->Delete("mumu_Count_JES-minus_Wjets;24"); 






  gDirectory->Delete("emu_Count_Wjets;1"); 
  gDirectory->Delete("emu_Count_Wjets;2"); 
  gDirectory->Delete("emu_Count_Wjets;3"); 
  gDirectory->Delete("emu_Count_Wjets;4"); 
  gDirectory->Delete("emu_Count_Wjets;5"); 
  gDirectory->Delete("emu_Count_Wjets;6"); 
  gDirectory->Delete("emu_Count_Wjets;7"); 
  gDirectory->Delete("emu_Count_Wjets;8");
  gDirectory->Delete("emu_Count_Wjets;9"); 
  gDirectory->Delete("emu_Count_Wjets;10"); 
  gDirectory->Delete("emu_Count_Wjets;11"); 
  gDirectory->Delete("emu_Count_Wjets;12"); 
  gDirectory->Delete("emu_Count_Wjets;13"); 
  gDirectory->Delete("emu_Count_Wjets;14"); 
  gDirectory->Delete("emu_Count_Wjets;15"); 
  gDirectory->Delete("emu_Count_Wjets;16"); 
  gDirectory->Delete("emu_Count_Wjets;17"); 
  gDirectory->Delete("emu_Count_Wjets;18"); 
  gDirectory->Delete("emu_Count_Wjets;19"); 
  gDirectory->Delete("emu_Count_Wjets;20"); 
  gDirectory->Delete("emu_Count_Wjets;21"); 
  gDirectory->Delete("emu_Count_Wjets;22"); 
  gDirectory->Delete("emu_Count_Wjets;23"); 
  gDirectory->Delete("emu_Count_Wjets;24"); 



  gDirectory->Delete("emu_Count_JES-plus_Wjets;1"); 
  gDirectory->Delete("emu_Count_JES-plus_Wjets;2"); 
  gDirectory->Delete("emu_Count_JES-plus_Wjets;3"); 
  gDirectory->Delete("emu_Count_JES-plus_Wjets;4"); 
  gDirectory->Delete("emu_Count_JES-plus_Wjets;5"); 
  gDirectory->Delete("emu_Count_JES-plus_Wjets;6"); 
  gDirectory->Delete("emu_Count_JES-plus_Wjets;7"); 
  gDirectory->Delete("emu_Count_JES-plus_Wjets;8");
  gDirectory->Delete("emu_Count_JES-plus_Wjets;9"); 
  gDirectory->Delete("emu_Count_JES-plus_Wjets;10"); 
  gDirectory->Delete("emu_Count_JES-plus_Wjets;11"); 
  gDirectory->Delete("emu_Count_JES-plus_Wjets;12"); 
  gDirectory->Delete("emu_Count_JES-plus_Wjets;13"); 
  gDirectory->Delete("emu_Count_JES-plus_Wjets;14"); 
  gDirectory->Delete("emu_Count_JES-plus_Wjets;15"); 
  gDirectory->Delete("emu_Count_JES-plus_Wjets;16"); 
  gDirectory->Delete("emu_Count_JES-plus_Wjets;17"); 
  gDirectory->Delete("emu_Count_JES-plus_Wjets;18"); 
  gDirectory->Delete("emu_Count_JES-plus_Wjets;19"); 
  gDirectory->Delete("emu_Count_JES-plus_Wjets;20"); 
  gDirectory->Delete("emu_Count_JES-plus_Wjets;21"); 
  gDirectory->Delete("emu_Count_JES-plus_Wjets;22"); 
  gDirectory->Delete("emu_Count_JES-plus_Wjets;23"); 
  gDirectory->Delete("emu_Count_JES-plus_Wjets;24"); 



  gDirectory->Delete("emu_Count_JES-minus_Wjets;1"); 
  gDirectory->Delete("emu_Count_JES-minus_Wjets;2"); 
  gDirectory->Delete("emu_Count_JES-minus_Wjets;3"); 
  gDirectory->Delete("emu_Count_JES-minus_Wjets;4"); 
  gDirectory->Delete("emu_Count_JES-minus_Wjets;5"); 
  gDirectory->Delete("emu_Count_JES-minus_Wjets;6"); 
  gDirectory->Delete("emu_Count_JES-minus_Wjets;7"); 
  gDirectory->Delete("emu_Count_JES-minus_Wjets;8");
  gDirectory->Delete("emu_Count_JES-minus_Wjets;9"); 
  gDirectory->Delete("emu_Count_JES-minus_Wjets;10"); 
  gDirectory->Delete("emu_Count_JES-minus_Wjets;11"); 
  gDirectory->Delete("emu_Count_JES-minus_Wjets;12"); 
  gDirectory->Delete("emu_Count_JES-minus_Wjets;13"); 
  gDirectory->Delete("emu_Count_JES-minus_Wjets;14"); 
  gDirectory->Delete("emu_Count_JES-minus_Wjets;15"); 
  gDirectory->Delete("emu_Count_JES-minus_Wjets;16"); 
  gDirectory->Delete("emu_Count_JES-minus_Wjets;17"); 
  gDirectory->Delete("emu_Count_JES-minus_Wjets;18"); 
  gDirectory->Delete("emu_Count_JES-minus_Wjets;19"); 
  gDirectory->Delete("emu_Count_JES-minus_Wjets;20"); 
  gDirectory->Delete("emu_Count_JES-minus_Wjets;21"); 
  gDirectory->Delete("emu_Count_JES-minus_Wjets;22"); 
  gDirectory->Delete("emu_Count_JES-minus_Wjets;23"); 
  gDirectory->Delete("emu_Count_JES-minus_Wjets;24"); 



























































  gDirectory->Delete("ee_Njets_Wjets;1"); 
  gDirectory->Delete("ee_Njets_Wjets;2"); 
  gDirectory->Delete("ee_Njets_Wjets;3"); 
  gDirectory->Delete("ee_Njets_Wjets;4"); 
  gDirectory->Delete("ee_Njets_Wjets;5"); 
  gDirectory->Delete("ee_Njets_Wjets;6"); 
  gDirectory->Delete("ee_Njets_Wjets;7"); 
  gDirectory->Delete("ee_Njets_Wjets;8");
  gDirectory->Delete("ee_Njets_Wjets;9"); 
  gDirectory->Delete("ee_Njets_Wjets;10"); 
  gDirectory->Delete("ee_Njets_Wjets;11"); 
  gDirectory->Delete("ee_Njets_Wjets;12"); 
  gDirectory->Delete("ee_Njets_Wjets;13"); 
  gDirectory->Delete("ee_Njets_Wjets;14"); 
  gDirectory->Delete("ee_Njets_Wjets;15"); 
  gDirectory->Delete("ee_Njets_Wjets;16"); 
  gDirectory->Delete("ee_Njets_Wjets;17"); 
  gDirectory->Delete("ee_Njets_Wjets;18"); 
  gDirectory->Delete("ee_Njets_Wjets;19"); 
  gDirectory->Delete("ee_Njets_Wjets;20"); 
  gDirectory->Delete("ee_Njets_Wjets;21"); 
  gDirectory->Delete("ee_Njets_Wjets;22"); 
  gDirectory->Delete("ee_Njets_Wjets;23"); 
  gDirectory->Delete("ee_Njets_Wjets;24"); 



  gDirectory->Delete("ee_Njets_JES-plus_Wjets;1"); 
  gDirectory->Delete("ee_Njets_JES-plus_Wjets;2"); 
  gDirectory->Delete("ee_Njets_JES-plus_Wjets;3"); 
  gDirectory->Delete("ee_Njets_JES-plus_Wjets;4"); 
  gDirectory->Delete("ee_Njets_JES-plus_Wjets;5"); 
  gDirectory->Delete("ee_Njets_JES-plus_Wjets;6"); 
  gDirectory->Delete("ee_Njets_JES-plus_Wjets;7"); 
  gDirectory->Delete("ee_Njets_JES-plus_Wjets;8");
  gDirectory->Delete("ee_Njets_JES-plus_Wjets;9"); 
  gDirectory->Delete("ee_Njets_JES-plus_Wjets;10"); 
  gDirectory->Delete("ee_Njets_JES-plus_Wjets;11"); 
  gDirectory->Delete("ee_Njets_JES-plus_Wjets;12"); 
  gDirectory->Delete("ee_Njets_JES-plus_Wjets;13"); 
  gDirectory->Delete("ee_Njets_JES-plus_Wjets;14"); 
  gDirectory->Delete("ee_Njets_JES-plus_Wjets;15"); 
  gDirectory->Delete("ee_Njets_JES-plus_Wjets;16"); 
  gDirectory->Delete("ee_Njets_JES-plus_Wjets;17"); 
  gDirectory->Delete("ee_Njets_JES-plus_Wjets;18"); 
  gDirectory->Delete("ee_Njets_JES-plus_Wjets;19"); 
  gDirectory->Delete("ee_Njets_JES-plus_Wjets;20"); 
  gDirectory->Delete("ee_Njets_JES-plus_Wjets;21"); 
  gDirectory->Delete("ee_Njets_JES-plus_Wjets;22"); 
  gDirectory->Delete("ee_Njets_JES-plus_Wjets;23"); 
  gDirectory->Delete("ee_Njets_JES-plus_Wjets;24"); 



  gDirectory->Delete("ee_Njets_JES-minus_Wjets;1"); 
  gDirectory->Delete("ee_Njets_JES-minus_Wjets;2"); 
  gDirectory->Delete("ee_Njets_JES-minus_Wjets;3"); 
  gDirectory->Delete("ee_Njets_JES-minus_Wjets;4"); 
  gDirectory->Delete("ee_Njets_JES-minus_Wjets;5"); 
  gDirectory->Delete("ee_Njets_JES-minus_Wjets;6"); 
  gDirectory->Delete("ee_Njets_JES-minus_Wjets;7"); 
  gDirectory->Delete("ee_Njets_JES-minus_Wjets;8");
  gDirectory->Delete("ee_Njets_JES-minus_Wjets;9"); 
  gDirectory->Delete("ee_Njets_JES-minus_Wjets;10"); 
  gDirectory->Delete("ee_Njets_JES-minus_Wjets;11"); 
  gDirectory->Delete("ee_Njets_JES-minus_Wjets;12"); 
  gDirectory->Delete("ee_Njets_JES-minus_Wjets;13"); 
  gDirectory->Delete("ee_Njets_JES-minus_Wjets;14"); 
  gDirectory->Delete("ee_Njets_JES-minus_Wjets;15"); 
  gDirectory->Delete("ee_Njets_JES-minus_Wjets;16"); 
  gDirectory->Delete("ee_Njets_JES-minus_Wjets;17"); 
  gDirectory->Delete("ee_Njets_JES-minus_Wjets;18"); 
  gDirectory->Delete("ee_Njets_JES-minus_Wjets;19"); 
  gDirectory->Delete("ee_Njets_JES-minus_Wjets;20"); 
  gDirectory->Delete("ee_Njets_JES-minus_Wjets;21"); 
  gDirectory->Delete("ee_Njets_JES-minus_Wjets;22"); 
  gDirectory->Delete("ee_Njets_JES-minus_Wjets;23"); 
  gDirectory->Delete("ee_Njets_JES-minus_Wjets;24"); 















  gDirectory->Delete("mumu_Njets_Wjets;1"); 
  gDirectory->Delete("mumu_Njets_Wjets;2"); 
  gDirectory->Delete("mumu_Njets_Wjets;3"); 
  gDirectory->Delete("mumu_Njets_Wjets;4"); 
  gDirectory->Delete("mumu_Njets_Wjets;5"); 
  gDirectory->Delete("mumu_Njets_Wjets;6"); 
  gDirectory->Delete("mumu_Njets_Wjets;7"); 
  gDirectory->Delete("mumu_Njets_Wjets;8");
  gDirectory->Delete("mumu_Njets_Wjets;9"); 
  gDirectory->Delete("mumu_Njets_Wjets;10"); 
  gDirectory->Delete("mumu_Njets_Wjets;11"); 
  gDirectory->Delete("mumu_Njets_Wjets;12"); 
  gDirectory->Delete("mumu_Njets_Wjets;13"); 
  gDirectory->Delete("mumu_Njets_Wjets;14"); 
  gDirectory->Delete("mumu_Njets_Wjets;15"); 
  gDirectory->Delete("mumu_Njets_Wjets;16"); 
  gDirectory->Delete("mumu_Njets_Wjets;17"); 
  gDirectory->Delete("mumu_Njets_Wjets;18"); 
  gDirectory->Delete("mumu_Njets_Wjets;19"); 
  gDirectory->Delete("mumu_Njets_Wjets;20"); 
  gDirectory->Delete("mumu_Njets_Wjets;21"); 
  gDirectory->Delete("mumu_Njets_Wjets;22"); 
  gDirectory->Delete("mumu_Njets_Wjets;23"); 
  gDirectory->Delete("mumu_Njets_Wjets;24"); 



  gDirectory->Delete("mumu_Njets_JES-plus_Wjets;1"); 
  gDirectory->Delete("mumu_Njets_JES-plus_Wjets;2"); 
  gDirectory->Delete("mumu_Njets_JES-plus_Wjets;3"); 
  gDirectory->Delete("mumu_Njets_JES-plus_Wjets;4"); 
  gDirectory->Delete("mumu_Njets_JES-plus_Wjets;5"); 
  gDirectory->Delete("mumu_Njets_JES-plus_Wjets;6"); 
  gDirectory->Delete("mumu_Njets_JES-plus_Wjets;7"); 
  gDirectory->Delete("mumu_Njets_JES-plus_Wjets;8");
  gDirectory->Delete("mumu_Njets_JES-plus_Wjets;9"); 
  gDirectory->Delete("mumu_Njets_JES-plus_Wjets;10"); 
  gDirectory->Delete("mumu_Njets_JES-plus_Wjets;11"); 
  gDirectory->Delete("mumu_Njets_JES-plus_Wjets;12"); 
  gDirectory->Delete("mumu_Njets_JES-plus_Wjets;13"); 
  gDirectory->Delete("mumu_Njets_JES-plus_Wjets;14"); 
  gDirectory->Delete("mumu_Njets_JES-plus_Wjets;15"); 
  gDirectory->Delete("mumu_Njets_JES-plus_Wjets;16"); 
  gDirectory->Delete("mumu_Njets_JES-plus_Wjets;17"); 
  gDirectory->Delete("mumu_Njets_JES-plus_Wjets;18"); 
  gDirectory->Delete("mumu_Njets_JES-plus_Wjets;19"); 
  gDirectory->Delete("mumu_Njets_JES-plus_Wjets;20"); 
  gDirectory->Delete("mumu_Njets_JES-plus_Wjets;21"); 
  gDirectory->Delete("mumu_Njets_JES-plus_Wjets;22"); 
  gDirectory->Delete("mumu_Njets_JES-plus_Wjets;23"); 
  gDirectory->Delete("mumu_Njets_JES-plus_Wjets;24"); 



  gDirectory->Delete("mumu_Njets_JES-minus_Wjets;1"); 
  gDirectory->Delete("mumu_Njets_JES-minus_Wjets;2"); 
  gDirectory->Delete("mumu_Njets_JES-minus_Wjets;3"); 
  gDirectory->Delete("mumu_Njets_JES-minus_Wjets;4"); 
  gDirectory->Delete("mumu_Njets_JES-minus_Wjets;5"); 
  gDirectory->Delete("mumu_Njets_JES-minus_Wjets;6"); 
  gDirectory->Delete("mumu_Njets_JES-minus_Wjets;7"); 
  gDirectory->Delete("mumu_Njets_JES-minus_Wjets;8");
  gDirectory->Delete("mumu_Njets_JES-minus_Wjets;9"); 
  gDirectory->Delete("mumu_Njets_JES-minus_Wjets;10"); 
  gDirectory->Delete("mumu_Njets_JES-minus_Wjets;11"); 
  gDirectory->Delete("mumu_Njets_JES-minus_Wjets;12"); 
  gDirectory->Delete("mumu_Njets_JES-minus_Wjets;13"); 
  gDirectory->Delete("mumu_Njets_JES-minus_Wjets;14"); 
  gDirectory->Delete("mumu_Njets_JES-minus_Wjets;15"); 
  gDirectory->Delete("mumu_Njets_JES-minus_Wjets;16"); 
  gDirectory->Delete("mumu_Njets_JES-minus_Wjets;17"); 
  gDirectory->Delete("mumu_Njets_JES-minus_Wjets;18"); 
  gDirectory->Delete("mumu_Njets_JES-minus_Wjets;19"); 
  gDirectory->Delete("mumu_Njets_JES-minus_Wjets;20"); 
  gDirectory->Delete("mumu_Njets_JES-minus_Wjets;21"); 
  gDirectory->Delete("mumu_Njets_JES-minus_Wjets;22"); 
  gDirectory->Delete("mumu_Njets_JES-minus_Wjets;23"); 
  gDirectory->Delete("mumu_Njets_JES-minus_Wjets;24"); 






  gDirectory->Delete("emu_Njets_Wjets;1"); 
  gDirectory->Delete("emu_Njets_Wjets;2"); 
  gDirectory->Delete("emu_Njets_Wjets;3"); 
  gDirectory->Delete("emu_Njets_Wjets;4"); 
  gDirectory->Delete("emu_Njets_Wjets;5"); 
  gDirectory->Delete("emu_Njets_Wjets;6"); 
  gDirectory->Delete("emu_Njets_Wjets;7"); 
  gDirectory->Delete("emu_Njets_Wjets;8");
  gDirectory->Delete("emu_Njets_Wjets;9"); 
  gDirectory->Delete("emu_Njets_Wjets;10"); 
  gDirectory->Delete("emu_Njets_Wjets;11"); 
  gDirectory->Delete("emu_Njets_Wjets;12"); 
  gDirectory->Delete("emu_Njets_Wjets;13"); 
  gDirectory->Delete("emu_Njets_Wjets;14"); 
  gDirectory->Delete("emu_Njets_Wjets;15"); 
  gDirectory->Delete("emu_Njets_Wjets;16"); 
  gDirectory->Delete("emu_Njets_Wjets;17"); 
  gDirectory->Delete("emu_Njets_Wjets;18"); 
  gDirectory->Delete("emu_Njets_Wjets;19"); 
  gDirectory->Delete("emu_Njets_Wjets;20"); 
  gDirectory->Delete("emu_Njets_Wjets;21"); 
  gDirectory->Delete("emu_Njets_Wjets;22"); 
  gDirectory->Delete("emu_Njets_Wjets;23"); 
  gDirectory->Delete("emu_Njets_Wjets;24"); 



  gDirectory->Delete("emu_Njets_JES-plus_Wjets;1"); 
  gDirectory->Delete("emu_Njets_JES-plus_Wjets;2"); 
  gDirectory->Delete("emu_Njets_JES-plus_Wjets;3"); 
  gDirectory->Delete("emu_Njets_JES-plus_Wjets;4"); 
  gDirectory->Delete("emu_Njets_JES-plus_Wjets;5"); 
  gDirectory->Delete("emu_Njets_JES-plus_Wjets;6"); 
  gDirectory->Delete("emu_Njets_JES-plus_Wjets;7"); 
  gDirectory->Delete("emu_Njets_JES-plus_Wjets;8");
  gDirectory->Delete("emu_Njets_JES-plus_Wjets;9"); 
  gDirectory->Delete("emu_Njets_JES-plus_Wjets;10"); 
  gDirectory->Delete("emu_Njets_JES-plus_Wjets;11"); 
  gDirectory->Delete("emu_Njets_JES-plus_Wjets;12"); 
  gDirectory->Delete("emu_Njets_JES-plus_Wjets;13"); 
  gDirectory->Delete("emu_Njets_JES-plus_Wjets;14"); 
  gDirectory->Delete("emu_Njets_JES-plus_Wjets;15"); 
  gDirectory->Delete("emu_Njets_JES-plus_Wjets;16"); 
  gDirectory->Delete("emu_Njets_JES-plus_Wjets;17"); 
  gDirectory->Delete("emu_Njets_JES-plus_Wjets;18"); 
  gDirectory->Delete("emu_Njets_JES-plus_Wjets;19"); 
  gDirectory->Delete("emu_Njets_JES-plus_Wjets;20"); 
  gDirectory->Delete("emu_Njets_JES-plus_Wjets;21"); 
  gDirectory->Delete("emu_Njets_JES-plus_Wjets;22"); 
  gDirectory->Delete("emu_Njets_JES-plus_Wjets;23"); 
  gDirectory->Delete("emu_Njets_JES-plus_Wjets;24"); 



  gDirectory->Delete("emu_Njets_JES-minus_Wjets;1"); 
  gDirectory->Delete("emu_Njets_JES-minus_Wjets;2"); 
  gDirectory->Delete("emu_Njets_JES-minus_Wjets;3"); 
  gDirectory->Delete("emu_Njets_JES-minus_Wjets;4"); 
  gDirectory->Delete("emu_Njets_JES-minus_Wjets;5"); 
  gDirectory->Delete("emu_Njets_JES-minus_Wjets;6"); 
  gDirectory->Delete("emu_Njets_JES-minus_Wjets;7"); 
  gDirectory->Delete("emu_Njets_JES-minus_Wjets;8");
  gDirectory->Delete("emu_Njets_JES-minus_Wjets;9"); 
  gDirectory->Delete("emu_Njets_JES-minus_Wjets;10"); 
  gDirectory->Delete("emu_Njets_JES-minus_Wjets;11"); 
  gDirectory->Delete("emu_Njets_JES-minus_Wjets;12"); 
  gDirectory->Delete("emu_Njets_JES-minus_Wjets;13"); 
  gDirectory->Delete("emu_Njets_JES-minus_Wjets;14"); 
  gDirectory->Delete("emu_Njets_JES-minus_Wjets;15"); 
  gDirectory->Delete("emu_Njets_JES-minus_Wjets;16"); 
  gDirectory->Delete("emu_Njets_JES-minus_Wjets;17"); 
  gDirectory->Delete("emu_Njets_JES-minus_Wjets;18"); 
  gDirectory->Delete("emu_Njets_JES-minus_Wjets;19"); 
  gDirectory->Delete("emu_Njets_JES-minus_Wjets;20"); 
  gDirectory->Delete("emu_Njets_JES-minus_Wjets;21"); 
  gDirectory->Delete("emu_Njets_JES-minus_Wjets;22"); 
  gDirectory->Delete("emu_Njets_JES-minus_Wjets;23"); 
  gDirectory->Delete("emu_Njets_JES-minus_Wjets;24"); 








  file_IN->Close();




 
}
