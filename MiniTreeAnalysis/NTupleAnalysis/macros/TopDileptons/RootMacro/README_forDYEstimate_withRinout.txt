Pour estimer la contribution DY et les facteurs correctifs:
1/ Produire un lot avec tous les samples MC et les SF du DY mis a 1.00 (exemple fichier CrossSection_noWeights2.root)
2/ A partir du lot precedent, definir les facteurs correctifs modifiant l'hypothese Rinout_MC = Rinout_Data, a savoir les
facteurs correctifs lies a:
- effet PU
- dependance Njet
- dependance MET

L'effet du PU est obtenu en tournant la macro Rout_in.C, a partir du lot precedent et en mettant les lignes 185 et 186:
    if( channel == "mumu") Routin_MC[biny-1] *= 1.00;
    if( channel == "ee"  ) Routin_MC[biny-1] *= 1.00;
(la configuration initiale est     
    if( channel == "mumu") Routin_MC[biny-1] *= 1.00;
    if( channel == "ee"  ) Routin_MC[biny-1] *= 1.00;
    if( channel == "mumu" && plot == "METSel")  Routin_MC[biny-1] *= 1.00;
    if( channel == "ee"   && plot == "METSel")  Routin_MC[biny-1] *= 1.00;
    if( channel == "mumu" && plot == "MET")  Routin_MC[biny-1] *= 1.00;
    if( channel == "ee"   && plot == "MET")  Routin_MC[biny-1] *= 1.00;
)    
    
 on fait tourner 
 Rout_in("ee","PU") et ensuite Rout_in("mumu","PU") 
 on obtient respectivement Mean for PU 1.08902 et Mean for PU 1.25321 (dependance constante: cela permet de se "recalibrer" a 1
  et verifier  Rinout_MC = Rinout_Data
  
 on modifie les lignes prededentes en 
    if( channel == "mumu") Routin_MC[biny-1] *= 1.25;
    if( channel == "ee"  ) Routin_MC[biny-1] *= 1.09;
  
 on recompile et on refait tourner  
  Rout_in("ee","NJet") et ensuite Rout_in("mumu","NJet") 
  on voit clairement la dependance en NJet
  on obtient Mean for Njets>=2 1.3599 et Mean for Njets>=2 1.41292 respectivement (calcule avec une somme ponderee sur njet=2,3
  et 4)
  
  on modifie les lignes  189 et 190 en 
    if( channel == "mumu" && plot == "MET")  Routin_MC[biny-1] *= 1.000;
    if( channel == "ee"   && plot == "MET")  Routin_MC[biny-1] *= 1.000;

on recompile et on lance      
Rout_in("ee","MET")    et  Rout_in("mumu","MET")
on obtient respectivement:
****************************************
Minimizer is Linear
Chi2                      =      4.44223
NDf                       =            6
p0                        =     0.957281   +/-   0.00864734

****************************************
Minimizer is Linear
Chi2                      =      2.93615
NDf                       =            6
p0                        =     0.954026   +/-   0.00725346

et on reinjecte les valeurs  0.957281 et 0.954026  dasn les lignes 189 et 189:
    if( channel == "mumu" && plot == "MET")  Routin_MC[biny-1] *= 0.957;
    if( channel == "ee"   && plot == "MET")  Routin_MC[biny-1] *= 0.954;
    
 on recompile et on relance:
Rout_in("ee","MET")    et  Rout_in("mumu","MET")    
et on obtient respectivement Mean for MET>30 1.07211 et 0.845701

A la fin de cette procedure on doit avoir:
    if( channel == "mumu") Routin_MC[biny-1] *= 1.25;
    if( channel == "ee"  ) Routin_MC[biny-1] *= 1.09;
    if( channel == "mumu" && plot == "METSel")  Routin_MC[biny-1] *= 1.00;
    if( channel == "ee"   && plot == "METSel")  Routin_MC[biny-1] *= 1.00;
    if( channel == "mumu" && plot == "MET")  Routin_MC[biny-1] *= 0.957;
    if( channel == "ee"   && plot == "MET")  Routin_MC[biny-1] *= 0.954;
    
    
 Ensuite on prend le fichier ZMasEstimateInOut.C
  et on met les lignes 277 et 278:
  if(channel == "EE") Routin = Routin*1.09*1.36*1.07;
  if(channel == "MM") Routin = Routin*1.25*1.41*0.845;
  
  ensuite on fait un 
dumpTable()


Avec le fichier 
proof_marcosample_all_SFDYequal1.root, on a :
  if(channel == "EE") Routin = Routin*1.09*1.38*0.99;
  if(channel == "MM") Routin = Routin*1.25*1.43*0.915;

  
     
Pour la methode DY avec le canal emu, il faut utiliser 
ZEMUmassFromLikelihoodFit.C qui a besoin de RooFit, pour cela il faut faire au prealable :
source setup_ForDYemu.sh

  
 
 

