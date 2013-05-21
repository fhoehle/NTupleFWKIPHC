#!/bin/bash
#
# Launch ttanalyzerjump ntuple production
# A.-C. 13/11/2009
#
# In CMSSW_X_X_X/src do...
#./submit_ttanalyzerjump.sh /opt/sbg/cms/ui3_data3/lebihan/PAT/MCData/CMSSW_314/test  TTbar
#
#

echo "============================================================================================================"
#echo " Process " $1
echo " Launching crab with dataset " $2
echo " Ouput will be located in ..." $3
echo "  "
echo "============================================================================================================"

#ProcessNameline=`sed -n /ProcessName/p NTupleProducer_cfg.py`
#OldProcessName=`echo $ProcessNameline | awk -F\" '{print $2}'`
#sed s/$OldProcessName/$1/g NTupleProducer_cfg.py > temp
#mv temp NTupleProducer_cfg.py


#------------------------------------------
# Updating crab_NTupleProducer_cfg.py  

DataSetline=`sed -n /datasetpath/p crab_NTupleProducer.cfg`
OldDataSet=`echo $DataSetline | awk -F\= '{print $2}'`
OldDataSet=`echo $OldDataSet | sed s/\ //g`

sed s@$OldDataSet@$2@g  crab_NTupleProducer.cfg > temp2
mv temp2 crab_NTupleProducer.cfg

RemoteDirline=`sed -n /user_remote_dir/p crab_NTupleProducer.cfg`
OldRemoteDir=`echo $RemoteDirline | awk -F\= '{print $2}'`
OldRemoteDir=`echo $OldRemoteDir | sed s/\ //g`
OldRemoteDir=`echo $OldRemoteDir | awk -F\check '{print $1}'`

echo $OldRemoteDirline
echo $OldRemoteDir

sed s@$OldRemoteDir@$3@g  crab_NTupleProducer.cfg > temp3
mv temp3 crab_NTupleProducer.cfg

#OutputFileline=`sed -n /output_file/p crab_NTupleProducer.cfg`
#OldOutputFile=`echo $OutputFileline | awk -F\= '{print $2}'`
#OldOutputFile=`echo $OldOutputFile | sed s/\ //g`

#sed s@$OldOutputFile@treeFile_$1.root@g  crab_NTupleProducer.cfg > temp4
#mv temp4 crab_NTupleProducer.cfg
		

#------------------------------------------
# Submitting with crab  

crab -create -cfg crab_NTupleProducer.cfg
crab -submit

echo "  "
echo "============================================================================================================"
