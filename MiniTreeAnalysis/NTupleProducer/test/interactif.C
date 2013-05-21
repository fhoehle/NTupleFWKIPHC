{
 TFile* file = new TFile("NTuple.root");
 TTree* tree = (TTree*) file->Get("MyModule/Event");
 //here on can:
 // access the data members like here: NTEvent.muons.NTrValidHits
 // access the methods like here: NTEvent.muons.p4.Pt()
 tree->Draw("NTEvent.muons.NTrValidHits","NTEvent.muons.p4.Pt()>20 && NTEvent.muons.p4.Eta()<2.5");
}
