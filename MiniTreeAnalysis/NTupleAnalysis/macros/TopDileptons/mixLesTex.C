
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include <limits.h>



#include "../../../../MiniTreeFormat/NTFormat/interface/NTEvent.h"

//NTupleAnalysis classes
#include "../../Selection/interface/DiLeptonSelection.h"
#include "../../Selection/interface/SelectionTable.h"
#include "../../Tools/interface/Dataset.h"
#include "../../Tools/interface/AnalysisEnvironmentLoader.h"
#include "../../Plots/interface/DiLepAnaHistoManager.h"
#include "../../BckgdEstimation/interface/DYEstimation.h"
#include "../../BckgdEstimation/interface/MMEstimation.h"
#include "../../Measurements/interface/PLRPlotsFurnisher.h"
#include "../../Tools/interface/PUWeighting.h"


#include <TFile.h>
#include <TTree.h>
#include <TBranch.h>
#include <TH1.h>
#include <TH2.h>
#include <TCanvas.h>


///////////////////////////////////////////////////////////////////////////////////


using namespace std;

int main ()
{
  cout<<"#########################"<<endl;
  cout<<"Beginning of the program"<<endl;
  cout<<"#########################"<<endl;


  int indic_precision=1;  //3
  int print_info=1;

  // CHECK THE VALUES!!!
  double SF_DY_ee = 1.55;
  double SF_DY_mm = 1.4;
  double SF_DY_em = 1.0;

  double SF_Wjets_ee = 1.;
  double SF_Wjets_mm = 1.;
  double SF_Wjets_em = 1.;

  
  cout<<"usage: ./mixLesTex"<<endl;


  string namedata[37];
  int ichan[37];
  float cut[8][37],errcut[8][37];
  float sf1[37],sf2[37];

  int jdata[3], jmcsum[3], jtopsig[3], jmcbg[3], jtopbg[3], jsingle[3], jwjet[3], jdy1[3], jdy2[3], jdiboson[3];
  for (int i2=0; i2<3; i2++) {
     jdata[i2]=30;
     jmcsum[i2]=30;
     jtopsig[i2]=30;
     jtopbg[i2]=30;
     jsingle[i2]=30;
     jwjet[i2]=30;
     jdy1[i2]=30;
     jdy2[i2]=30;
     jdiboson[i2]=30;
  }
  jmcsum[0]=31;
  jmcsum[1]=32;
  jmcsum[2]=33;
  jmcbg[0]=34;
  jmcbg[1]=35;
  jmcbg[2]=36;

  //init =0 
  for (int i3=0; i3<37; i3++) {
   ichan[i3]=-1;
   for (int i3_2=0; i3_2<8; i3_2++) {
     cut[i3_2][i3]=0;
     errcut[i3_2][i3]=0;
   }
   sf1[i3]=0;
   sf2[i3]=0;
  }

  string namefile[4];
  namefile[0]="CrossSection_data.txt";
  namefile[1]="CrossSection_mc.txt";
  namefile[2]="CrossSection_dy1.txt";
  namefile[3]="CrossSection_dy2.txt";

  int idataset=0;
  for (int i4=0; i4<4; i4++) {
      cout << " OPENING OF " << namefile[i4] << endl;
      string tobeopened=namefile[i4];
      ifstream file_in(tobeopened.c_str());
      while (!file_in.eof () && idataset<30) {
        file_in >> namedata[idataset] >> ichan[idataset] ;
        for (int i4_2=0; i4_2<8; i4_2++) {
              file_in >> cut[i4_2][idataset] >> errcut[i4_2][idataset];
        }
        file_in >> sf1[idataset] >> sf2[idataset];

        if (namedata[idataset]=="Data")           jdata[ichan[idataset]]=idataset;
        else if (namedata[idataset]=="TTbarSig")  jtopsig[ichan[idataset]]=idataset;
        else if (namedata[idataset]=="TopBkgd")   jtopbg[ichan[idataset]]=idataset;
        else if (namedata[idataset]=="SingleTop") jsingle[ichan[idataset]]=idataset;
        else if (namedata[idataset]=="Wjet")      jwjet[ichan[idataset]]=idataset;
        else if (namedata[idataset]=="Diboson")   jdiboson[ichan[idataset]]=idataset;
        else if (namedata[idataset]=="DY") { 
             if (jdy1[ichan[idataset]]==30)   jdy1[ichan[idataset]]=idataset;
             else jdy2[ichan[idataset]]=idataset;
        }

        if (ichan[idataset]!=-1) idataset++;
      }
      file_in.close ();
  }
  cout << " # of DATASETS : " << idataset << endl;
  if (print_info==1) {
   for (int i5=0; i5<idataset; i5++) {
    cout << " DEBUG " << namedata[i5] << " " << ichan[i5] << " ";
    for (int i5_2=0; i5_2<8; i5_2++) {
       cout << cut[i5_2][i5] << " " << errcut[i5_2][i5] << "  " ;
    }
    cout  <<  sf1[i5] << " " << sf2[i5] << endl;
   }
  }

  // compute Sum :)
  for  (int i6=0; i6<3; i6++) {
     float info1=0;
     float info2=0;
     float info10=0;
     float info11=0;
   
     // 1. sum DY 
     if (jdy2[i6]!=30) {
      info1=cut[1][jdy1[i6]]+cut[1][jdy2[i6]];
      info2=cut[2][jdy1[i6]]+cut[2][jdy2[i6]];
      if (sf1[jdy1[i6]]>0 && sf1[jdy2[i6]]>0) info10=cut[1][jdy1[i6]]/sf1[jdy1[i6]]+cut[1][jdy2[i6]]/sf1[jdy2[i6]];
      if (sf2[jdy1[i6]]>0 && sf2[jdy2[i6]]>0) info11=cut[2][jdy1[i6]]/sf2[jdy1[i6]]+cut[2][jdy2[i6]]/sf2[jdy2[i6]];
//      cout << " info2 " << info2 << " = " << cut[2][jdy1[i6]] << " + " << cut[2][jdy2[i6]] << endl;
//      cout << " info11 " << info11 << " = " << cut[2][jdy1[i6]]/sf2[jdy1[i6]] << " + " << cut[2][jdy2[i6]]/sf2[jdy2[i6]] << endl;
      sf1[jdy1[i6]]=info1/info10;
      sf2[jdy1[i6]]=info2/info11;
//      cout << " sf2 " << sf2[jdy1[i6]] << " = " << info2 << " / " << info11 << endl; 
      for (int i6_2=0; i6_2<8; i6_2++) {
//       cout << " DY CUT " << i6_2 << " "  << cut[i6_2][jdy1[i6]] << " " << cut[i6_2][jdy2[i6]] << endl;
       cut[i6_2][jdy1[i6]] += cut[i6_2][jdy2[i6]];
       errcut[i6_2][jdy1[i6]] *= errcut[i6_2][jdy1[i6]];
       errcut[i6_2][jdy1[i6]] += errcut[i6_2][jdy2[i6]]*errcut[i6_2][jdy2[i6]];
       errcut[i6_2][jdy1[i6]] = sqrt(errcut[i6_2][jdy1[i6]]);
      }
     }

     // 2. sum bg
     for (int i6_2=0; i6_2<8; i6_2++) {
       cut[i6_2][jmcbg[i6]] += cut[i6_2][jtopbg[i6]];
       cut[i6_2][jmcbg[i6]] += cut[i6_2][jsingle[i6]];
       cut[i6_2][jmcbg[i6]] += cut[i6_2][jwjet[i6]];
       cut[i6_2][jmcbg[i6]] += cut[i6_2][jdy1[i6]];
       cut[i6_2][jmcbg[i6]] += cut[i6_2][jdiboson[i6]];
       errcut[i6_2][jmcbg[i6]] += errcut[i6_2][jtopbg[i6]]*errcut[i6_2][jtopbg[i6]];
       errcut[i6_2][jmcbg[i6]] += errcut[i6_2][jsingle[i6]]*errcut[i6_2][jsingle[i6]];
       errcut[i6_2][jmcbg[i6]] += errcut[i6_2][jwjet[i6]]*errcut[i6_2][jwjet[i6]];
       errcut[i6_2][jmcbg[i6]] += errcut[i6_2][jdy1[i6]]*errcut[i6_2][jdy1[i6]];
       errcut[i6_2][jmcbg[i6]] += errcut[i6_2][jdiboson[i6]]*errcut[i6_2][jdiboson[i6]];
       errcut[i6_2][jmcbg[i6]] = sqrt(errcut[i6_2][jmcbg[i6]]);
      }
      info1=cut[1][jtopbg[i6]]+cut[1][jsingle[i6]]+cut[1][jwjet[i6]]+cut[1][jdy1[i6]]+cut[1][jdiboson[i6]];
      info2=cut[2][jtopbg[i6]]+cut[2][jsingle[i6]]+cut[2][jwjet[i6]]+cut[2][jdy1[i6]]+cut[2][jdiboson[i6]];
      info10=0;
      if (sf1[jtopbg[i6]]>0 ) info10 += cut[1][jtopbg[i6]]/sf1[jtopbg[i6]];
      if (sf1[jsingle[i6]]>0 ) info10 += cut[1][jsingle[i6]]/sf1[jsingle[i6]];
      if (sf1[jwjet[i6]]>0 ) info10 += cut[1][jwjet[i6]]/sf1[jwjet[i6]];
      if (sf1[jdy1[i6]]>0 ) info10 += cut[1][jdy1[i6]]/sf1[jdy1[i6]];
      if (sf1[jdiboson[i6]]>0 ) info10 += cut[1][jdiboson[i6]]/sf1[jdiboson[i6]];
      info11=0;
      if (sf2[jtopbg[i6]]>0 ) info11 += cut[2][jtopbg[i6]]/sf2[jtopbg[i6]];
      if (sf2[jsingle[i6]]>0 ) info11 += cut[2][jsingle[i6]]/sf2[jsingle[i6]];
      if (sf2[jwjet[i6]]>0 ) info11 += cut[2][jwjet[i6]]/sf2[jwjet[i6]];
      if (sf2[jdy1[i6]]>0 ) info11 += cut[2][jdy1[i6]]/sf2[jdy1[i6]];
      if (sf2[jdiboson[i6]]>0 ) info11 += cut[2][jdiboson[i6]]/sf2[jdiboson[i6]];
      sf1[jmcbg[i6]]=info1/info10;
      sf2[jmcbg[i6]]=info2/info11;

     // 3. sum mc
      for (int i6_2=0; i6_2<8; i6_2++) {
       cut[i6_2][jmcsum[i6]] += cut[i6_2][jtopsig[i6]];
       cut[i6_2][jmcsum[i6]] += cut[i6_2][jmcbg[i6]];
       errcut[i6_2][jmcsum[i6]] += errcut[i6_2][jtopsig[i6]]*errcut[i6_2][jtopsig[i6]];
       errcut[i6_2][jmcsum[i6]] += errcut[i6_2][jmcbg[i6]]*errcut[i6_2][jmcbg[i6]];
       errcut[i6_2][jmcsum[i6]] =sqrt(errcut[i6_2][jmcsum[i6]]);
      }
      info1=cut[1][jtopsig[i6]]+cut[1][jmcbg[i6]];
      info2=cut[2][jtopsig[i6]]+cut[2][jmcbg[i6]];
      info10=0;
      if (sf1[jtopsig[i6]]>0 ) info10 += cut[1][jtopsig[i6]]/sf1[jtopsig[i6]];
      if (sf1[jmcbg[i6]]>0 ) info10 += cut[1][jmcbg[i6]]/sf1[jmcbg[i6]];
      info11=0;
      if (sf2[jtopsig[i6]]>0 ) info11 += cut[2][jtopsig[i6]]/sf2[jtopsig[i6]];
      if (sf2[jmcbg[i6]]>0 ) info11 += cut[2][jmcbg[i6]]/sf2[jmcbg[i6]];
      sf1[jmcsum[i6]]=info1/info10;
      sf2[jmcsum[i6]]=info2/info11;
  }


  vector<string> CutName;  // for same flavour leptons
  CutName.push_back("Total");
  CutName.push_back("Trigger");
//  CutName.push_back("$ll$ pair, M($ll$)$>$12 GeV$/c^2$ and VxP"); 
  CutName.push_back("DiLepton pair"); 
  CutName.push_back("M($ll$) $\\notin$ [76,106]");
  CutName.push_back("$E_T(jet1,2)$ $>$ 30 GeV");
  CutName.push_back("$\\not\\!\\!E_T$ $>$ 30 GeV");
  CutName.push_back("btag ($\\ge$1)"); 
  CutName.push_back("btag ($\\ge$2)"); 
  vector<string> CutName2;
  CutName2.push_back("Total");
  CutName2.push_back("Trigger");
//  CutName2.push_back("$ll$ pair, M($ll$)$>$12 GeV$/c^2$ and VxP");
  CutName2.push_back("DiLepton pair"); 
  CutName2.push_back(" - ");
  CutName2.push_back("$E_T(jet1,2)$ $>$ 30 GeV");
  CutName2.push_back(" - ");
  CutName2.push_back("btag ($\\ge$1)"); 
  CutName2.push_back("btag ($\\ge$2)"); 

  cout<<"#########################"<<endl;
  cout<<" Fill the latex tables   "<<endl;
  cout<<"#########################"<<endl;
  
  string ofilenametex = string("CrossSectionMix")+string(".tex");
  ofstream ofile(ofilenametex.c_str());
  ofile<<"\\documentclass[amsmath,amssymb]{revtex4}"<<endl;
  ofile<<"\\begin{document}"<<endl;
  
  ofile.setf(ios::fixed);
  
  for (int IChannel=0; IChannel<3; IChannel++) {
  // Summary tables
  ofile << "\\clearpage" << endl;
  ofile << "\\begin{table}[p]" << endl;
  
  ofile << "\\begin{tabular}{|l|c|c|c|c|c|}" << endl;
  ofile << "\\hline" << endl;
  ofile << "\\hline" << endl;
  ofile << "Cut & DATA & Sum MC & TT signal  & Total Background & S/B \\\\" << endl;
  ofile << "\\hline" << endl;
  
  if (print_info==1) cout << " DATA/MC " << IChannel << endl; 
  ofile.precision(indic_precision);
  for(int ic=2; ic<CutName.size(); ++ic) {
   // no need to print "Total" and "Trigger" lines
   if (IChannel<2) {
     ofile << CutName[ic]<<" & " <<  
     cut[ic][jdata[IChannel]] << " $\\pm$ "<<  errcut[ic][jdata[IChannel]] << " & " <<
     cut[ic][jmcsum[IChannel]]  << " $\\pm$ "<<  errcut[ic][jmcsum[IChannel]] << " & " <<
     cut[ic][jtopsig[IChannel]] << " $\\pm$ "<<  errcut[ic][jtopsig[IChannel]] << " & " <<
     cut[ic][jmcbg[IChannel]]   << " $\\pm$ "<<  errcut[ic][jmcbg[IChannel]] << " & " <<
     cut[ic][jtopsig[IChannel]]/cut[ic][jmcbg[IChannel]] <<  " \\\\" << endl;

     if (print_info==1 && cut[ic][jmcsum[IChannel]]>0) cout << CutName[ic]<<"  " << cut[ic][jdata[IChannel]]/cut[ic][jmcsum[IChannel]] << endl;
   }
   else { //emu
    if (ic!=3 && ic!=5) {
     ofile << CutName2[ic]<<" & " <<  
     cut[ic][jdata[IChannel]] << " $\\pm$ "<<  errcut[ic][jdata[IChannel]] << " & " <<
     cut[ic][jmcsum[IChannel]]  << " $\\pm$ "<<  errcut[ic][jmcsum[IChannel]] << " & " <<
     cut[ic][jtopsig[IChannel]] << " $\\pm$ "<<  errcut[ic][jtopsig[IChannel]] << " & " <<
     cut[ic][jmcbg[IChannel]]   << " $\\pm$ "<<  errcut[ic][jmcbg[IChannel]] << " & " <<
     cut[ic][jtopsig[IChannel]]/cut[ic][jmcbg[IChannel]] <<  " \\\\" << endl;

     if (print_info==1 && cut[ic][jmcsum[IChannel]]>0) cout << CutName2[ic]<<"  " << cut[ic][jdata[IChannel]]/cut[ic][jmcsum[IChannel]] << endl;
    }
   }
  }
  ofile.precision(3);
  ofile << "\\hline" << endl;
  ofile << "$SF_{trig}$  & " << 
      sf1[jdata[IChannel]] <<  " & " <<
      sf1[jmcsum[IChannel]] <<  " & " <<
      sf1[jtopsig[IChannel]] <<  " & " <<
      sf1[jmcbg[IChannel]] <<  " &   \\\\" << endl;
  ofile << "$SF_{ID}$  & " << 
      sf2[jdata[IChannel]] <<  " & " <<
      sf2[jmcsum[IChannel]] <<  " & " <<
      sf2[jtopsig[IChannel]] <<  " & " <<
      sf2[jmcbg[IChannel]] <<  " &   \\\\" << endl;
  ofile << "\\hline" << endl;
  ofile << "\\hline" << endl;
  ofile << "\\end{tabular}" << endl;
  ofile << "\\begin{tabular}{|l|c|c|c|c|c|}" << endl;
  ofile << "\\hline" << endl;
  ofile << "\\hline" << endl;
  ofile.precision(indic_precision);
  ofile << "Cut & TopBackg & SingleTop & DY & Wjets  & Diboson  \\\\" << endl;
  ofile << "\\hline" << endl;
 
  for(int ic=2; ic<CutName.size(); ++ic) {
    // no need to print "Total" and "Trigger" lines
    if (IChannel<2) {
     ofile <<CutName[ic]<<" & "<<
     cut[ic][jtopbg[IChannel]] << " $\\pm$ "<<  errcut[ic][jtopbg[IChannel]] << " & " <<
     cut[ic][jsingle[IChannel]]  << " $\\pm$ "<<  errcut[ic][jsingle[IChannel]] << " & " <<
     cut[ic][jdy1[IChannel]] << " $\\pm$ "<<  errcut[ic][jdy1[IChannel]] << " & " <<
     cut[ic][jwjet[IChannel]]   << " $\\pm$ "<<  errcut[ic][jwjet[IChannel]] << " & " <<
     cut[ic][jdiboson[IChannel]]   << " $\\pm$ "<<  errcut[ic][jdiboson[IChannel]] << " \\\\" << endl;
    }
    else { // emu
    if (ic!=3 && ic!=5) {
     ofile <<CutName2[ic]<<" & "<<
     cut[ic][jtopbg[IChannel]] << " $\\pm$ "<<  errcut[ic][jtopbg[IChannel]] << " & " <<
     cut[ic][jsingle[IChannel]]  << " $\\pm$ "<<  errcut[ic][jsingle[IChannel]] << " & " <<
     cut[ic][jdy1[IChannel]] << " $\\pm$ "<<  errcut[ic][jdy1[IChannel]] << " & " <<
     cut[ic][jwjet[IChannel]]   << " $\\pm$ "<<  errcut[ic][jwjet[IChannel]] << " & " <<
     cut[ic][jdiboson[IChannel]]   << " $\\pm$ "<<  errcut[ic][jdiboson[IChannel]] << " \\\\" << endl;
    }
   } 
  }
  
  ofile.precision(3);
  ofile << "\\hline " << endl;
  ofile << "$SF_{trig}$  & " <<
      sf1[jtopbg[IChannel]] <<  " & " <<
      sf1[jsingle[IChannel]] <<  " & " <<
      sf1[jdy1[IChannel]] <<  " & " <<
      sf1[jwjet[IChannel]] <<  " & " <<
      sf1[jdiboson[IChannel]] <<  " \\\\" << endl;
  ofile << "$SF_{ID}$  & " << 
      sf2[jtopbg[IChannel]] <<  " & " <<
      sf2[jsingle[IChannel]] <<  " & " <<
      sf2[jdy1[IChannel]] <<  " & " <<
      sf2[jwjet[IChannel]] <<  " & " <<
      sf2[jdiboson[IChannel]] <<  " \\\\" << endl;
  if (IChannel==0) ofile << "$SF_{DD}$  &  &  & " << SF_DY_mm << " & " << SF_Wjets_mm << " &   \\\\" << endl;
  else if (IChannel==1) ofile << "$SF_{DD}$  &  &  & " << SF_DY_ee << " & " << SF_Wjets_ee << " &  \\\\" << endl;
  else if (IChannel==2) ofile << "$SF_{DD}$  &  &  & " << SF_DY_em << " & " << SF_Wjets_em << " &  \\\\" << endl;
  ofile << "\\hline " << endl;
  ofile << "\\hline" << endl;
  ofile << "\\end{tabular}" << endl;
  
  if (IChannel==0) ofile << "\\caption{ Dimuon cut flow }" << endl;
  else if (IChannel==1) ofile << "\\caption{ Dielectron cut flow }" << endl;
  else if (IChannel==2) ofile << "\\caption{ ElectronMuon cut flow }" << endl;
  ofile << "\\label{Table:CutFlow}" << endl;
  ofile << "\\end{table}" << endl;
  } // end loop IChannel
  
  
  
  ofile<<"\\end{document}"<<endl;
  string prodpdf = string("pdflatex ")+ofilenametex;
  system(prodpdf.c_str());
  
    cout<<"#########################"<<endl;
    cout<<"    End of the program   "<<endl;
    cout<<"#########################"<<endl;
  
  return (0);
}
