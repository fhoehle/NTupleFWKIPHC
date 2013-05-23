import ROOT,sys
sys .path.append('/grid_mnt/opt__sbg__data__safe1/cms/fhoehle/PyRoot_Helpers/')
import PyRoot_Functions.MyHistFunctions_cfi as MyHistFunctions_cfi

cut = "_afterbtag1_TTbarSig;1"
channels=['ee','emu','mumu']
HistsFileWithSC = ['DeltaPhiLLept_','DeltaPhiLLept_topPtReweighted_']; 
myScColors = dict([(histType,ROOT.kGreen+(i*2)) for i,histType in enumerate(HistsFileWithSC)])
fileWithSC = ROOT.TFile("backup_outputProof_13-05-19_04-10-30_Mcatnlo5217_TopPtWeighting_FullSim_completeSample/proof_backup_outputProof_13-05-19_04-10-30_Mcatnlo5217_TopPtWeighting_FullSim_completeSample.root")

HistsFileNoSC = HistsFileWithSC
myNoScColors = dict([(histType,ROOT.kRed+(i*2)) for i,histType in enumerate(HistsFileNoSC)])
fileNoSC = ROOT.TFile("backup_outputProof_13-05-19_12-19-27_Mcatnlo6134_TopPtWeighting_FullSim_completeSample/proof_backup_outputProof_13-05-19_12-19-27_Mcatnlo6134_TopPtWeighting_FullSim_completeSample.root" )

ROOT.TH1.AddDirectory(False)
histsSC = dict( ( hist, dict ( (channel,fileWithSC.Get(hist+channel+cut).Clone("SC_"+hist+channel+cut)) for channel in channels )) for hist in HistsFileWithSC )
histsNoSC = dict( (hist, dict( ( channel,fileNoSC.Get(hist+channel+cut).Clone("NOSC_"+hist+channel+cut)) for channel in channels)) for hist in HistsFileNoSC)

import PyRoot_Functions.StatBoxFunctions_cfi as StatBoxFunctions_cfi
cans = []
for chTested in channels:
  canComparison = ROOT.TCanvas("canComparison_"+chTested,"with/without SC and ON/OFF topPt reweighting "+chTested,200,10,700,500)
  cans.append(canComparison)
  canComparison.cd()
  
  histType_iter = histsSC.iteritems()
  histType,hists = histType_iter.next()
  hist = hists[chTested];hist.SetLineColor(myScColors[histType]);
  hist.Draw();StatBoxFunctions_cfi.StatBoxSameLineColor(hist)
  for (histtype , hists) in histType_iter:
      hist = hists[chTested]
      hist.SetLineColor(myScColors[histtype]);
      hist.Draw("sames")
      StatBoxFunctions_cfi.StatBoxSameLineColor(hist)
  for (histtype , hists) in histsNoSC.iteritems():
      hist = hists[chTested]
      hist.SetLineColor(myNoScColors[histtype]);
      hist.Draw("sames")
      StatBoxFunctions_cfi.StatBoxSameLineColor(hist)
  
  import PyRoot_Functions.PadFunctions_cfi as PadFunctions_cfi
  PadFunctions_cfi.adaptMaxMinOfPad()
  import PyRoot_Functions.PadFunctions_cfi as PadFunctions_cfi
  PadFunctions_cfi.statboxesRightSide()
  ROOT.gPad.Modified(); ROOT.gPad.Update()
  canComparison.SaveAs(canComparison.GetName()+".pdf");canComparison.SaveAs(canComparison.GetName()+".root");
