{

  gROOT->ProcessLine(".L PlotDYStack.C");


//  PlotDYStack("ee", "InvMassemu", 0, -1);
//  PlotDYStack("ee", "InvMassContCR", 0, -1);
  
//  PlotDYStack("ee", "InvMassCR", 1, -1);

//  PlotDYStack("ee", "InvMassSR", 0, 35);

//  PlotDYStack("mumu", "InvMassemu", 0, -1);
//  PlotDYStack("mumu", "InvMassContCR", 0, -1);
  
//  PlotDYStack("mumu", "InvMassCR", 1, -1);

  PlotDYStack("mumu", "InvMassSR", 0,60 );

}
