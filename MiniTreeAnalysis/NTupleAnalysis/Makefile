ObjSuf        = o
SrcSuf        = cc
ExeSuf        = 
DllSuf        = so
OutPutOpt     = -o
HeadSuf       = h

ROOTCFLAGS    = $(shell root-config --cflags)
#ROOTLIBS      = $(shell root-config --libs) -lMinuit -lMathMore 
#ROOTGLIBS     = $(shell root-config --glibs) -lMinuit -lMathMore
ROOTLIBS      = -lGenVector -lCore -lCint -lRIO -lNet -lHist -lGraf -lGraf3d -lGpad -lTree -lRint -lPostscript -lMatrix -lMinuit -lPhysics -lMathCore -lThread -pthread -lm -ldl -rdynamic -lTMVA
ROOTGLIBS     = -lGenVector -lCore -lCint -lRIO -lNet -lHist -lGraf -lGraf3d -lGpad -lTree -lRint -lPostscript -lMatrix -lMinuit -lPhysics -lMathCore -lThread -pthread -lm -ldl -rdynamic -lTMVA

# Linux with egcs
DEFINES       = -DNO_ORCA_CLASSES
CXX           = g++
CXXFLAGS	= -O -Wall -fPIC $(DEFINES)  -I /opt/sbg/data/data1/cms/jandrea/LHAPDF/lhapdf-5.8.5/final/include



LD		= g++
LDFLAGS		= -g -O -Wall -fPIC
SOFLAGS		= -shared

CXXFLAGS	+= $(ROOTCFLAGS)
LIBS		= $(ROOTLIBS)  -lEG 
GLIBS		= $(ROOTGLIBS)
#------------------------------------------------------------------------------
SOURCES		= $(wildcard Selection/src/*.cc BckgdEstimation/src/*.cc Plots/src/*.cc Measurements/src/*.cc Tools/src/*.cc tinyxml/*.cc EffEstimation/src/*cc MassAMWT/src/*cc  KinFitter/src/*cc)
HEADERS		= $(wildcard Selection/interface/*.h BckgdEstimation/interface/*.h Plots/interface/*.h Measurements/interface/*.h Tools/interface/*.h tinyxml/*.h EffEstimation/interface/*h MassAMWT/interface/*.h KinFitter/interface/*.h)
OBJECTS		= $(SOURCES:.$(SrcSuf)=.$(ObjSuf))
DEPENDS		= $(SOURCES:.$(SrcSuf)=.d)
SOBJECTS	= $(SOURCES:.$(SrcSuf)=.$(DllSuf))


all:  libNTupleAna.so ; cp  libNTupleAna.so .lib/libNTupleAna_`date +"%d-%m-%y_%H-%M-%S"`.so ; 

testDir:
	if [ -d ~/libs ]  then echo "" else mkdir ~/libs  fi

clean:
	@echo "Cleaning..."
	@rm -f $(OBJECTS) $(DEPENDS) *Dict.* core 

.SUFFIXES: .$(SrcSuf) .C .o .so


libNTupleAna.so: $(OBJECTS) 
	@echo "Building libNTupleAna..."
	$(LD) -L${ROOTSYS}/lib $(LIBS) $(SOFLAGS) $(LDFLAGS) $+ -o $@

