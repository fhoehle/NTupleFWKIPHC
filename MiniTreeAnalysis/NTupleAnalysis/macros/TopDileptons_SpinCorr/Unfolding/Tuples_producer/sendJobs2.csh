screen -d -m -S ChargeAsymmetry_DATA2011AB_screen
screen -S ChargeAsymmetry_DATA2011AB_screen -p 0 -X stuff 'source screener'`echo -ne '\015'`
screen -S ChargeAsymmetry_DATA2011AB_screen -p 0 -X stuff './compile ChargeAsymmetry_DATA2011AB.C'`echo -ne '\015'`
screen -S ChargeAsymmetry_DATA2011AB_screen -p 0 -X stuff './ChargeAsymmetry_DATA2011AB -c test_DATA2011AB.xml'`echo -ne '\015'`

sleep 60

screen -d -m -S ChargeAsymmetry_DiBosons_PileUpReweighted_screen
screen -S ChargeAsymmetry_DiBosons_PileUpReweighted_screen -p 0 -X stuff 'source screener'`echo -ne '\015'`
screen -S ChargeAsymmetry_DiBosons_PileUpReweighted_screen -p 0 -X stuff './compile ChargeAsymmetry_DiBosons_PileUpReweighted.C'`echo -ne '\015'`
screen -S ChargeAsymmetry_DiBosons_PileUpReweighted_screen -p 0 -X stuff './ChargeAsymmetry_DiBosons_PileUpReweighted -c testMC_DiBosons.xml'`echo -ne '\015'`

sleep 60

screen -d -m -S ChargeAsymmetry_SingleTbartW_PileUpReweighted_screen
screen -S ChargeAsymmetry_SingleTbartW_PileUpReweighted_screen -p 0 -X stuff 'source screener'`echo -ne '\015'`
screen -S ChargeAsymmetry_SingleTbartW_PileUpReweighted_screen -p 0 -X stuff './compile ChargeAsymmetry_SingleTbartW_PileUpReweighted.C'`echo -ne '\015'`
screen -S ChargeAsymmetry_SingleTbartW_PileUpReweighted_screen -p 0 -X stuff './ChargeAsymmetry_SingleTbartW_PileUpReweighted -c testMC_SingleTbartW.xml'`echo -ne '\015'`

sleep 60

screen -d -m -S ChargeAsymmetry_SingleTtW_PileUpReweighted_screen
screen -S ChargeAsymmetry_SingleTtW_PileUpReweighted_screen -p 0 -X stuff 'source screener'`echo -ne '\015'`
screen -S ChargeAsymmetry_SingleTtW_PileUpReweighted_screen -p 0 -X stuff './compile ChargeAsymmetry_SingleTtW_PileUpReweighted.C'`echo -ne '\015'`
screen -S ChargeAsymmetry_SingleTtW_PileUpReweighted_screen -p 0 -X stuff './ChargeAsymmetry_SingleTtW_PileUpReweighted -c testMC_SingleTtW.xml'`echo -ne '\015'`

sleep 60

screen -d -m -S ChargeAsymmetry_TTbarAllMCatNLO_Skimmed_screen
screen -S ChargeAsymmetry_TTbarAllMCatNLO_Skimmed_screen -p 0 -X stuff 'source screener'`echo -ne '\015'`
screen -S ChargeAsymmetry_TTbarAllMCatNLO_Skimmed_screen -p 0 -X stuff './compile ChargeAsymmetry_TTbarAllMCatNLO_Skimmed.C'`echo -ne '\015'`
screen -S ChargeAsymmetry_TTbarAllMCatNLO_Skimmed_screen -p 0 -X stuff './ChargeAsymmetry_TTbarAllMCatNLO_Skimmed -c testMC_ttbarMCatNLO.xml'`echo -ne '\015'`

sleep 60

screen -d -m -S ChargeAsymmetry_TTbarAllMCatNLO_Skimmed_PileUpReweighted_screen
screen -S ChargeAsymmetry_TTbarAllMCatNLO_Skimmed_PileUpReweighted_screen -p 0 -X stuff 'source screener'`echo -ne '\015'`
screen -S ChargeAsymmetry_TTbarAllMCatNLO_Skimmed_PileUpReweighted_screen -p 0 -X stuff './compile ChargeAsymmetry_TTbarAllMCatNLO_Skimmed_PileUpReweighted.C'`echo -ne '\015'`
screen -S ChargeAsymmetry_TTbarAllMCatNLO_Skimmed_PileUpReweighted_screen -p 0 -X stuff './ChargeAsymmetry_TTbarAllMCatNLO_Skimmed_PileUpReweighted -c testMC_ttbarMCatNLO.xml'`echo -ne '\015'`

