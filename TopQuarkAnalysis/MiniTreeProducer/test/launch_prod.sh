#!/bin/bash

#usage:
#it's important to change the values below and the list of datasets you want to run over


#change your email below
#export email="Eric.Chabert@cern.ch"
#this will be used for the publish-name and for the directory
export prodname="Prod_414_sync_v1"
#crab config file
export configfile="crab_MiniTreeProducer.cfg"
#you have to manualy change the eMail in the config file
#pset: config file
export pset="MiniTree_PATwithPF2PAT_forMC_AOD_41X_cfg.py"

echo Number of arguments $#

function create_config_file(){
	#usage:
	# create_config_file config_ref.cfg 
	DataSetline=`sed -n /datasetpath/p $configfile`
	OldDataSet=`echo $DataSetline | awk -F\= '{print $2}'`
	OldDataSet=`echo $OldDataSet | sed s/\ //g`

	PublishNameline=`sed -n /publish_data_name/p $configfile`
	OldPublishName=`echo $PublishNameline | awk -F\= '{print $2}'`
	OldPublishName=`echo $OldPublishName | sed s/\ //g`

	DirNameline=`sed -n /user_remote_dir/p $configfile`
	OldDirName=`echo $DirNameline | awk -F\= '{print $2}'`
	OldDirName=`echo $OldDirName | sed s/\ //g`

	UIDirNameline=`sed -n /ui_working_dir/p $configfile`
	OldUIDirName=`echo $UIDirNameline | awk -F\= '{print $2}'`
	OldUIDirName=`echo $OldUIDirName | sed s/\ //g`

	PSETline=`sed -n /pset/p $configfile`
	OldPSET=`echo $PSETline | awk -F\= '{print $2}'`
	OldPSET=`echo $OldPSET | sed s/\ //g`

	sed s@$OldDataSet@$datasetname@g $configfile > config.tmp.v1
	sed s@$OldPublishName@$pub_name@g config.tmp.v1 > config.tmp.v2
	sed s@$OldDirName@$se_dir@g config.tmp.v2 > config.tmp.v3
	sed s@$OldUIDirName@$ui_dir@g config.tmp.v3 > config.tmp.v4
	sed s@$OldPSET@$pset@g config.tmp.v4 > config.tmp.v5
	#the config file is now ready
	mv config.tmp.v5 crab_$name.cfg
        
	#remove useless files
	rm config.tmp.v1 config.tmp.v2 config.tmp.v3 config.tmp.v4

}

function launch(){
  echo Launching dataset $datasetname
  echo "+++++++++++++++++++"
  echo " Creating config file ... "
  create_config_file
  echo "+++++++++++++++++++"
  echo " Creating crab jobs ... "
  crab -create all -cfg crab_$name.cfg
  echo "+++++++++++++++++++"
  echo " Submitting crab jobs ... "
  crab -submit all -c $ui_dir 
  echo "+++++++++++++++++++"
  echo "   DONE !!! "
  echo "+++++++++++++++++++"

}

