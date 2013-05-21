#!/bin/bash

taggers="
NjetsNBjets_0_1.70
NjetsNBjets_0_3.30
NjetsNBjets_0_10.2
NjetsNBjets_1_1.74
NjetsNBjets_1_3.05
NjetsNBjets_2_1.19
NjetsNBjets_2_1.93
NjetsNBjets_2_3.41
NjetsNBjets_3_2.00
NjetsNBjets_4_0.275
NjetsNBjets_4_0.545
NjetsNBjets_4_0.790
NjetsNBjets_5_1.33
NjetsNBjets_5_2.55
NjetsNBjets_5_3.74
NjetsNBjets_6_0.244
NjetsNBjets_6_0.679
NjetsNBjets_6_0.898
"


xmlfile=PLRMeas_btag_2fb.xml

for i in $taggers ; do

	
	echo "############################################"
	echo "#	Tagger" $i
	echo "############################################"
	
	echo "############################################"
	echo "#	Create a new xml file			 #"
	echo "############################################"
	NewFileName=${xmlfile}_$i
	RootFileName=plr_${i}.root
	LogFileName=plr_${i}.log
	SessionName=screen_$i

	screen -d -m -S $SessionName
	echo " Initialization"
	screen -S $SessionName -p 0 -X stuff 'source ~/.bash_profile'`echo -ne '\015'`
	screen -S $SessionName -p 0 -X stuff 'cd /opt/sbg/data/data1/cms/echabert/TopDilep/PLR/CMSSW_4_2_3/src/'`echo -ne '\015'`
	screen -S $SessionName -p 0 -X stuff 'cmsenv'`echo -ne '\015'`
	screen -S $SessionName -p 0 -X stuff 'cd -'`echo -ne '\015'`
	screen -S $SessionName -p 0 -X stuff '. setup.sh'`echo -ne '\015'`

	
	line=`grep -n NjetsNBjets ../config/$xmlfile | awk -F ':' '{print $1}'`
        line=`echo $line | awk '{print $1}'`
        echo "THE line is " $line"|"
        sed ''${line}'s/NjetsNBjets/'$i'/' ../config/$xmlfile > ../config/$NewFileName

	echo "############################################"
	echo "#	Running the code 			 #"
	echo "############################################"
	//./RunPLRMeas ../config/$NewFileName $RootFileName > $LogFileName 2>&1 
	screen -S $SessionName -p 0 -X stuff './RunPLRMeas ../config/'$NewFileName' '$RootFileName' > '$LogFileName' 2>&1'`echo -ne '\015'`
 
	echo "############################################"
	echo "#	Done		 			 #"
	echo "############################################"
done
