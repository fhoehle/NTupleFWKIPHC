screen -d -m -S ChargeAsymmetry_TTMCatNLO_Herwig_Skimmed_screen
screen -S ChargeAsymmetry_TTMCatNLO_Herwig_Skimmed_screen -p 0 -X stuff 'source screener'`echo -ne '\015'`
screen -S ChargeAsymmetry_TTMCatNLO_Herwig_Skimmed_screen -p 0 -X stuff './compile ChargeAsymmetry_TTMCatNLO_Herwig_Skimmed.C'`echo -ne '\015'`
screen -S ChargeAsymmetry_TTMCatNLO_Herwig_Skimmed_screen -p 0 -X stuff './ChargeAsymmetry_TTMCatNLO_Herwig_Skimmed -c testMC_TTMCatNLO_Herwig.xml'`echo -ne '\015'`

sleep 60

screen -d -m -S ChargeAsymmetry_TTMCatNLO_fac-0_5_Skimmed_screen
screen -S ChargeAsymmetry_TTMCatNLO_fac-0_5_Skimmed_screen -p 0 -X stuff 'source screener'`echo -ne '\015'`
screen -S ChargeAsymmetry_TTMCatNLO_fac-0_5_Skimmed_screen -p 0 -X stuff './compile ChargeAsymmetry_TTMCatNLO_fac-0_5_Skimmed.C'`echo -ne '\015'`
screen -S ChargeAsymmetry_TTMCatNLO_fac-0_5_Skimmed_screen -p 0 -X stuff './ChargeAsymmetry_TTMCatNLO_fac-0_5_Skimmed -c testMC_TTMCatNLO_fac-0_5.xml'`echo -ne '\015'`

sleep 60

screen -d -m -S ChargeAsymmetry_TTMCatNLO_fac-2_Skimmed_screen
screen -S ChargeAsymmetry_TTMCatNLO_fac-2_Skimmed_screen -p 0 -X stuff 'source screener'`echo -ne '\015'`
screen -S ChargeAsymmetry_TTMCatNLO_fac-2_Skimmed_screen -p 0 -X stuff './compile ChargeAsymmetry_TTMCatNLO_fac-2_Skimmed.C'`echo -ne '\015'`
screen -S ChargeAsymmetry_TTMCatNLO_fac-2_Skimmed_screen -p 0 -X stuff './ChargeAsymmetry_TTMCatNLO_fac-2_Skimmed -c testMC_TTMCatNLO_fac-2.xml'`echo -ne '\015'`

sleep 60

screen -d -m -S ChargeAsymmetry_TTMCatNLO_massTop-170_Skimmed_screen
screen -S ChargeAsymmetry_TTMCatNLO_massTop-170_Skimmed_screen -p 0 -X stuff 'source screener'`echo -ne '\015'`
screen -S ChargeAsymmetry_TTMCatNLO_massTop-170_Skimmed_screen -p 0 -X stuff './compile ChargeAsymmetry_TTMCatNLO_massTop-170_Skimmed.C'`echo -ne '\015'`
screen -S ChargeAsymmetry_TTMCatNLO_massTop-170_Skimmed_screen -p 0 -X stuff './ChargeAsymmetry_TTMCatNLO_massTop-170_Skimmed -c testMC_TTMCatNLO_massTop-170.xml'`echo -ne '\015'`

sleep 60

screen -d -m -S ChargeAsymmetry_TTMCatNLO_massTop-172_5_Skimmed_screen
screen -S ChargeAsymmetry_TTMCatNLO_massTop-172_5_Skimmed_screen -p 0 -X stuff 'source screener'`echo -ne '\015'`
screen -S ChargeAsymmetry_TTMCatNLO_massTop-172_5_Skimmed_screen -p 0 -X stuff './compile ChargeAsymmetry_TTMCatNLO_massTop-172_5_Skimmed.C'`echo -ne '\015'`
screen -S ChargeAsymmetry_TTMCatNLO_massTop-172_5_Skimmed_screen -p 0 -X stuff './ChargeAsymmetry_TTMCatNLO_massTop-172_5_Skimmed -c testMC_TTMCatNLO_massTop-172_5.xml'`echo -ne '\015'`

sleep 60

screen -d -m -S ChargeAsymmetry_TTMCatNLO_massTop-175_Skimmed_screen
screen -S ChargeAsymmetry_TTMCatNLO_massTop-175_Skimmed_screen -p 0 -X stuff 'source screener'`echo -ne '\015'`
screen -S ChargeAsymmetry_TTMCatNLO_massTop-175_Skimmed_screen -p 0 -X stuff './compile ChargeAsymmetry_TTMCatNLO_massTop-175_Skimmed.C'`echo -ne '\015'`
screen -S ChargeAsymmetry_TTMCatNLO_massTop-175_Skimmed_screen -p 0 -X stuff './ChargeAsymmetry_TTMCatNLO_massTop-175_Skimmed -c testMC_TTMCatNLO_massTop-175.xml'`echo -ne '\015'`

sleep 60

screen -d -m -S ChargeAsymmetry_TTPowheg_Pythia_Skimmed_screen
screen -S ChargeAsymmetry_TTPowheg_Pythia_Skimmed_screen -p 0 -X stuff 'source screener'`echo -ne '\015'`
screen -S ChargeAsymmetry_TTPowheg_Pythia_Skimmed_screen -p 0 -X stuff './compile ChargeAsymmetry_TTPowheg_Pythia_Skimmed.C'`echo -ne '\015'`
screen -S ChargeAsymmetry_TTPowheg_Pythia_Skimmed_screen -p 0 -X stuff './ChargeAsymmetry_TTPowheg_Pythia_Skimmed -c testMC_TTPowheg_Pythia.xml'`echo -ne '\015'`

sleep 60

