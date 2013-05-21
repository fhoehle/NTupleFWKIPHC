#!/bin/bash

list="
crab_toto
"

for i in $list ; do
	echo "###################################################"
        echo  Getting output of $i
 	crab -getoutput all -c $i
	echo "###################################################"
        echo  Publishing $i
 	crab -publish all -c $i
	echo "###################################################"
done

