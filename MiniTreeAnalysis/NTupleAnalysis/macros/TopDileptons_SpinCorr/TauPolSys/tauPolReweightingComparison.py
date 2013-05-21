import ROOT,sys
sys.path.append('/opt/sbg/data/safe1/cms/fhoehle/PyRoot_Functions')
import MyHistFunctions_cfi
import MyStyles_cfi
MyStyles_cfi.NormalStyle()
#filenames =['../backup_outputProof_13-04-18_14-35-35_Mcatnlo6134_TauPolSys_FullSim_completeSample_ReweightingOn/proof.root','../backup_outputProof_13-04-18_13-13-57_Mcatnlo6134_TauPolSys_FullSim_completeSample_ReweightingOff/proof.root']; postfix = "_noSC_comparison"
filenames =['../backup_outputProof_13-04-18_09-48-55_Mcatnlo5217_TauPolSys_FullSim_completeSample_ReweightingOn/proof.root','../backup_outputProof_13-04-18_12-26-37_Mcatnlo5217_TauPolSys_FullSim_completeSample_ReweightingOff/proof.root']; postfix = "_SC_comparison"
files = [ROOT.TFile(name) for name in filenames ]
histnames = ["DeltaPhiLLept_mumu_afterbtag1_tauPolReweighted_TTbarSig;1","DeltaPhiLLept_emu_afterbtag1_tauPolReweighted_TTbarSig;1","DeltaPhiLLept_ee_afterbtag1_tauPolReweighted_TTbarSig;1"] 
cans =[];hists_channels = [];
legs = []
for hn in histnames:
  cans.append(makeCan(hn)) 
  hists_channels.append(MyHistFunctions_cfi.loadSameHistFromFiles(hn,files))
  legs.append(ROOT.TLegend(0.55,0.15,0.87,0.3))
#
for i,can in enumerate(cans):
  can.cd();    
  legs[i].SetShadowColor(0)                                                                                          
  for j,h in enumerate(hists_channels[i]):                                                          
    MyHistFunctions_cfi.norm1Hist1D(h);
    h.SetTitle("");h.SetLineColor(j+1);h.SetStats(0);
    h.Draw("" if j == 0 else "same"); print("j ",j," ",h.GetName())
    legs[i].AddEntry(h,"Tau Pol Rew. "+("Off" if "Off" in h.fromFile else "On" if "On" in h.fromFile else h.fromFile),"l")
  hists_channels[i][0].SetStats(False);hists_channels[i][0].SetXTitle("#Delta #phi");
  hists_channels[i][0].GetYaxis().SetRangeUser(0,hists_channels[i][0].GetMaximum()*1.2);
  legs[i].Draw()
  can.Update();can.Modified();can.SaveAs(can.GetName()+postfix+".pdf")

