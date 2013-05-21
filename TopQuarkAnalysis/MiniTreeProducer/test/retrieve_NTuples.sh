#!/bin/bash

#This script has to be executed in the directory where you want to store the ntuples
#uidir is the local directory where you want to store your ntuples
uidir="/opt/sbg/data/data1/cms/echabert/NTuples/415_v1/Skim2Lept"

list="
crab_DYToMuMu_M-20_TuneZ2_7TeV-pythia6
crab_DYToTauTau_M-10To20_TuneZ2_7TeV-pythia6-tauola
crab_TToBLNu_TuneZ2_tW-channel_7TeV-madgraph
crab_WJetsToLNu_TuneD6T_7TeV-madgraph-tauola
crab_WToENu_TuneZ2_7TeV-pythia6
crab_ZZtoAnything_TuneZ2_7TeV-pythia6-tauola
"

for i in $list
do
dataset=`echo $i | awk -F 'crab_' '{print $2}'`
dirname=`tail $i/log/crab.log | grep store | tail -1 | awk '{print $11}' | awk -F 'patT' '{print $1}'`
dirname=/dpm/in2p3.fr/home/cms/phedex$dirname
uidirdataset=$uidir/$dataset
echo "#############################"
echo " Dataset " $dataset
echo " - Creating directory"
mkdir $uidir/$dataset
echo " - Moving NTuples from "$dirname to $uidir/$dataset
#rfcpNTuples $dirname $uidirdataset/.
source ~echabert/scripts/rfcpNTuples $dirname $uidirdataset/.
echo "#############################"
done

echo "          THE END        !"
echo "#############################"
