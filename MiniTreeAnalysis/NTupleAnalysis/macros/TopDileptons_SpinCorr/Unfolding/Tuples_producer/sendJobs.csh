screen -d -m -S ChargeAsymmetry_TTbarSignalMCatNLO_Skimmed_screen
screen -S ChargeAsymmetry_TTbarSignalMCatNLO_Skimmed_screen -p 0 -X stuff 'source screener'`echo -ne '\015'`
screen -S ChargeAsymmetry_TTbarSignalMCatNLO_Skimmed_screen -p 0 -X stuff './compile ChargeAsymmetry_TTbarSignalMCatNLO_Skimmed.C'`echo -ne '\015'`
screen -S ChargeAsymmetry_TTbarSignalMCatNLO_Skimmed_screen -p 0 -X stuff './ChargeAsymmetry_TTbarSignalMCatNLO_Skimmed -c testMC_ttbarMCatNLO.xml'`echo -ne '\015'`

sleep 60

screen -d -m -S ChargeAsymmetry_TTbarSignalMCatNLO_Skimmed_PileUpReweighted_screen
screen -S ChargeAsymmetry_TTbarSignalMCatNLO_Skimmed_PileUpReweighted_screen -p 0 -X stuff 'source screener'`echo -ne '\015'`
screen -S ChargeAsymmetry_TTbarSignalMCatNLO_Skimmed_PileUpReweighted_screen -p 0 -X stuff './compile ChargeAsymmetry_TTbarSignalMCatNLO_Skimmed_PileUpReweighted.C'`echo -ne '\015'`
screen -S ChargeAsymmetry_TTbarSignalMCatNLO_Skimmed_PileUpReweighted_screen -p 0 -X stuff './ChargeAsymmetry_TTbarSignalMCatNLO_Skimmed_PileUpReweighted -c testMC_ttbarMCatNLO.xml'`echo -ne '\015'`

sleep 60

screen -d -m -S ChargeAsymmetry_TTbarSignalPowheg_Skimmed_screen
screen -S ChargeAsymmetry_TTbarSignalPowheg_Skimmed_screen -p 0 -X stuff 'source screener'`echo -ne '\015'`
screen -S ChargeAsymmetry_TTbarSignalPowheg_Skimmed_screen -p 0 -X stuff './compile ChargeAsymmetry_TTbarSignalPowheg_Skimmed.C'`echo -ne '\015'`
screen -S ChargeAsymmetry_TTbarSignalPowheg_Skimmed_screen -p 0 -X stuff './ChargeAsymmetry_TTbarSignalPowheg_Skimmed -c testMC_ttbarPowheg.xml'`echo -ne '\015'`

sleep 60

screen -d -m -S ChargeAsymmetry_TTbarSignalTuneZ2_7TeV-madgraph-tauola_Skimmed_screen
screen -S ChargeAsymmetry_TTbarSignalTuneZ2_7TeV-madgraph-tauola_Skimmed_screen -p 0 -X stuff 'source screener'`echo -ne '\015'`
screen -S ChargeAsymmetry_TTbarSignalTuneZ2_7TeV-madgraph-tauola_Skimmed_screen -p 0 -X stuff './compile ChargeAsymmetry_TTbarSignalTuneZ2_7TeV-madgraph-tauola_Skimmed.C'`echo -ne '\015'`
screen -S ChargeAsymmetry_TTbarSignalTuneZ2_7TeV-madgraph-tauola_Skimmed_screen -p 0 -X stuff './ChargeAsymmetry_TTbarSignalTuneZ2_7TeV-madgraph-tauola_Skimmed -c testMC_ttbarTuneZ2_7TeV-madgraph-tauola.xml'`echo -ne '\015'`

sleep 60

