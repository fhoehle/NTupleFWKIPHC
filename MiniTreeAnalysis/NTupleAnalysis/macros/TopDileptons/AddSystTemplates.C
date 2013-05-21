#include <iostream>
#include <string>
#include <TFile.h>
#include <TH2D.h>

using namespace std;

void AddSystTemplatesOneSyst(string systfilename, string systname,string ifilename){




	string varname = string("NjetsNBjets_6_0.244");

 	//TFile* ifile = TFile::Open("templates_PLR_wWjets_2_withAddNuisParam.root","UPDATE");
 	TFile* ifile = TFile::Open(ifilename.c_str(),"UPDATE");
	//TFile* ifileSyst = TFile::Open("Template_TTbarScaleUp.root","OPEN");
	TFile* ifileSyst = TFile::Open(systfilename.c_str(),"OPEN");

	//string systname="Q2-plus";

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

	for(unsigned int i=0;i<channels.size();i++){
		for(unsigned int j=0;j<processes.size();j++){

			string hname = channels[i]+"_"+varname+"_"+processes[j];
			string outname = channels[i]+"_"+varname+"_"+systname+"_"+processes[j];
			histo1 = ((TH2D*)ifileSyst->Get(hname.c_str())->Clone());
			histo1->SetName(outname.c_str());
			ifile->cd();
			cout<<"Write "<<outname<<endl;
			histo1->Write();
			

		}
	}
	ifile->Write();
	ifile->Close();


}


void AddSystTemplates(){
	string ifilename = string("templates_PLR_wWjets_2_withAddNuisParam.root");
	cout<<"####################### Q2-minus  "<<endl;
	AddSystTemplatesOneSyst(string("newp_TTbarScaleDown.root"),string("Q2-minus"),ifilename);
	cout<<"####################### Q2-plus  "<<endl;
	AddSystTemplatesOneSyst(string("newp_TTbarScaleUp.root"),string("Q2-plus"),ifilename);
	cout<<"####################### Match-minus  "<<endl;
	AddSystTemplatesOneSyst(string("newp_TTbarMatchDown.root"),string("match-minus"),ifilename);
	cout<<"####################### Match-plus  "<<endl;
	AddSystTemplatesOneSyst(string("newp_TTbarMatchUp.root"),string("match-plus"),ifilename);
	cout<<"####################### PU-minus  "<<endl;
	AddSystTemplatesOneSyst(string("newp_TTbar_PUdown.root"),string("PU-minus"),ifilename);
	cout<<"####################### PU-plus  "<<endl;
	AddSystTemplatesOneSyst(string("newp_TTbar_PUup.root"),string("PU-plus"),ifilename);
	cout<<"####################### Mtop-minus  "<<endl;
	AddSystTemplatesOneSyst(string("newp_TTbarMass169.root"),string("mtop-minus"),ifilename);
	cout<<"####################### Mtop-plus  "<<endl;
	AddSystTemplatesOneSyst(string("newp_TTbarMass178.root"),string("mtop-plus"),ifilename);

}
