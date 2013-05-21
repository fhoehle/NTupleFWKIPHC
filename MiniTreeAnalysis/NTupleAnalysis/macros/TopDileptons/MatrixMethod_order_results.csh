mkdir results
mkdir results/MM_results_DATA
mkdir results/MM_results_MC

mkdir results/root_files_and_logs

cp MatrixMethod*.root results/root_files_and_logs
mv *_log results/root_files_and_logs

mkdir results/Tests

mv *NoWeight.root results/Tests
mv *EpsilonsTest.root results/Tests
mv *EpsilonsTest.pdf results/Tests
mv *PullTest.root results/Tests
mv *PullTest.pdf results/Tests

mkdir results/Contamination
mv *Contamination*.root results/Contamination

mkdir results/MM_results_DATA/DATA
mkdir results/MM_results_DATA/DATA/EECase
mkdir results/MM_results_DATA/DATA/MuMuCase
mkdir results/MM_results_DATA/DATA/EMuCase

mkdir results/MM_results_DATA/DATA_Fast
mkdir results/MM_results_DATA/DATA_Fast/EECase
mkdir results/MM_results_DATA/DATA_Fast/MuMuCase
mkdir results/MM_results_DATA/DATA_Fast/EMuCase

mkdir results/MM_results_MC/MC
mkdir results/MM_results_MC/MC/EECase
mkdir results/MM_results_MC/MC/MuMuCase
mkdir results/MM_results_MC/MC/EMuCase

mkdir results/MM_results_MC/MC_Fast
mkdir results/MM_results_MC/MC_Fast/EECase
mkdir results/MM_results_MC/MC_Fast/MuMuCase
mkdir results/MM_results_MC/MC_Fast/EMuCase

mv *DATA.root results/MM_results_DATA/DATA
mv *DATA.pdf results/MM_results_DATA/DATA
mv *DATA_Fast.root results/MM_results_DATA/DATA_Fast
mv *DATA_Fast.pdf results/MM_results_DATA/DATA_Fast

mv *MC.root results/MM_results_MC/MC
mv *MC.pdf results/MM_results_MC/MC
mv *MC_Fast.root results/MM_results_MC/MC_Fast
mv *MC_Fast.pdf results/MM_results_MC/MC_Fast


mv results/MM_results_DATA/DATA/*EE* results/MM_results_DATA/DATA/EECase
mv results/MM_results_DATA/DATA/*MuMu* results/MM_results_DATA/DATA/MuMuCase
mv results/MM_results_DATA/DATA/*EMu* results/MM_results_DATA/DATA/EMuCase

mv results/MM_results_DATA/DATA_Fast/*EE* results/MM_results_DATA/DATA_Fast/EECase
mv results/MM_results_DATA/DATA_Fast/*MuMu* results/MM_results_DATA/DATA_Fast/MuMuCase
mv results/MM_results_DATA/DATA_Fast/*EMu* results/MM_results_DATA/DATA_Fast/EMuCase

mv results/MM_results_MC/MC/*EE* results/MM_results_MC/MC/EECase
mv results/MM_results_MC/MC/*MuMu* results/MM_results_MC/MC/MuMuCase
mv results/MM_results_MC/MC/*EMu* results/MM_results_MC/MC/EMuCase

mv results/MM_results_MC/MC_Fast/*EE* results/MM_results_MC/MC_Fast/EECase
mv results/MM_results_MC/MC_Fast/*MuMu* results/MM_results_MC/MC_Fast/MuMuCase
mv results/MM_results_MC/MC_Fast/*EMu* results/MM_results_MC/MC_Fast/EMuCase

mkdir results/MM_efficiencies

mv FakeRate*.pdf results/MM_efficiencies
mv SignalEfficiency*.pdf results/MM_efficiencies
mv MatrixMethod_Efficiency*.root results/MM_efficiencies
