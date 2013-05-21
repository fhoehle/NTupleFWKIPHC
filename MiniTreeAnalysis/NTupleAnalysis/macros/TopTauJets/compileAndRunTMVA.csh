export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$HOME/lib

#cd ..
#make all
#make clean
#cd -
rm TMVA_tree
./compile TMVA_tree.C
./TMVA_tree
