#!/bin/bash

help(){
echo use: resubmit.sh file
echo file must be the status.log file from "./status_prod.sh > status.log"
}
if [[ $# -ne 1 ]]; then
	help
	exit
fi

echo " ############################### "
echo          RESUBMITTING
echo " ############################### "
echo Read file $1


#search for lines with crab_dir and jobs to be resubmitted
grep 'Examining\|You can resubmit\|List of jobs:' $1 > resubmit.tmp
grep -n 'You can resubmit' resubmit.tmp > resubmit.lines
lines=`cat resubmit.lines | awk -F ':' '{print $1}'`
for i in $lines ; do
	eval l1=$i-1
	eval l2=$i+1
	#echo $l1 $i $l2
	crabdir=`awk NR==$l1 resubmit.tmp| awk '{print $2}'`
	#echo $crabdir
	list=`awk NR==$l2 resubmit.tmp| awk -F ':' '{print $2}'`
	echo For directory $crabdir, resubmitting jobs $list
	crab -resubmit $list -c $crabdir
done
