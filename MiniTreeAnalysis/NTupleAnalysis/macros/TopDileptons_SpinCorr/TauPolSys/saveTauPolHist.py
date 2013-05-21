import ROOT,getopt,sys
opts, args = getopt.getopt(sys.argv[1:], '',['inputFile=','outputFile='])
inputFile=None; outputFile=None;
tauPolLepEnergyDist = ROOT.TF1("tauPolLepEnergyDist","(5.0/3.0 - 3*x*x + 4.0/3.0*x*x*x + [0]*(-1.0/3.0 + 3*x*x - 8.0/3.0 *x*x*x))",0,1);
tauPolLepEnergyDist.SetParameter(0,1)
file_mc6134="backup_outputProof_13-04-16_16-34-23_mcatnlo6134_noTauPolReweighting/proof.root"
file_mc5217="backup_outputProof_13-04-16_18-46-00_Mcatnlo5217_TauPolSys_FullSim_completeSample/proof.root"
print ""
for opt,arg in opts:
 #print opt , " :   " , arg
 if opt in  ("--inputFile"):
  inputFile=arg
 if opt in ("--outputFile"):
  outputFile=arg
print "inputFile ",inputFile," outputFile ",outputFile
file = ROOT.TFile(inputFile)

histname = "tauPol_lep_TTbarSig;1"
hist = file.Get(histname)
hist.Sumw2();hist.Scale(1.0/hist.Integral("width"))

#hist.SaveAs(outputFile)
can = ROOT.TCanvas("can","",200,10,700,500)
can.cd(1)
tauPolLepEnergyDist.Draw()
hist.Draw("same")
can.SaveAs("test.pdf")
ROOT.TH1.AddDirectory(False)
outputFile = ROOT.TFile(outputFile,"RECREATE")
outputFile.cd()
tauPolLepWeights = hist.Clone("tauPolLepWeights")
for i in range(1,hist.GetNbinsX()+1):
  tauPolLepWeights.SetBinContent(i,tauPolLepEnergyDist.Eval(hist.GetBinCenter(i))/hist.GetBinContent(i))
hist.Write()
tauPolLepWeights.Write()
tauPolLepEnergyDist.Write()
outputFile.Close()
