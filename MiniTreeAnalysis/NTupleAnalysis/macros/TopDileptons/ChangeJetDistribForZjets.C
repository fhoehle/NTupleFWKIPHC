#include "TFile.h"
#include "TH2D.h"
#include "TGraphErrors.h"
#include <string>
#include <vector>
#include <sstream>

using namespace std;

string convertInt(int number)
{
   stringstream ss;//create a stringstream
   ss << number;//add number to the stream
   return ss.str();//return a string with the contents of the stream
}

void Load(){

	string varname = string("NjetsNBjets_6_0.244");

	TFile* fin = new TFile("outfile_test.root","UPDATE");

	//channels
	vector<string> channels;
	channels.push_back(string("ee"));	
	channels.push_back(string("mumu"));	
	channels.push_back(string("emu"));	


	vector<string> systs;
	systs.push_back(string("JES"));
	systs.push_back(string("EES"));
	systs.push_back(string("MES"));


	vector<float> norm_ee;
	norm_ee.push_back(102.);//2jets	
	norm_ee.push_back(103.);//3jets	
	norm_ee.push_back(104.);//4jets	
	
	vector<float> norm_mumu;	
	norm_mumu.push_back(202.);//2jets	
	norm_mumu.push_back(203.);//3jets	
	norm_mumu.push_back(204.);//4jets	
	
	vector<float> norm_emu;	
	norm_emu.push_back(302.);//2jets	
	norm_emu.push_back(303.);//3jets	
	norm_emu.push_back(304.);//4jets	

	vector<string> sf;
	sf.push_back(string("sfb"));
	sf.push_back(string("sfl"));

	vector<string> jmulbmul;
	jmulbmul.push_back(string("2_0"));
	jmulbmul.push_back(string("2_1"));
	jmulbmul.push_back(string("2_2"));
	jmulbmul.push_back(string("3_0"));
	jmulbmul.push_back(string("3_1"));
	jmulbmul.push_back(string("3_2"));
	jmulbmul.push_back(string("3_3"));
	jmulbmul.push_back(string("4_0"));
	jmulbmul.push_back(string("4_1"));
	jmulbmul.push_back(string("4_2"));
	jmulbmul.push_back(string("4_3"));
	jmulbmul.push_back(string("4_4"));


	for(unsigned int i=0;i<channels.size();i++){
		for(int njets=2;njets<5;njets++){
			//reference plots
			string iname = channels[i]+"_"+varname+"_Zjets";
			string outname = channels[i]+"_"+varname+"_Zjets"+convertInt(njets);
			TH2D* href = (TH2D*) fin->Get(iname.c_str())->Clone("");
			
			int ibinRef = njets+1;
			//Set to zero all bins except the ones corresponding to the ibinRef jets mult.
			for(unsigned int xbin=3;xbin<6;xbin++){
				for(unsigned int ybin=1;ybin<6;ybin++){
					if((int)xbin!=ibinRef){
						href->SetBinContent(xbin,ybin,0.);
					}
				}
			}
			//Rescale the histo
			if(channels[i] == string("ee")) href->Scale(norm_ee[njets-2]/href->Integral());	
			if(channels[i] == string("emu")) href->Scale(norm_emu[njets-2]/href->Integral());	
			if(channels[i] == string("mumu")) href->Scale(norm_mumu[njets-2]/href->Integral());	
			href->SetName(outname.c_str());
			href->Write();
	
			for(unsigned int j=0;j<systs.size();j++){
				//minus
				string systname = systs[j]+string("-minus");
				iname = channels[i]+"_"+varname+"_"+systname+"_Zjets";
				outname = channels[i]+"_"+varname+"_"+systname+"_Zjets"+convertInt(njets);
				TH2D* hsystMinus = (TH2D*) fin->Get(iname.c_str())->Clone("");
				ibinRef =  njets+1;
				//Set to zero all bins except the ones corresponding to the ibinRef jets mult.
				for(unsigned int xbin=3;xbin<6;xbin++){
					for(unsigned int ybin=1;ybin<6;ybin++){
						if((int)xbin!=ibinRef){
							hsystMinus->SetBinContent(xbin,ybin,0.);
						}
					}
				}
				//Rescale the histo
				if(channels[i] == string("ee")) hsystMinus->Scale(norm_ee[njets-2]/hsystMinus->Integral());	
				if(channels[i] == string("emu")) hsystMinus->Scale(norm_emu[njets-2]/hsystMinus->Integral());	
				if(channels[i] == string("mumu")) hsystMinus->Scale(norm_mumu[njets-2]/hsystMinus->Integral());	
				hsystMinus->SetName(outname.c_str());
				hsystMinus->Write();

				//plus
				systname = systs[j]+string("-plus");
				iname = channels[i]+"_"+varname+"_"+systname+"_Zjets";
				outname = channels[i]+"_"+varname+"_"+systname+"_Zjets"+convertInt(njets);
				TH2D* hsystPlus = (TH2D*) fin->Get(iname.c_str())->Clone("");
				//Set to zero all bins except the ones corresponding to the ibinRef jets mult.
				for(unsigned int xbin=3;xbin<6;xbin++){
					for(unsigned int ybin=1;ybin<6;ybin++){
						if((int)xbin!=ibinRef){
							hsystPlus->SetBinContent(xbin,ybin,0.);
						}
					}
				}
				//Rescale the histo
				if(channels[i] == string("ee")) hsystPlus->Scale(norm_ee[njets-2]/hsystPlus->Integral());	
				if(channels[i] == string("emu")) hsystPlus->Scale(norm_emu[njets-2]/hsystPlus->Integral());	
				if(channels[i] == string("mumu")) hsystPlus->Scale(norm_mumu[njets-2]/hsystPlus->Integral());	
				hsystPlus->SetName(outname.c_str());
				hsystPlus->Write();
			}
				
			for(unsigned int l=0;l<jmulbmul.size();l++){
				for(unsigned int m=0;m<sf.size();m++){
					iname = channels[i]+"_"+varname+"_"+sf[m]+"_"+jmulbmul[l]+"_Zjets";
					outname = channels[i]+"_"+varname+"_"+sf[m]+"_"+jmulbmul[l]+"_Zjets"+convertInt(njets);
					TGraphErrors* graph = (TGraphErrors*) fin->Get(iname.c_str())->Clone("");
					graph->SetName(outname.c_str());
					graph->Write();
				}
			}			


			
		}

	}

	fin->Close();
	

}
