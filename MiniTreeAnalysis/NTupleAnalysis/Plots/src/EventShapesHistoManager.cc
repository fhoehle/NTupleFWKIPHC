#include "../interface/EventShapesHistoManager.h"




EventShapesHistoManager::EventShapesHistoManager(){
}

EventShapesHistoManager::~EventShapesHistoManager(){
}


void EventShapesHistoManager::CreateHistos(){
	AddHisto(string("Aplanarity"),string("Aplanarity"),string("Aplanarity"),50,0,1.);
	AddHisto(string("Sphericity"),string("Sphericity"),string("Sphericity"),25,0,1);
	AddHisto(string("Circularity"),string("Circularity"),string("Circularity"),50,0,1);
	AddHisto(string("Isotropy"),string("Isotropy"),string("Isotropy"),50,0,1);
	AddHisto(string("C"),string("C"),string("C"),50,0,1);
	AddHisto(string("D"),string("D"),string("D"),50,0,1);
	AddHisto(string("Ht"),string("HT"),string("H_{T}"),50,0,1500);
	AddHisto(string("H"),string("H"),string("H"),200,0,1000);
	AddHisto(string("sqrts"),string("M(#tau,jets)"),string("M(#tau,jets)"),100,0,2000);
	AddHisto(string("NNOutput"),string("NNOutput"),string("NNOutput"),30,-1.5,1.5);
	AddHisto(string("Chi2"),string("Chi2"),string("Chi2"),200,0,1000);
	AddHisto(string("Chi2zoom"),string("Chi2zoom"),string("Chi2"),40,0,20);
	AddHisto(string("Chi2NN"),string("Chi2 NN>0.5"),string("Chi2 NN>0.5"),200,0,1000);
	AddHisto(string("HtNN"),string("H_{T} NN>0.5"),string("H_{T} NN>0.5"),50,0,1500);
	AddHisto(string("AplanarityNN"),string("Aplanarity NN>0.5"),string("Aplanarity NN>0.5"),50,0,1.);
	AddHisto(string("SphericityNN"),string("Sphericity NN>0.5"),string("Sphericity NN>0.5"),25,0,1);
	AddHisto(string("CircularityNN"),string("Circularity NN>0.5"),string("Circularity NN>0.5"),50,0,1);
	AddHisto(string("M3Chi2"),string("M3Chi2"),string("M3Chi2"),200,0,1000);
	AddHisto(string("M2Chi2"),string("M2Chi2"),string("M2Chi2"),200,0,1000);
	AddHisto(string("M3Chi2NN"),string("M3Chi2 NN>0.5"),string("M3Chi2 NN>0.5"),200,0,1000);
	AddHisto(string("M2Chi2NN"),string("M2Chi2 NN>0.5"),string("M2Chi2 NN>0.5"),200,0,1000);
	AddHisto(string("CNN"),string("C NN>0.5"),string("C NN>0.5"),50,0,1);
	AddHisto(string("DNN"),string("D NN>0.5"),string("D NN>0.5"),50,0,1);
	AddHisto(string("sqrtsNN"),string("M(#tau,jets) NN>0.5"),string("M(#tau,jets) NN>0.5"),100,0,2000);
	AddHisto(string("kinFitChi2"),string("kinFitChi2"),string("kinFit #chi_2"),200,-100,100);
	AddHisto(string("kinFitTopMass"),string("kinFit top mass"),string("kinFit top mass"),150,-100,500);
	AddHisto(string("kinFitChi2NN"),string("kinFitChi2 NN>0.5"),string("kinFit #chi_2 NN>0.5"),200,-100,100);
	AddHisto(string("kinFitTopMassNN"),string("kinFit top mass NN>0.5"),string("kinFit top mass NN>0.5"),150,-100,500);
	AddHisto(string("kinFitTopMassChi2"),string("kinFit top mass chi2<1"),string("kinFit top mass #chi_2<1"),150,-100,500);
	
}

