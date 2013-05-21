#!/bin/bash
#
# Launch the full minitree production
# A.-C. 20/11/2009
#
#


eval `scramv1 runtime -sh`
source /opt/exp_soft/cms/CRAB/CRAB_2_7_7_patch1/crab.sh

./submit_NTupleProducer.sh TTbar /TTJets_TuneD6T_7TeV-madgraph-tauola/alebihan-CMSSW_4_1_4_minitrees_MC_v1-93c2634cffb5e4c3042f8d0c302b4c21/USER MC_ntuples_4_1_4_v1/TTJets_TuneD6T_7TeV-madgraph-tauola

#./submit_NTupleProducer.sh SingleMu /SingleMu/alebihan-CMSSW_4_1_3_p3_SingleMu_PF2PAT_v5_DCSonly-791c60e2ef20b17c14565d52b727524c/USER SingleMu_ntuples_4_1_3_p3_v5_DCSonly
#./submit_NTupleProducer.sh SingleMu /SingleMu/alebihan-CMSSW_4_1_3_p3_SingleMu_PF2PAT_v5-791c60e2ef20b17c14565d52b727524c/USER SingleMu_ntuples_4_1_3_p3_v5
#./submit_NTupleProducer.sh MultiJet /MultiJet/alebihan-CMSSW_4_1_3_p3_MultiJet_PF2PAT_v5bis-791c60e2ef20b17c14565d52b727524c/USER MultiJet_ntuples_4_1_3_p3_v5bis
