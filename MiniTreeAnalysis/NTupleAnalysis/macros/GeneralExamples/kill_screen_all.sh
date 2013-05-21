#!/bin/bash


for i in ` screen -ls ` ; do

	
	echo "############################################"
	echo "#	Kill screen session " $i
	echo "############################################"
	
	
	screen -S $i -p 0 -X stuff $'\003'
	screen -S $i -p 0 -X stuff 'exit'`echo -ne '\015'`

 
	echo "############################################"
	echo "#	Done		 			 #"
	echo "############################################"
done
