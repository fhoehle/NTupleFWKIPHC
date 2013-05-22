import ROOT,sys
sys .path.append('/grid_mnt/opt__sbg__data__safe1/cms/fhoehle/PyRoot_Helpers/')
import PyRoot_Functions.MyHistFunctions_cfi as MyHistFunctions_cfi

cut = "_afterbtag1_TTbarSig;1"
channels=['ee','emu','mumu']
HistsFileWithSC = ['DeltaPhiLLept_','DeltaPhiLLept_topPtReweighted_']; myColors = dict([(histType,i+2) for i,histType in enumerate(HistsFileWithSC)])
fileWithSC = ROOT.TFile("backup_outputProof_13-05-19_04-10-30_Mcatnlo5217_TopPtWeighting_FullSim_completeSample/proof_backup_outputProof_13-05-19_04-10-30_Mcatnlo5217_TopPtWeighting_FullSim_completeSample.root")

HistsFileNoSC = HistsFileWithSC
fileNoSC = ROOT.TFile("backup_outputProof_13-05-19_12-19-27_Mcatnlo6134_TopPtWeighting_FullSim_completeSample/proof_backup_outputProof_13-05-19_12-19-27_Mcatnlo6134_TopPtWeighting_FullSim_completeSample.root" )

ROOT.TH1.AddDirectory(False)
histsSC = dict( ( hist, dict ( (channel,fileWithSC.Get(hist+channel+cut)) for channel in channels )) for hist in HistsFileWithSC )
#for hist in HistsFileWithSC:
#  histsSC.append([fileWithSC.Get(hist+channel+cut) for channel in channels])

histsNoSC = []
for hist in HistsFileNoSC:
  histsNoSC.append([fileNoSC.Get(hist+channel+cut) for channel in channels])

import PyRoot_Functions.StatBoxFunctions_cfi as StatBoxFunctions_cfi
canSC = ROOT.TCanvas("canSC","with SC",200,10,700,500)
canSC.cd()
chTested = 'ee'
histType_iter = histsSC.iteritems()
histType,hists = histType_iter.next()
hist = hists[chTested];hist.SetLineColor(myColors[histType]);
hist.Draw();StatBoxFunctions_cfi.StatBoxSameLineColor(hist)
for (histtype , hists) in histType_iter:
    hist = hists[chTested]
    hist.SetLineColor(myColors[histtype]);
    hist.Draw("sames")
    StatBoxFunctions_cfi.StatBoxSameLineColor(hist)
import PyRoot_Functions.PadFunctions_cfi as PadFunctions_cfi
PadFunctions_cfi.adaptMaxMinOfPad()
canSC.SaveAs(canSC.GetName()+".pdf")

