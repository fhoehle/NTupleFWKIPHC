#!/bin/tcsh

echo "#################################" 

setenv dir `pwd`
setenv  NTUPLEANA_PATH $dir
setenv NTUPLEANA $dir
setenv LD_LIBRARY_PATH "${LD_LIBRARY_PATH}:$HOME/lib:$dir/../../../MiniTreeFormat/NTFormat/src/.:$dir/../.lib/"
setenv NTUPLEDATAFORMAT_PATH $dir/../../../MiniTreeFormat/NTFormat/


echo NTUPLEANA_PATH=$NTUPLEANA_PATH
echo NTUPLEANA=$NTUPLEANA_PATH

if (-e ../.lib) then
	echo "List of librairies"
	ls ../.lib
else 
	mkdir ../.lib
endif

echo " Setup is DONE"
echo "#################################" 
