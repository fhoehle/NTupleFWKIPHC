import ROOT
ROOT.TH1.AddDirectory(False)
file_mc6134="../backup_outputProof_13-04-16_16-34-23_mcatnlo6134_noTauPolReweightin_tauWeights.root"
file_mc5217="../backup_outputProof_13-04-16_18-46-00_Mcatnlo5217_TauPolSys_FullSim_noTauPolReweighting_completeSample_tauWeights.root"
histname = "tauPol_lep_TTbarSig;1"
can = ROOT.TCanvas("compareTauPol","",200,10,700,500)
can.cd()
colors = [1,3]
hists=[]
for i,filename in enumerate([file_mc6134,file_mc5217]):
  file = ROOT.TFile(filename)
  hist = file.Get(histname)
  hist.SetLineColor(colors[i])
  hist.DrawCopy("" if i == 0 else "sames")
  setattr(hist,"fileFrom",filename)
  hists.append(hist)
postname = ""
def saveCan():
  can.SaveAs(can.GetName()+( ("_"+postname) if postname != "" else postname )+".pdf")
