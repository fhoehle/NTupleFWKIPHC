export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$HOME/lib

cd ..
make all
make clean
cd -
rm NTupleSplitter
./compile NTupleSplitter.C
./NTupleSplitter
