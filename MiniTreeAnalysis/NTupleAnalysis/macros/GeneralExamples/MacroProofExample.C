#include <TApplication.h>
#include <TGClient.h>
#include <TProof.h>
#include <TChain.h>
#include <TFileCollection.h>
#include <TDrawFeedback.h>

#include "../../Tools/interface/Dataset.h"
#include "../../Tools/interface/AnalysisEnvironmentLoader.h"

int main(int argc, char* argv[]){


TApplication theApp("App",&argc,argv);

//---------------------------------------//
// Global variables: could be give as argument later
//---------------------------------------//

int nwnodes = 10; //8 to 10 is the optimal
string macroName = "ProofSelectorExample.C+"; //"+" should be put at the end to use ACLIC complication - This macro should inherit from TSelector 
//In order to allow the node to access the xml, the name should be given with the full path
string xmlFileName = string ("/opt/sbg/data/data1/cms/echabert/TopDilep/PLR/CMSSW_4_2_3/src/MiniTreeAnalysis/NTupleAnalysis/config/MyCutFlow.xml");
string outputFileName = "proof.root";

//---------------------------------------//
//	Decleration of TProof
//---------------------------------------//

//to be done before colling TProof
system("./clean_proof.sh ; echo 'Wait few seconds ... ' ; sleep 6");
system("rm -r $HOME/.proof");

TProof *proof = TProof::Open("");
proof->SetParallel(nwnodes);
//you should not have any package yet
proof->ShowPackages();
//proof->ClearPackages();
//Loading package related to NTupleAnalysis
cout<<" ## Upload package NTAna.par: ";
proof->UploadPackage("../NTAna.par");
cout<<" DONE [don't worry with symlink error - do rm NTAna if you change NTAna.par in the meanwhile !] "<<endl;
proof->EnablePackage("NTAna");
//Adding histograms for feedback: must exist in the TSelector !
proof->AddFeedback("fHist"); //give the "name" of the histogram and not the name of the variable TH1F* (could be the same !)

//This line is required to display histograms durint the process
TDrawFeedback fb(proof);


//---------------------------------------//
// Xml Loading  & Dataset registration
//---------------------------------------//

vector < Dataset > datasets;
AnalysisEnvironmentLoader anaEL (xmlFileName);
anaEL.LoadSamples (datasets); // now the list of datasets written in the xml file is known

cout<<" #------------------------------------# "<<endl;
cout<<" PROOF DATASETS SUMMARY [normaly 0]"<<endl;
proof->ShowDataSets();
cout<<" #------------------------------------# "<<endl;
cout<<" # Registring dataset ... "<<endl;
cout<<" Don't be worry with the checksum error message [at least I'm not ;-) ]"<<endl;
cout<<" #------------------------------------# "<<endl;
//Create datasets in proof format
TFileCollection** fileCollec = new TFileCollection*[datasets.size()];
for(unsigned int i=0;i<datasets.size();i++){
	fileCollec[i]  = new TFileCollection(datasets[i].Name().c_str(),"");
	for(unsigned int j=0;j<datasets[i].Filenames().size();j++){
		fileCollec[i]->Add(datasets[i].Filenames()[j].c_str());
	}
	//register dataset in proof
	proof->RegisterDataSet(datasets[i].Name().c_str(),fileCollec[i]);
	proof->VerifyDataSet(datasets[i].Name().c_str());
}

//summarize the list of datasets
cout<<" #------------------------------------# "<<endl;
cout<<" PROOF DATASETS SUMMARY"<<endl;
proof->ShowDataSets();
cout<<" #------------------------------------# "<<endl;

//---------------------------------------//
// 	Loading of the xml file
//---------------------------------------//
//Possibility to give float ... ex:
//Double_t f = 3.14;
//proof->SetParameter("IN_FLOAT",f);

proof->AddInput(new TNamed("PROOF_XMLFILENAME", xmlFileName));
proof->AddInput(new TNamed("PROOF_OUTPUTFILE", outputFileName));
//summary of the parameters
cout<<" #------------------------------------# "<<endl;
cout<<" PROOF PARAMETERS SUMMARY"<<endl;
proof->ShowParameters();
cout<<" #------------------------------------# "<<endl;

//---------------------------------------//
// 	Processing of the datasets
//---------------------------------------//

string outputFileNameModif = outputFileName.substr(0,outputFileName.size()-5);
string MergingCommand = "hadd -f "+outputFileNameModif+"_merged.root ";

for(unsigned int i=0;i<datasets.size();i++){
	proof->AddInput(new TNamed("PROOF_DATASETNAME", datasets[i].Name()));
	cout<<"################################################################"<<endl;
	cout<<"########### Processing the dataset "<<datasets[i].Name()<<endl;
	cout<<"################################################################"<<endl;
	//proof->Process(datasets[i].Name().c_str(),macroName.c_str());
	proof->Process(datasets[i].Name().c_str(),macroName.c_str());
	string newFileName = outputFileNameModif+"_"+datasets[i].Name()+".root";
	cout<<"Copying the output file with the name "<<endl;
	string command = "cp "+outputFileName+" "+newFileName;
	MergingCommand+=newFileName+" ";
	system(command.c_str());
	//cout<<datasets[i].eventTree()->Process(macroName.c_str())<<endl;
}

cout<<"## Merging of all the dataset into one single file with hadd: "<<outputFileName<<endl;
system(MergingCommand.c_str());
cout<<"#######################      DONE      #########################"<<endl;
cout<<"################################################################"<<endl;

return (0);

}



