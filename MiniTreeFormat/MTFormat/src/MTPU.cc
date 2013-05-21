#include "../interface/MTPU.h"

using namespace TopTree;
using namespace std;
using namespace edm;
//using namespace reco;

MTPU::MTPU ()
{
}

MTPU::~MTPU ()
{
}

void
MTPU::Reset ()
{
  NTPU::Reset ();
}

void MTPU::DumpExtended(std::ostream & os){
	Dump(os);
}

