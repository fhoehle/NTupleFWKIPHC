#include "TH1F.h"
#include "TH2F.h"
#include "TFile.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TLegend.h"
#include "TF1.h"


void PLRBtagTemplateProvider();

TGraphErrors* Tg(TH2F *h2d, int nj, int color, TString poly);