screen -d -m -S ChargeAsymmetry_TTbarSignalTuneZ2_7TeV-madgraph-tauola_Skimmed_PileUpReweighted_screen
screen -S ChargeAsymmetry_TTbarSignalTuneZ2_7TeV-madgraph-tauola_Skimmed_PileUpReweighted_screen -p 0 -X stuff 'source screener'`echo -ne '\015'`
screen -S ChargeAsymmetry_TTbarSignalTuneZ2_7TeV-madgraph-tauola_Skimmed_PileUpReweighted_screen -p 0 -X stuff './compile ChargeAsymmetry_TTbarSignalTuneZ2_7TeV-madgraph-tauola_Skimmed_PileUpReweighted.C'`echo -ne '\015'`
screen -S ChargeAsymmetry_TTbarSignalTuneZ2_7TeV-madgraph-tauola_Skimmed_PileUpReweighted_screen -p 0 -X stuff './ChargeAsymmetry_TTbarSignalTuneZ2_7TeV-madgraph-tauola_Skimmed_PileUpReweighted -c testMC_ttbarTuneZ2_7TeV-madgraph-tauola.xml '`echo -ne '\015'`

sleep 60

screen -d -m -S ChargeAsymmetry_TTbarSignalTuneZ2_7TeV-madgraph-tauola_Skimmed_PileUpReweighted_Down_screen
screen -S ChargeAsymmetry_TTbarSignalTuneZ2_7TeV-madgraph-tauola_Skimmed_PileUpReweighted_Down_screen -p 0 -X stuff 'source screener'`echo -ne '\015'`
screen -S ChargeAsymmetry_TTbarSignalTuneZ2_7TeV-madgraph-tauola_Skimmed_PileUpReweighted_Down_screen -p 0 -X stuff './compile ChargeAsymmetry_TTbarSignalTuneZ2_7TeV-madgraph-tauola_Skimmed_PileUpReweighted_Down.C'`echo -ne '\015'`
screen -S ChargeAsymmetry_TTbarSignalTuneZ2_7TeV-madgraph-tauola_Skimmed_PileUpReweighted_Down_screen -p 0 -X stuff './ChargeAsymmetry_TTbarSignalTuneZ2_7TeV-madgraph-tauola_Skimmed_PileUpReweighted_Down -c testMC_ttbarTuneZ2_7TeV-madgraph-tauola.xml'`echo -ne '\015'`

sleep 60

screen -d -m -S ChargeAsymmetry_TTbarSignalTuneZ2_7TeV-madgraph-tauola_Skimmed_PileUpReweighted_Up_screen
screen -S ChargeAsymmetry_TTbarSignalTuneZ2_7TeV-madgraph-tauola_Skimmed_PileUpReweighted_Up_screen -p 0 -X stuff 'source screener'`echo -ne '\015'`
screen -S ChargeAsymmetry_TTbarSignalTuneZ2_7TeV-madgraph-tauola_Skimmed_PileUpReweighted_Up_screen -p 0 -X stuff './compile ChargeAsymmetry_TTbarSignalTuneZ2_7TeV-madgraph-tauola_Skimmed_PileUpReweighted_Up.C'`echo -ne '\015'`
screen -S ChargeAsymmetry_TTbarSignalTuneZ2_7TeV-madgraph-tauola_Skimmed_PileUpReweighted_Up_screen -p 0 -X stuff './ChargeAsymmetry_TTbarSignalTuneZ2_7TeV-madgraph-tauola_Skimmed_PileUpReweighted_Up -c testMC_ttbarTuneZ2_7TeV-madgraph-tauola.xml'`echo -ne '\015'`

sleep 60

screen -d -m -S ChargeAsymmetry_TTJetsmass161_Skimmed_screen
screen -S ChargeAsymmetry_TTJetsmass161_Skimmed_screen -p 0 -X stuff 'source screener'`echo -ne '\015'`
screen -S ChargeAsymmetry_TTJetsmass161_Skimmed_screen -p 0 -X stuff './compile ChargeAsymmetry_TTJetsmass161_Skimmed.C'`echo -ne '\015'`
screen -S ChargeAsymmetry_TTJetsmass161_Skimmed_screen -p 0 -X stuff './ChargeAsymmetry_TTJetsmass161_Skimmed -c testMC_TTJetsmass161.xml'`echo -ne '\015'`