sleep 60

screen -d -m -S ChargeAsymmetry_TTbarSemileptonicMadgraph_Skimmed_PileUpReweighted_screen
screen -S ChargeAsymmetry_TTbarSemileptonicMadgraph_Skimmed_PileUpReweighted_screen -p 0 -X stuff 'source screener'`echo -ne '\015'`
screen -S ChargeAsymmetry_TTbarSemileptonicMadgraph_Skimmed_PileUpReweighted_screen -p 0 -X stuff './compile ChargeAsymmetry_TTbarSemileptonicMadgraph_Skimmed_PileUpReweighted.C'`echo -ne '\015'`
screen -S ChargeAsymmetry_TTbarSemileptonicMadgraph_Skimmed_PileUpReweighted_screen -p 0 -X stuff './ChargeAsymmetry_TTbarSemileptonicMadgraph_Skimmed_PileUpReweighted -c testMC_ttbarTuneZ2_7TeV-madgraph-tauola.xml'`echo -ne '\015'`

sleep 60

screen -d -m -S ChargeAsymmetry_TTbarSemileptonicMCatNLO_Skimmed_screen
screen -S ChargeAsymmetry_TTbarSemileptonicMCatNLO_Skimmed_screen -p 0 -X stuff 'source screener'`echo -ne '\015'`
screen -S ChargeAsymmetry_TTbarSemileptonicMCatNLO_Skimmed_screen -p 0 -X stuff './compile ChargeAsymmetry_TTbarSemileptonicMCatNLO_Skimmed.C'`echo -ne '\015'`
screen -S ChargeAsymmetry_TTbarSemileptonicMCatNLO_Skimmed_screen -p 0 -X stuff './ChargeAsymmetry_TTbarSemileptonicMCatNLO_Skimmed -c testMC_ttbarMCatNLO.xml'`echo -ne '\015'`

sleep 60

screen -d -m -S ChargeAsymmetry_TTbarSemileptonicMCatNLO_Skimmed_PileUpReweighted_screen
screen -S ChargeAsymmetry_TTbarSemileptonicMCatNLO_Skimmed_PileUpReweighted_screen -p 0 -X stuff 'source screener'`echo -ne '\015'`
screen -S ChargeAsymmetry_TTbarSemileptonicMCatNLO_Skimmed_PileUpReweighted_screen -p 0 -X stuff './compile ChargeAsymmetry_TTbarSemileptonicMCatNLO_Skimmed_PileUpReweighted.C'`echo -ne '\015'`
screen -S ChargeAsymmetry_TTbarSemileptonicMCatNLO_Skimmed_PileUpReweighted_screen -p 0 -X stuff './ChargeAsymmetry_TTbarSemileptonicMCatNLO_Skimmed_PileUpReweighted -c testMC_ttbarMCatNLO.xml'`echo -ne '\015'`

sleep 60

screen -d -m -S ChargeAsymmetry_TTbarSemileptonicPowheg_Skimmed_screen
screen -S ChargeAsymmetry_TTbarSemileptonicPowheg_Skimmed_screen -p 0 -X stuff 'source screener'`echo -ne '\015'`
screen -S ChargeAsymmetry_TTbarSemileptonicPowheg_Skimmed_screen -p 0 -X stuff './compile ChargeAsymmetry_TTbarSemileptonicPowheg_Skimmed.C'`echo -ne '\015'`
screen -S ChargeAsymmetry_TTbarSemileptonicPowheg_Skimmed_screen -p 0 -X stuff './ChargeAsymmetry_TTbarSemileptonicPowheg_Skimmed -c testMC_ttbarPowheg.xml'`echo -ne '\015'`

sleep 60

screen -d -m -S ChargeAsymmetry_TTbarSignalMadgraph_JER10_Skimmed_screen
screen -S ChargeAsymmetry_TTbarSignalMadgraph_JER10_Skimmed_screen -p 0 -X stuff 'source screener'`echo -ne '\015'`
screen -S ChargeAsymmetry_TTbarSignalMadgraph_JER10_Skimmed_screen -p 0 -X stuff './compile ChargeAsymmetry_TTbarSignalMadgraph_JER10_Skimmed.C'`echo -ne '\015'`
screen -S ChargeAsymmetry_TTbarSignalMadgraph_JER10_Skimmed_screen -p 0 -X stuff './ChargeAsymmetry_TTbarSignalMadgraph_JER10_Skimmed -c testMC_ttbarTuneZ2_7TeV-madgraph-tauola.xml'`echo -ne '\015'`

sleep 60

