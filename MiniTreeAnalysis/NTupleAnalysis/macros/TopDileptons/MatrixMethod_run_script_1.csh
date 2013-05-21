screen -d -m -S MatrixMethodGeneral_DATA_EMu_screen
screen -S MatrixMethodGeneral_DATA_EMu_screen -p 0 -X stuff 'source screener'`echo -ne '\015'`
screen -S MatrixMethodGeneral_DATA_EMu_screen -p 0 -X stuff './compile MatrixMethodGeneral_DATA_EMu.C'`echo -ne '\015'`
screen -S MatrixMethodGeneral_DATA_EMu_screen -p 0 -X stuff './MatrixMethodGeneral_DATA_EMu > MatrixMethodGeneral_DATA_EMu_log'`echo -ne '\015'`

sleep 60

screen -d -m -S MatrixMethod_DATA_E_screen
screen -S MatrixMethod_DATA_E_screen -p 0 -X stuff 'source screener'`echo -ne '\015'`
screen -S MatrixMethod_DATA_E_screen -p 0 -X stuff './compile MatrixMethod_DATA_E.C'`echo -ne '\015'`
screen -S MatrixMethod_DATA_E_screen -p 0 -X stuff './MatrixMethod_DATA_E > MatrixMethod_DATA_E_log'`echo -ne '\015'`

sleep 60

screen -d -m -S MatrixMethod_DATA_Mu_screen
screen -S MatrixMethod_DATA_Mu_screen -p 0 -X stuff 'source screener'`echo -ne '\015'`
screen -S MatrixMethod_DATA_Mu_screen -p 0 -X stuff './compile MatrixMethod_DATA_Mu.C'`echo -ne '\015'`
screen -S MatrixMethod_DATA_Mu_screen -p 0 -X stuff './MatrixMethod_DATA_Mu > MatrixMethod_DATA_Mu_log'`echo -ne '\015'`

sleep 60

screen -d -m -S MatrixMethodGeneral_MC_screen
screen -S MatrixMethodGeneral_MC_screen -p 0 -X stuff 'source screener'`echo -ne '\015'`
screen -S MatrixMethodGeneral_MC_screen -p 0 -X stuff './compile MatrixMethodGeneral_MC.C'`echo -ne '\015'`
screen -S MatrixMethodGeneral_MC_screen -p 0 -X stuff './MatrixMethodGeneral_MC > MatrixMethodGeneral_MC_log'`echo -ne '\015'`

sleep 60

screen -d -m -S MatrixMethod_MC_screen
screen -S MatrixMethod_MC_screen -p 0 -X stuff 'source screener'`echo -ne '\015'`
screen -S MatrixMethod_MC_screen -p 0 -X stuff './compile MatrixMethod_MC.C'`echo -ne '\015'`
screen -S MatrixMethod_MC_screen -p 0 -X stuff './MatrixMethod_MC > MatrixMethod_MC_log'`echo -ne '\015'`

sleep 60

screen -d -m -S MatrixMethod_MC_NoWeight_screen
screen -S MatrixMethod_MC_NoWeight_screen -p 0 -X stuff 'source screener'`echo -ne '\015'`
screen -S MatrixMethod_MC_NoWeight_screen -p 0 -X stuff './compile MatrixMethod_MC_NoWeight.C'`echo -ne '\015'`
screen -S MatrixMethod_MC_NoWeight_screen -p 0 -X stuff './MatrixMethod_MC_NoWeight > MatrixMethod_MC_NoWeight_log'`echo -ne '\015'`

sleep 60

screen -d -m -S MatrixMethod_Contamination_Studies_screen
screen -S MatrixMethod_Contamination_Studies_screen -p 0 -X stuff 'source screener'`echo -ne '\015'`
screen -S MatrixMethod_Contamination_Studies_screen -p 0 -X stuff './compile MatrixMethod_Contamination_Studies.C'`echo -ne '\015'`
screen -S MatrixMethod_Contamination_Studies_screen -p 0 -X stuff './MatrixMethod_Contamination_Studies > MatrixMethod_Contamination_Studies_log'`echo -ne '\015'`

sleep 60

screen -d -m -S MatrixMethod_Efficiency_MC_Estimation_E_screen
screen -S MatrixMethod_Efficiency_MC_Estimation_E_screen -p 0 -X stuff 'source screener'`echo -ne '\015'`
screen -S MatrixMethod_Efficiency_MC_Estimation_E_screen -p 0 -X stuff './compile MatrixMethod_Efficiency_MC_Estimation_E.C'`echo -ne '\015'`
screen -S MatrixMethod_Efficiency_MC_Estimation_E_screen -p 0 -X stuff './MatrixMethod_Efficiency_MC_Estimation_E > MatrixMethod_Efficiency_MC_Estimation_E_log'`echo -ne '\015'`

sleep 60

screen -d -m -S MatrixMethod_Efficiency_MC_Estimation_Mu_screen
screen -S MatrixMethod_Efficiency_MC_Estimation_Mu_screen -p 0 -X stuff 'source screener'`echo -ne '\015'`
screen -S MatrixMethod_Efficiency_MC_Estimation_Mu_screen -p 0 -X stuff './compile MatrixMethod_Efficiency_MC_Estimation_Mu.C'`echo -ne '\015'`
screen -S MatrixMethod_Efficiency_MC_Estimation_Mu_screen -p 0 -X stuff './MatrixMethod_Efficiency_MC_Estimation_Mu > MatrixMethod_Efficiency_MC_Estimation_Mu_log'`echo -ne '\015'`

sleep 60


screen -d -m -S MatrixMethod_Efficiency_DATA_Estimation_E_screen
screen -S MatrixMethod_Efficiency_DATA_Estimation_E_screen -p 0 -X stuff 'source screener'`echo -ne '\015'`
screen -S MatrixMethod_Efficiency_DATA_Estimation_E_screen -p 0 -X stuff './compile MatrixMethod_Efficiency_DATA_Estimation_E.C'`echo -ne '\015'`
screen -S MatrixMethod_Efficiency_DATA_Estimation_E_screen -p 0 -X stuff './MatrixMethod_Efficiency_DATA_Estimation_E > MatrixMethod_Efficiency_DATA_Estimation_E_log'`echo -ne '\015'`

sleep 60

screen -d -m -S MatrixMethod_Efficiency_DATA_Estimation_Mu_screen
screen -S MatrixMethod_Efficiency_DATA_Estimation_Mu_screen -p 0 -X stuff 'source screener'`echo -ne '\015'`
screen -S MatrixMethod_Efficiency_DATA_Estimation_Mu_screen -p 0 -X stuff './compile MatrixMethod_Efficiency_DATA_Estimation_Mu.C'`echo -ne '\015'`
screen -S MatrixMethod_Efficiency_DATA_Estimation_Mu_screen -p 0 -X stuff './MatrixMethod_Efficiency_DATA_Estimation_Mu > MatrixMethod_Efficiency_DATA_Estimation_Mu_log'`echo -ne '\015'`