sleep 60

screen -d -m -S ChargeAsymmetry_TTJetsmass163_Skimmed_screen
screen -S ChargeAsymmetry_TTJetsmass163_Skimmed_screen -p 0 -X stuff 'source screener'`echo -ne '\015'`
screen -S ChargeAsymmetry_TTJetsmass163_Skimmed_screen -p 0 -X stuff './compile ChargeAsymmetry_TTJetsmass163_Skimmed.C'`echo -ne '\015'`
screen -S ChargeAsymmetry_TTJetsmass163_Skimmed_screen -p 0 -X stuff './ChargeAsymmetry_TTJetsmass163_Skimmed -c testMC_TTJetsmass163.xml'`echo -ne '\015'`

sleep 60

screen -d -m -S ChargeAsymmetry_TTJetsmass166_Skimmed_screen
screen -S ChargeAsymmetry_TTJetsmass166_Skimmed_screen -p 0 -X stuff 'source screener'`echo -ne '\015'`
screen -S ChargeAsymmetry_TTJetsmass166_Skimmed_screen -p 0 -X stuff './compile ChargeAsymmetry_TTJetsmass166_Skimmed.C'`echo -ne '\015'`
screen -S ChargeAsymmetry_TTJetsmass166_Skimmed_screen -p 0 -X stuff './ChargeAsymmetry_TTJetsmass166_Skimmed -c testMC_TTJetsmass166.xml'`echo -ne '\015'`

sleep 60

screen -d -m -S ChargeAsymmetry_TTJetsmass169_Skimmed_screen
screen -S ChargeAsymmetry_TTJetsmass169_Skimmed_screen -p 0 -X stuff 'source screener'`echo -ne '\015'`
screen -S ChargeAsymmetry_TTJetsmass169_Skimmed_screen -p 0 -X stuff './compile ChargeAsymmetry_TTJetsmass169_Skimmed.C'`echo -ne '\015'`
screen -S ChargeAsymmetry_TTJetsmass169_Skimmed_screen -p 0 -X stuff './ChargeAsymmetry_TTJetsmass169_Skimmed -c testMC_TTJetsmass169.xml'`echo -ne '\015'`

sleep 60

screen -d -m -S ChargeAsymmetry_TTJetsmass178_Skimmed_screen
screen -S ChargeAsymmetry_TTJetsmass178_Skimmed_screen -p 0 -X stuff 'source screener'`echo -ne '\015'`
screen -S ChargeAsymmetry_TTJetsmass178_Skimmed_screen -p 0 -X stuff './compile ChargeAsymmetry_TTJetsmass178_Skimmed.C'`echo -ne '\015'`
screen -S ChargeAsymmetry_TTJetsmass178_Skimmed_screen -p 0 -X stuff './ChargeAsymmetry_TTJetsmass178_Skimmed -c testMC_TTJetsmass178.xml'`echo -ne '\015'`

sleep 60

screen -d -m -S ChargeAsymmetry_TTJetsmass181_Skimmed_screen
screen -S ChargeAsymmetry_TTJetsmass181_Skimmed_screen -p 0 -X stuff 'source screener'`echo -ne '\015'`
screen -S ChargeAsymmetry_TTJetsmass181_Skimmed_screen -p 0 -X stuff './compile ChargeAsymmetry_TTJetsmass181_Skimmed.C'`echo -ne '\015'`
screen -S ChargeAsymmetry_TTJetsmass181_Skimmed_screen -p 0 -X stuff './ChargeAsymmetry_TTJetsmass181_Skimmed -c testMC_TTJetsmass181.xml'`echo -ne '\015'`

sleep 60

screen -d -m -S ChargeAsymmetry_TTJetsmass184_Skimmed_screen
screen -S ChargeAsymmetry_TTJetsmass184_Skimmed_screen -p 0 -X stuff 'source screener'`echo -ne '\015'`
screen -S ChargeAsymmetry_TTJetsmass184_Skimmed_screen -p 0 -X stuff './compile ChargeAsymmetry_TTJetsmass184_Skimmed.C'`echo -ne '\015'`
screen -S ChargeAsymmetry_TTJetsmass184_Skimmed_screen -p 0 -X stuff './ChargeAsymmetry_TTJetsmass184_Skimmed -c testMC_TTJetsmass184.xml'`echo -ne '\015'`