screen -d -m -S ChargeAsymmetry_TTbarSignalMadgraph_JER20_Skimmed_screen
screen -S ChargeAsymmetry_TTbarSignalMadgraph_JER20_Skimmed_screen -p 0 -X stuff 'source screener'`echo -ne '\015'`
screen -S ChargeAsymmetry_TTbarSignalMadgraph_JER20_Skimmed_screen -p 0 -X stuff './compile ChargeAsymmetry_TTbarSignalMadgraph_JER20_Skimmed.C'`echo -ne '\015'`
screen -S ChargeAsymmetry_TTbarSignalMadgraph_JER20_Skimmed_screen -p 0 -X stuff './ChargeAsymmetry_TTbarSignalMadgraph_JER20_Skimmed -c testMC_ttbarTuneZ2_7TeV-madgraph-tauola.xml'`echo -ne '\015'`

sleep 60

screen -d -m -S ChargeAsymmetry_TTbarSignalMadgraph_JER30_Skimmed_screen
screen -S ChargeAsymmetry_TTbarSignalMadgraph_JER30_Skimmed_screen -p 0 -X stuff 'source screener'`echo -ne '\015'`
screen -S ChargeAsymmetry_TTbarSignalMadgraph_JER30_Skimmed_screen -p 0 -X stuff './compile ChargeAsymmetry_TTbarSignalMadgraph_JER30_Skimmed.C'`echo -ne '\015'`
screen -S ChargeAsymmetry_TTbarSignalMadgraph_JER30_Skimmed_screen -p 0 -X stuff './ChargeAsymmetry_TTbarSignalMadgraph_JER30_Skimmed -c testMC_ttbarTuneZ2_7TeV-madgraph-tauola.xml'`echo -ne '\015'`

sleep 60

screen -d -m -S ChargeAsymmetry_TTbarSignalMadgraph_JESdown_Skimmed_screen
screen -S ChargeAsymmetry_TTbarSignalMadgraph_JESdown_Skimmed_screen -p 0 -X stuff 'source screener'`echo -ne '\015'`
screen -S ChargeAsymmetry_TTbarSignalMadgraph_JESdown_Skimmed_screen -p 0 -X stuff './compile ChargeAsymmetry_TTbarSignalMadgraph_JESdown_Skimmed.C'`echo -ne '\015'`
screen -S ChargeAsymmetry_TTbarSignalMadgraph_JESdown_Skimmed_screen -p 0 -X stuff './ChargeAsymmetry_TTbarSignalMadgraph_JESdown_Skimmed -c testMC_ttbarTuneZ2_7TeV-madgraph-tauola.xml'`echo -ne '\015'`

sleep 60

screen -d -m -S ChargeAsymmetry_TTbarSignalMadgraph_JESup_Skimmed_screen
screen -S ChargeAsymmetry_TTbarSignalMadgraph_JESup_Skimmed_screen -p 0 -X stuff 'source screener'`echo -ne '\015'`
screen -S ChargeAsymmetry_TTbarSignalMadgraph_JESup_Skimmed_screen -p 0 -X stuff './compile ChargeAsymmetry_TTbarSignalMadgraph_JESup_Skimmed.C'`echo -ne '\015'`
screen -S ChargeAsymmetry_TTbarSignalMadgraph_JESup_Skimmed_screen -p 0 -X stuff './ChargeAsymmetry_TTbarSignalMadgraph_JESup_Skimmed -c testMC_ttbarTuneZ2_7TeV-madgraph-tauola.xml'`echo -ne '\015'`

sleep 60

screen -d -m -S ChargeAsymmetry_TTbarSignalMadgraph_LeptonSelEffNegLeptonWithMinus_Skimmed_btag_screen
screen -S ChargeAsymmetry_TTbarSignalMadgraph_LeptonSelEffNegLeptonWithMinus_Skimmed_btag_screen -p 0 -X stuff 'source screener'`echo -ne '\015'`
screen -S ChargeAsymmetry_TTbarSignalMadgraph_LeptonSelEffNegLeptonWithMinus_Skimmed_btag_screen -p 0 -X stuff './compile ChargeAsymmetry_TTbarSignalMadgraph_LeptonSelEffNegLeptonWithMinus_Skimmed_btag.C'`echo -ne '\015'`
screen -S ChargeAsymmetry_TTbarSignalMadgraph_LeptonSelEffNegLeptonWithMinus_Skimmed_btag_screen -p 0 -X stuff './ChargeAsymmetry_TTbarSignalMadgraph_LeptonSelEffNegLeptonWithMinus_Skimmed_btag -c testMC_ttbarTuneZ2_7TeV-madgraph-tauola.xml'`echo -ne '\015'`

sleep 60