#list of samples
list="
/TTJets_TuneD6T_7TeV-madgraph-tauola/Spring11-PU_S1_START311_V1G1-v1/AODSIM
/TToBLNu_TuneZ2_tW-channel_7TeV-madgraph/Spring11-PU_S1_START311_V1G1-v1/AODSIM
/DYToEE_M-10To20_TuneZ2_7TeV-pythia6/Spring11-PU_S1_START311_V1G1-v1/AODSIM
/DYToEE_M-20_TuneZ2_7TeV-pythia6/Spring11-PU_S1_START311_V1G1-v1/AODSIM
/DYToMuMu_M-10To20_TuneZ2_7TeV-pythia6/Spring11-PU_S1_START311_V1G1-v1/AODSIM
/DYToMuMu_M-20_TuneZ2_7TeV-pythia6/Spring11-PU_S1_START311_V1G1-v1/AODSIM
/DYToTauTau_M-10To20_TuneZ2_7TeV-pythia6-tauola/Spring11-PU_S1_START311_V1G1-v1/AODSIM
/DYToTauTau_M-20_TuneZ2_7TeV-pythia6-tauola/Spring11-PU_S1_START311_V1G1-v1/AODSIM
/DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola/Spring11-PU_S1_START311_V1G1-v1/AODSIM
/WWTo2L2Nu_TuneZ2_7TeV-pythia6/Spring11-PU_S1_START311_V1G1-v1/AODSIM
/WZTo3LNu_TuneZ2_7TeV-pythia6/Spring11-PU_S1_START311_V1G1-v1/AODSIM
/ZZtoAnything_TuneZ2_7TeV-pythia6-tauola/Spring11-PU_S1_START311_V1G1-v1/AODSIM
/WToENu_TuneZ2_7TeV-pythia6/Spring11-PU_S1_START311_V1G1-v1/AODSIM
/WToMuNu_TuneZ2_7TeV-pythia6/Spring11-PU_S1_START311_V1G1-v1/AODSIM
/WToTauNu_TuneZ2_7TeV-pythia6-tauola/Spring11-PU_S1_START311_V1G1-v1/AODSIM
/DYJetsToLL_TuneD6T_M-50_7TeV-madgraph-tauola/Spring11-PU_S1_START311_V1G1-v1/AODSIM
/WJetsToLNu_TuneD6T_7TeV-madgraph-tauola/Spring11-PU_S1_START311_V1G1-v1/AODSIM
/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/Spring11-PU_S1_START311_V1G1-v1/AODSIM
/QCD_Pt_0to5_TuneZ2_7TeV_pythia6/Spring11-PU_S1_START311_V1G1-v1/AODSIM
/QCD_Pt_5to15_TuneZ2_7TeV_pythia6/Spring11-PU_S1_START311_V1G1-v1/AODSIM
/QCD_Pt_15to30_TuneZ2_7TeV_pythia6/Spring11-PU_S1_START311_V1G1-v1/AODSIM
/QCD_Pt_30to50_TuneZ2_7TeV_pythia6/Spring11-PU_S1_START311_V1G1-v1/AODSIM
/QCD_Pt_50to80_TuneZ2_7TeV_pythia6/Spring11-PU_S1_START311_V1G1-v1/AODSIM
/QCD_Pt_80to120_TuneZ2_7TeV_pythia6/Spring11-PU_S1_START311_V1G1-v1/AODSIM
/QCD_Pt_120to170_TuneZ2_7TeV_pythia6/Spring11-PU_S1_START311_V1G1-v1/AODSIM
/QCD_Pt_170to300_TuneZ2_7TeV_pythia6/Spring11-PU_S1_START311_V1G1-v1/AODSIM
/QCD_Pt_300to470_TuneZ2_7TeV_pythia6/Spring11-PU_S1_START311_V1G1-v1/AODSIM
/QCD_Pt_470to600_TuneZ2_7TeV_pythia6/Spring11-PU_S1_START311_V1G1-v1/AODSIM
/QCD_Pt_600to800_TuneZ2_7TeV_pythia6/Spring11-PU_S1_START311_V1G1-v1/AODSIM
/QCD_Pt_800to1000_TuneZ2_7TeV_pythia6/Spring11-PU_S1_START311_V1G1-v1/AODSIM
/QCD_Pt_1000to1400_TuneZ2_7TeV_pythia6/Spring11-PU_S1_START311_V1G1-v1/AODSIM
/QCD_Pt_1400to1800_TuneZ2_7TeV_pythia6/Spring11-PU_S1_START311_V1G1-v1/AODSIM
/QCD_Pt_1800_TuneZ2_7TeV_pythia6/Spring11-PU_S1_START311_V1G1-v1/AODSIM
"

listData="
"

for i in $list ; do
	echo "###################################################"
        export datasetname=$i
	echo datasetname: $i ;
	export name=`echo $i | awk -F '/' '{print $2}'`
	echo name: $name
	export ui_dir="crab_"$name
	export se_dir=$prodname"/"$name
	export pub_name=$name"_"$prodname
	echo ui_dir: $ui_dir
	echo se_dir: $se_dir
	echo publish_name: $pub_name
	launch
done

