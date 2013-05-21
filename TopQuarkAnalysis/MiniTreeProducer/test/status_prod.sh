#!/bin/bash

for i in crab_* ; do
	echo "###################################################"
        echo   Examining $i
 	crab -status -c $i
	echo "###################################################"
done

