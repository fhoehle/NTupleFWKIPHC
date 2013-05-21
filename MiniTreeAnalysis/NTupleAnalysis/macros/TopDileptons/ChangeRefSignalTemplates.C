#include <iostream>
#include <string>
#include <TFile.h>
#include <TH2D.h>

using namespace std;


//copy histo1 en histo2 (histo ecraser)
void CopyTH2D(TH2D* histo1, TH2D* histo2, float scale = 1.){
	for(int i=1;i<histo1->GetNbinsX()+1;i++){
		for(int j=1;j<histo1->GetNbinsX()+1;j++){
			histo1->SetBinContent(i,j,histo2->GetBinContent(i,j));
		}
	}
	histo1->Scale(scale);
}

void ChangeRefSignalTemplatesOnSyst(string ofilename, string systfilename){


	string varname = string("NjetsNBjets_6_0.244");

 	//TFile* ifile = TFile::Open("outfile_TemplateNormalizedNewJESParam_ScaleUp.root","UPDATE");
 	TFile* ifile = TFile::Open(ofilename.c_str(),"UPDATE");
	//TFile* ifileSyst = TFile::Open("Template_TTbarScaleUp.root","OPEN");
	TFile* ifileSyst = TFile::Open(systfilename.c_str(),"OPEN");
	//float scale = 3631452./912748;
	float scale = 1.;


	//conditions
	vector<string> syst;
	syst.push_back("JES-plus");
	syst.push_back("JES-minus");

	//channels
	vector<string> channels;
	channels.push_back(string("ee"));	
	channels.push_back(string("mumu"));	
	channels.push_back(string("emu"));	

	//process
	vector<string> processes;
	processes.push_back(string("TTbarSig"));
	processes.push_back(string("TTbarBkg"));


	TH2D* histo1 = 0;
	TH2D* histo2 = 0;

	for(unsigned int i=0;i<channels.size();i++){
		for(unsigned int j=0;j<processes.size();j++){

			string hname = channels[i]+"_"+varname+"_"+processes[j];
			histo1 = ((TH2D*)ifile->Get(hname.c_str()));
			histo2 = ((TH2D*)ifileSyst->Get(hname.c_str()));
			CopyTH2D(histo1,histo2,scale);
			ifile->cd();
			histo1->Write();
			
			for(unsigned int k=0;k<syst.size();k++){
				hname = channels[i]+"_"+varname+"_"+syst[k]+"_"+processes[j];
				histo1 = ((TH2D*)ifile->Get(hname.c_str()));
				histo2 = ((TH2D*)ifileSyst->Get(hname.c_str()));
				CopyTH2D(histo1,histo2,scale);
				ifile->cd();
				histo1->Write();
			}

		}
	}
	ifile->Write();
	ifile->Close();
}

void Compute(string reffilename, string systname){
	cout<<"################ Computation for "<<systname<<endl;
	string ofilename = reffilename+"_"+systname+".root";
	string command = "cp "+reffilename+" "+ofilename;
	system(command.c_str());
	string systfilename = "newp_Tbar"+systname+".root";
 	ChangeRefSignalTemplatesOnSyst(ofilename, systfilename);
}

void ChangeRefSignalTemplates(){ 
	string reffilename("templates_PLR_wWjets_2.root");

	Compute(reffilename,string("ScaleDown"));
	Compute(reffilename,string("ScaleUp"));
	Compute(reffilename,string("MatchDown"));
	Compute(reffilename,string("MatchDown"));
	Compute(reffilename,string("PUdown"));
	Compute(reffilename,string("PUup"));
	Compute(reffilename,string("Mass161"));
	Compute(reffilename,string("Mass163"));
	Compute(reffilename,string("Mass166"));
	Compute(reffilename,string("Mass169"));
	Compute(reffilename,string("Mass178"));
	Compute(reffilename,string("Mass181"));
	Compute(reffilename,string("Mass184"));
	Compute(reffilename,string("TriggerDown"));
	Compute(reffilename,string("TriggerUp"));
	Compute(reffilename,string("LeptDown"));
	Compute(reffilename,string("LeptUp"));
	Compute(reffilename,string("METDown"));
	Compute(reffilename,string("METUp"));


}
