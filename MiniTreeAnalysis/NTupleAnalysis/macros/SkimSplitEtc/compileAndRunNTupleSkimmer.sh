
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:.

#cp ../src/libToto.so .

#g++  -L ~/lib -l NTupleAna -I `root-config --incdir` `root-config --libs` NTupleSkimmer.C -o NTupleSkimmer
g++  -L ~/lib -l NTupleAna -l NTuple -I `root-config --incdir` -L $ROOTSYS/lib -lGenVector -lCore -lCint -lRIO -lNet -lHist -lGraf -lGraf3d -lGpad -lTree -lRint -lPostscript -lMatrix -lPhysics -lMathCore -lThread -pthread -lm -ldl -rdynamic NTupleSkimmer.C -o NTupleSkimmer

if [ $? -eq 0 ]
then
	echo "Compiling OK, now run the code!"
	./NTupleSkimmer
else
	echo "Problem while compiling!"
fi
