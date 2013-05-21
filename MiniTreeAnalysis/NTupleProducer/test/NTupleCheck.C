{
// Aim: checking empty leaves in a TTree & and leaves filled with a unique value.
// 	problems are dumped on the screen and on a file
// 	one bin histos are plotted on a TCanvas


	bool verbose = true;
	bool printOut = true;
	ofstream ofile("AnaTree.txt");
        //For NTuple ...
	char filename[] = "NTuple.root";
	char treeName[] = "MyModule/Event";
        //For MiniTre ...
	//char filename[] = "../../../TopQuarkAnalysis/MiniTreeProducer/test/MiniTree.root";
	//char treeName[] = "Events";

	vector<TString> EmptyLeaves;
	vector<TString> OneBinLeaves;
	
	TFile* file = new TFile(filename,"READ");
 	TTree* tree = (TTree*) file->Get(treeName);
	TObjArray* listBranch = tree->GetListOfBranches();
	TH1* histo = 0;
	cout<<"Nof of branches in this tree: "<<listBranch->GetEntries()<<endl;
	for(int ib=0;ib<listBranch->GetEntries();ib++){
		TBranch* b = (TBranch*) listBranch->At(ib);
		if(verbose){
			cout<<"Branch "<<b->GetName()<<endl;
			cout<<" - its leaves: "<<endl;
		}
		TObjArray* listLeaves = tree->GetListOfLeaves();
		for(int i=0;i<listLeaves->GetEntries();i++){
			TLeaf* l = (TLeaf*) listLeaves->At(i);
			if(verbose) cout<<" -- Leave: "<<l->GetName()<<endl;
			if(l->IsOnTerminalBranch()){
				TString s = TString(b->GetName())+"."+TString(l->GetName());
				tree->Draw(s.Data());
				histo = tree->GetHistogram();
				if(verbose){
					cout<<"Entries: "<<histo->GetEntries()<<endl;
					cout<<"Maximum: "<<histo->GetMaximum()<<endl;
				}
				if(histo->GetEntries()==0) EmptyLeaves.push_back(s);
				if(histo->GetMaximum()==histo->GetEntries() && histo->GetEntries()>0) OneBinLeaves.push_back(s);
				
			}
		}
	}
	cout<<"+++++++++++++++++++++++++"<<endl;
	cout<<"	List of problems:       "<<endl;
	cout<<"+++++++++++++++++++++++++"<<endl;
	cout<<"	* Empty leaves:"<<endl<<endl;
	for(unsigned int x=0;x<EmptyLeaves.size();x++){
		cout<<" - "<<EmptyLeaves[x]<<endl;
	}
	cout<<endl;
	cout<<"	* One bin leaves:"<<endl<<endl;
	for(unsigned int x=0;x<OneBinLeaves.size();x++){
		cout<<" - "<<OneBinLeaves[x]<<endl;
	}
	if(printOut){
		ofile<<"+++++++++++++++++++++++++"<<endl;
		ofile<<"	List of problems:       "<<endl;
		ofile<<"+++++++++++++++++++++++++"<<endl;
		ofile<<"	* Empty leaves:"<<endl<<endl;
		for(unsigned int x=0;x<EmptyLeaves.size();x++){
			ofile<<" - "<<EmptyLeaves[x]<<endl;
		}
		ofile<<endl;
		ofile<<"	* One bin leaves:"<<endl<<endl;
		for(unsigned int x=0;x<OneBinLeaves.size();x++){
			ofile<<" - "<<OneBinLeaves[x]<<endl;
		}
	}
	cout<<"Plotting those histos ..."<<endl;
	int x1_ = 1;
	int x2_ = 1;
	for(int x1=1;x1<10;x1++){
		if(x1*(x1) >=(int)OneBinLeaves.size()){
			x1_ = x1;
			x2_ = x1;
			break;
		}
		if(x1*(x1+1) >=(int)OneBinLeaves.size()){
			x1_ = x1;
			x2_ = x1+1;
			break;
		}
	}
	TCanvas* cDebug = new TCanvas("cOneBinHistos");
	cDebug->Divide(x1_,x2_);
	for(unsigned int x=0;x<OneBinLeaves.size();x++){
		cDebug->cd(x+1);
		tree->Draw(OneBinLeaves[x]);
	}
}