void EventShapesHistoManager::Fill(const vector<NTJet>& candJet, const vector<NTJet>& candJetTauClean, const int& maxSelStep, const int&
iChannel, const int& iDataset, const float& weight, const vector<float>& NNOutput){
	if(!Check(iChannel, iDataset)) return;
	for(unsigned int i=0;i<SelectionSteps.size();i++){
		if(maxSelStep>=(int)i) FillSelStep(candJet, candJetTauClean, i, iChannel, iDataset, weight, NNOutput);	
	}	
}

void EventShapesHistoManager::FillSelStep(const vector<NTJet>& candJet, const vector<NTJet>& candJetTauClean, const int& iSelStep, const int&
iChannel, const int& iDataset, const float& weight, const vector<float>& NNOutput){
        if(!Check(iChannel, iSelStep, iDataset, 0) ) return;
	EventShapes evShape(candJet);
	Histos[0][iChannel][iSelStep][iDataset].Fill(evShape.aplanarity(),weight);
	Histos[1][iChannel][iSelStep][iDataset].Fill(evShape.sphericity(),weight);
	Histos[2][iChannel][iSelStep][iDataset].Fill(evShape.circularity(),weight);
	Histos[3][iChannel][iSelStep][iDataset].Fill(evShape.isotropy(),weight);
	Histos[4][iChannel][iSelStep][iDataset].Fill(evShape.C(),weight);
	Histos[5][iChannel][iSelStep][iDataset].Fill(evShape.D(),weight);
	Histos[6][iChannel][iSelStep][iDataset].Fill(evShape.HT(),weight);
	Histos[7][iChannel][iSelStep][iDataset].Fill(evShape.H(),weight);
	Histos[8][iChannel][iSelStep][iDataset].Fill(evShape.sqrt_s(),weight);
        Histos[9][iChannel][iSelStep][iDataset].Fill(NNOutput.at(0),weight);
        Histos[10][iChannel][iSelStep][iDataset].Fill(evShape.chi2(candJetTauClean),weight);
        Histos[11][iChannel][iSelStep][iDataset].Fill(evShape.chi2(candJetTauClean),weight);
	
	if (NNOutput.at(0)>0.5) 
	{ 
	Histos[12][iChannel][iSelStep][iDataset].Fill(evShape.chi2(candJetTauClean),weight);
	Histos[13][iChannel][iSelStep][iDataset].Fill(evShape.HT(),weight);
	Histos[14][iChannel][iSelStep][iDataset].Fill(evShape.aplanarity(),weight);
	Histos[15][iChannel][iSelStep][iDataset].Fill(evShape.sphericity(),weight);
	Histos[16][iChannel][iSelStep][iDataset].Fill(evShape.circularity(),weight);
	Histos[19][iChannel][iSelStep][iDataset].Fill(evShape.M3chi2(candJetTauClean),weight);
        Histos[20][iChannel][iSelStep][iDataset].Fill(evShape.M2chi2(candJetTauClean),weight);
        Histos[21][iChannel][iSelStep][iDataset].Fill(evShape.C(),weight);
	Histos[22][iChannel][iSelStep][iDataset].Fill(evShape.D(),weight);
	Histos[23][iChannel][iSelStep][iDataset].Fill(evShape.sqrt_s(),weight);
	Histos[26][iChannel][iSelStep][iDataset].Fill(NNOutput.at(1),weight);
	Histos[27][iChannel][iSelStep][iDataset].Fill(NNOutput.at(2),weight);
	
	}
	
        Histos[17][iChannel][iSelStep][iDataset].Fill(evShape.M3chi2(candJetTauClean),weight);
        Histos[18][iChannel][iSelStep][iDataset].Fill(evShape.M2chi2(candJetTauClean),weight);
	Histos[24][iChannel][iSelStep][iDataset].Fill(NNOutput.at(1),weight);
	Histos[25][iChannel][iSelStep][iDataset].Fill(NNOutput.at(2),weight);
	if(NNOutput.at(1)<1) Histos[28][iChannel][iSelStep][iDataset].Fill(NNOutput.at(2),weight);
	
}
