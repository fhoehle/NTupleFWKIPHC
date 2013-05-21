#!/bin/bash

xmlfile="EfficiencyPlot.xml"
Macro="EfficiencyPlotAllTaggers"
RootFileBase="btageff"

#SampleList="DataMu DataEG DataMuEG TTbar DYToMuMu_M-10To20 DYToTauTau_M-10To20 DYToEE_M-10To20 DYToMuMu_M-20 DYToTauTau_M-20 DYToEE_M-20 Wjets Zjets TtW TbartW WW WZ ZZ"

SampleList="TTbar_mass_down TTbar_mass_up TTbar_scale_down TTbar_scale_up  TTbar_match_down TTbar_match_up"

usage(){
	echo " ################################### "
	echo "  Usage:"
	echo " ./launch_plots4PRL.sh OPTION"
	echo " Available OPTION:"
	echo " launch, status, close"
}


close(){
	for i in $SampleList 
	do
		SessionName=screen_Plots_$i
		screen -S $SessionName -p 0 -X stuff $'\003'
		screen -S $SessionName -p 0 -X stuff 'exit'`echo -ne '\015'`
	done
}

status(){
	for i in $SampleList 
	do
		out=`ps -ef | grep echabert | grep $Macro | grep $i`
		echo Sample $i
		echo $out
	done
}

launch(){

echo "##############################################"
echo " Launching production of plots for PLR usage "
echo "##############################################"

for i in $SampleList;
do
	echo Sample $i
	SessionName=screen_Plots_$i
	echo "Creating xml file"
	BaseName=`echo $xmlfile|awk -F '.' '{print $1}'`
	NewFileName=${BaseName}_$i.xml
	RootFileName=$RootFileBase_$i.root
	line=`grep -n $i ../../config/$xmlfile | awk -F ':' '{print $1}'`
	line=`echo $line | awk '{print $1}'`
	echo "THE line is " $line"|"
	sed ''${line}'s/add="0"/add="1"/' ../../config/$xmlfile > ../../config/$NewFileName	

	echo file $NewFileName is ready

	echo "   ----------------- "
	echo " Creating screen session"
	screen -d -m -S $SessionName
	echo " Initialization"
	screen -S $SessionName -p 0 -X stuff 'source ~/.bash_profile'`echo -ne '\015'`
	screen -S $SessionName -p 0 -X stuff 'cmsenv'`echo -ne '\015'`
	screen -S $SessionName -p 0 -X stuff '. compile '$Macro'.C'`echo -ne '\015'`
	screen -S $SessionName -p 0 -X stuff './'$Macro' ../../config/'$NewFileName' '$RootFileName' '`echo -ne '\015'`
  	sleep 20	
done


echo "##############################################"
echo "           THE      END                       "
echo "##############################################"

}

#### MAIN

if [ $# -eq 0 ] ; then 
	echo "You need arguments ..."
	usage 
	exit 
fi

if [ $1 == "launch" ]; then
	launch
fi

if [ $1 == "close" ]; then
	close
fi

if [ $1 == "status" ]; then
	status
fi
