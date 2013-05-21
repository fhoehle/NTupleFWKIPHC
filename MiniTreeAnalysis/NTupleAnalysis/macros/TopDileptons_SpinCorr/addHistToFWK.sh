#!/bin/bash
testParameter(){
 if [ -z "$1" ]; then
  echo "set variables as follows"
  echo "histName xTitle yTitle nBins xMin xMax"
  exit 1
 fi
}
histName=$1; testParameter $histName
xTitle=$2; testParameter $xTitle
yTitle=$3; testParameter $yTitle
nBins=$4; testParameter $nBins 
xMin=$5; testParameter $xMin
xMax=$6; testParameter $xMax
#${histName:?"Need to set DEST non-empty"}
echo "adding Hist histName: $histName xTitle: $xTitle yTitle: $yTitle nBins: $nBins xMin: $xMin xMax: $xMax" 
pattern='^\ *\/\/nextHistWillBeAddedHereDeclaration\ *$'
echo "adding to ProofSelectorMyCutFlow.h"
timeStamp=`date +"_%y%m%d_%H%M%S"`; echo $timeStamp
sed -i_bckAddHistToFWK_$timeStamp 's/\('"$pattern"'\)/\ \ std::vector<TH1F> '$histName';\/\/addedBy_addHistToFWK.sh'$timeStamp'\n\1/' ProofSelectorMyCutFlow.h
echo "adding to ProofSelectorMyCutFlow.C "
createHistoPattern='^\ *\/\/nextHistWillBeAddedHereCreateWithHistoManager\ *$'
writeHistoPattern='^\ *\/\/HereWillScriptAddItWrite\ *$'
clearHistoPattern='^\ *\/\/HereWillScriptAddClearing\ *$'
stampForDeleting='\/\/addedBy_addHistToFWK.sh'$timeStamp
sed -i_bckAddHistToFWK_$timeStamp -e 's/\('"$createHistoPattern"'\)/\ \ \MyhistoManager.CreateHisto('$histName',"'$histName'", datasetName, "'$xTitle'", "'$yTitle'", '$nBins','$xMin','$xMax');'$stampForDeleting'\n\1/' -e 's/\('"$writeHistoPattern"'\)/\ \ \ \ MyhistoManager.WriteMyHisto('$histName',"all");'$stampForDeleting'\n\1/' -e 's/\('"$clearHistoPattern"'\)/\ \ \ \ '$histName'.clear();'$stampForDeleting'\n\1/' ProofSelectorMyCutFlow.C 
#sed 's/\ \ \ \ \/\/HereWillScriptAddIt.*$/\ \ \ \ MyhistoManager.WriteMyHisto('$histName',"all");\/\/addedBy_addHistToFWK.sh\n\ \ \ \ \/\/NextHistosWroteWillBeAddedHere/' ProofSelectorMyCutFlow.C >& ProofSelectorMyCutFlow.C_new1 
echo "MyhistoManager.FillHisto($histName, \"$histName\", xyz, datasetName, IsSignal, Dweight[ITypeMC]);//addedBy_addHistToFWK.sh$timeStamp"
## deletion with
#sed -i_BCK_$(date "+%F_%T") '/^.*addedBy_addHistToFWK.sh_130521_100147$/d' ./ProofSelectorMyCutFlow.h 
