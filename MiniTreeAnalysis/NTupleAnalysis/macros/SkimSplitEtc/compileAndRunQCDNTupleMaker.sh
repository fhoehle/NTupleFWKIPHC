
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$HOME/lib

#cp ../src/libToto.so .

#g++  -L ~/lib -l NTupleAna -I `root-config --incdir` `root-config --libs` QCDNTupleMaker.C -o QCDNTupleMaker
g++  -L ~/lib -l NTupleAna -l NTuple -I `root-config --incdir` -L $ROOTSYS/lib -lGenVector -lCore -lCint -lRIO -lNet -lHist -lGraf -lGraf3d -lGpad -lTree -lRint -lPostscript -lMatrix -lPhysics -lMathCore -lThread -pthread -lm -ldl -rdynamic QCDNTupleMaker.C -o QCDNTupleMaker

if [ $? -eq 0 ]
then
	echo "Compiling OK, now run the code!"
	./QCDNTupleMaker
else
	echo "Problem while compiling!"
fi