sleep 60

screen -d -m -S ChargeAsymmetry_TTJetsmatchingdown_Skimmed_screen
screen -S ChargeAsymmetry_TTJetsmatchingdown_Skimmed_screen -p 0 -X stuff 'source screener'`echo -ne '\015'`
screen -S ChargeAsymmetry_TTJetsmatchingdown_Skimmed_screen -p 0 -X stuff './compile ChargeAsymmetry_TTJetsmatchingdown_Skimmed.C'`echo -ne '\015'`
screen -S ChargeAsymmetry_TTJetsmatchingdown_Skimmed_screen -p 0 -X stuff './ChargeAsymmetry_TTJetsmatchingdown_Skimmed -c testMC_TTJetsmatchingdown.xml'`echo -ne '\015'`

sleep 60

screen -d -m -S ChargeAsymmetry_TTJetsmatchingup_Skimmed_screen
screen -S ChargeAsymmetry_TTJetsmatchingup_Skimmed_screen -p 0 -X stuff 'source screener'`echo -ne '\015'`
screen -S ChargeAsymmetry_TTJetsmatchingup_Skimmed_screen -p 0 -X stuff './compile ChargeAsymmetry_TTJetsmatchingup_Skimmed.C'`echo -ne '\015'`
screen -S ChargeAsymmetry_TTJetsmatchingup_Skimmed_screen -p 0 -X stuff './ChargeAsymmetry_TTJetsmatchingup_Skimmed -c testMC_TTJetsmatchingup.xml'`echo -ne '\015'`

sleep 60

screen -d -m -S ChargeAsymmetry_TTJetsscaledown_Skimmed_screen
screen -S ChargeAsymmetry_TTJetsscaledown_Skimmed_screen -p 0 -X stuff 'source screener'`echo -ne '\015'`
screen -S ChargeAsymmetry_TTJetsscaledown_Skimmed_screen -p 0 -X stuff './compile ChargeAsymmetry_TTJetsscaledown_Skimmed.C'`echo -ne '\015'`
screen -S ChargeAsymmetry_TTJetsscaledown_Skimmed_screen -p 0 -X stuff './ChargeAsymmetry_TTJetsscaledown_Skimmed -c testMC_TTJetsscaledown.xml'`echo -ne '\015'`

sleep 60

screen -d -m -S ChargeAsymmetry_TTJetsscaleup_Skimmed_screen
screen -S ChargeAsymmetry_TTJetsscaleup_Skimmed_screen -p 0 -X stuff 'source screener'`echo -ne '\015'`
screen -S ChargeAsymmetry_TTJetsscaleup_Skimmed_screen -p 0 -X stuff './compile ChargeAsymmetry_TTJetsscaleup_Skimmed.C'`echo -ne '\015'`
screen -S ChargeAsymmetry_TTJetsscaleup_Skimmed_screen -p 0 -X stuff './ChargeAsymmetry_TTJetsscaleup_Skimmed -c testMC_TTJetsscaleup.xml'`echo -ne '\015'`

sleep 60

screen -d -m -S ChargeAsymmetry_ZJets_PileUpReweighted_screen
screen -S ChargeAsymmetry_ZJets_PileUpReweighted_screen -p 0 -X stuff 'source screener'`echo -ne '\015'`
screen -S ChargeAsymmetry_ZJets_PileUpReweighted_screen -p 0 -X stuff './compile ChargeAsymmetry_ZJets_PileUpReweighted.C'`echo -ne '\015'`
screen -S ChargeAsymmetry_ZJets_PileUpReweighted_screen -p 0 -X stuff './ChargeAsymmetry_ZJets_PileUpReweighted -c testMC_ZJets.xml'`echo -ne '\015'`