screen -d -m -S ChargeAsymmetry_TTbarSignalMadgraph_LeptonSelEffPosLeptonWithMinus_Skimmed_btag_screen
screen -S ChargeAsymmetry_TTbarSignalMadgraph_LeptonSelEffPosLeptonWithMinus_Skimmed_btag_screen -p 0 -X stuff 'source screener'`echo -ne '\015'`
screen -S ChargeAsymmetry_TTbarSignalMadgraph_LeptonSelEffPosLeptonWithMinus_Skimmed_btag_screen -p 0 -X stuff './compile ChargeAsymmetry_TTbarSignalMadgraph_LeptonSelEffPosLeptonWithMinus_Skimmed_btag.C'`echo -ne '\015'`
screen -S ChargeAsymmetry_TTbarSignalMadgraph_LeptonSelEffPosLeptonWithMinus_Skimmed_btag_screen -p 0 -X stuff './ChargeAsymmetry_TTbarSignalMadgraph_LeptonSelEffPosLeptonWithMinus_Skimmed_btag -c testMC_ttbarTuneZ2_7TeV-madgraph-tauola.xml'`echo -ne '\015'`

sleep 60

screen -d -m -S ChargeAsymmetry_TTbarSignalMadgraph_LESdown_Skimmed_screen
screen -S ChargeAsymmetry_TTbarSignalMadgraph_LESdown_Skimmed_screen -p 0 -X stuff 'source screener'`echo -ne '\015'`
screen -S ChargeAsymmetry_TTbarSignalMadgraph_LESdown_Skimmed_screen -p 0 -X stuff './compile ChargeAsymmetry_TTbarSignalMadgraph_LESdown_Skimmed.C'`echo -ne '\015'`
screen -S ChargeAsymmetry_TTbarSignalMadgraph_LESdown_Skimmed_screen -p 0 -X stuff './ChargeAsymmetry_TTbarSignalMadgraph_LESdown_Skimmed -c testMC_ttbarTuneZ2_7TeV-madgraph-tauola.xml'`echo -ne '\015'`

sleep 60

screen -d -m -S ChargeAsymmetry_TTbarSignalMadgraph_LESup_Skimmed_screen
screen -S ChargeAsymmetry_TTbarSignalMadgraph_LESup_Skimmed_screen -p 0 -X stuff 'source screener'`echo -ne '\015'`
screen -S ChargeAsymmetry_TTbarSignalMadgraph_LESup_Skimmed_screen -p 0 -X stuff './compile ChargeAsymmetry_TTbarSignalMadgraph_LESup_Skimmed.C'`echo -ne '\015'`
screen -S ChargeAsymmetry_TTbarSignalMadgraph_LESup_Skimmed_screen -p 0 -X stuff './ChargeAsymmetry_TTbarSignalMadgraph_LESup_Skimmed -c testMC_ttbarTuneZ2_7TeV-madgraph-tauola.xml'`echo -ne '\015'`

sleep 60

screen -d -m -S ChargeAsymmetry_TTbarSignalMadgraph_METdown_Skimmed_screen
screen -S ChargeAsymmetry_TTbarSignalMadgraph_METdown_Skimmed_screen -p 0 -X stuff 'source screener'`echo -ne '\015'`
screen -S ChargeAsymmetry_TTbarSignalMadgraph_METdown_Skimmed_screen -p 0 -X stuff './compile ChargeAsymmetry_TTbarSignalMadgraph_METdown_Skimmed.C'`echo -ne '\015'`
screen -S ChargeAsymmetry_TTbarSignalMadgraph_METdown_Skimmed_screen -p 0 -X stuff './ChargeAsymmetry_TTbarSignalMadgraph_METdown_Skimmed -c testMC_ttbarTuneZ2_7TeV-madgraph-tauola.xml'`echo -ne '\015'`

sleep 60

screen -d -m -S ChargeAsymmetry_TTbarSignalMadgraph_METup_Skimmed_screen
screen -S ChargeAsymmetry_TTbarSignalMadgraph_METup_Skimmed_screen -p 0 -X stuff 'source screener'`echo -ne '\015'`
screen -S ChargeAsymmetry_TTbarSignalMadgraph_METup_Skimmed_screen -p 0 -X stuff './compile ChargeAsymmetry_TTbarSignalMadgraph_METup_Skimmed.C'`echo -ne '\015'`
screen -S ChargeAsymmetry_TTbarSignalMadgraph_METup_Skimmed_screen -p 0 -X stuff './ChargeAsymmetry_TTbarSignalMadgraph_METup_Skimmed -c testMC_ttbarTuneZ2_7TeV-madgraph-tauola.xml'`echo -ne '\015'`
