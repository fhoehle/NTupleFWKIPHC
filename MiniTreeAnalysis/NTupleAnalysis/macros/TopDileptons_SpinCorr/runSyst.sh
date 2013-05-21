#!/bin/bash

#btag="../../config/MyCutFlow_1_3.05.xml"

#btag="
#../../config/MyCutFlow_DataMuEG.xml
#../../config/MyCutFlow_DataMu.xml
#../../config/MyCutFlow_DataEG.xml
#../../config/MyCutFlow_TTbar.xml
#../../config/MyCutFlow_Zjets.xml
#../../config/MyCutFlow_DYToLL_M10_50.xml
#../../config/MyCutFlow_ZZ.xml
#../../config/MyCutFlow_Wjets.xml
#../../config/MyCutFlow_WZ.xml
#../../config/MyCutFlow_WW.xml
#../../config/MyCutFlow_TtW.xml
#../../config/MyCutFlow_TbartW.xml
#../../config/MyCutFlow_TTbarTriggerUp.xml
#../../config/MyCutFlow_TTbarTriggerDown.xml
#../../config/MyCutFlow_TTbarMETUp.xml
#../../config/MyCutFlow_TTbarMETDown.xml
#../../config/MyCutFlow_TTbarLeptUp.xml
#../../config/MyCutFlow_TTbarLeptDown.xml
#../../config/MyCutFlow_TTbarPUup.xml
#../../config/MyCutFlow_TTbarPUdown.xml
#../../config/MyCutFlow_TTbarTriggerUp.xml
#../../config/MyCutFlow_TTbarTriggerDown.xml
#../../config/MyCutFlow_TTbarScaleUp.xml
#../../config/MyCutFlow_TTbarScaleDown.xml
#../../config/MyCutFlow_TTbarMass178.xml
#../../config/MyCutFlow_TTbarMass169.xml
#../../config/MyCutFlow_TTbarMass166.xml
#../../config/MyCutFlow_TTbarMatchUp.xml
#../../config/MyCutFlow_TTbarMatchDown.xml
#../../config/MyCutFlow_TtWScaleUp.xml
#../../config/MyCutFlow_TtWScaleDown.xml
#../../config/MyCutFlow_TbartWScaleUp.xml
#../../config/MyCutFlow_TbartWScaleDown.xml
#../../config/MyCutFlow_TTbarMass184.xml
#../../config/MyCutFlow_TTbarMass181.xml
#../../config/MyCutFlow_TTbarMass163.xml
#../../config/MyCutFlow_TTbarMass161.xml
btag="
../../config/MyCutFlow_TTbar.xml
"


echo "start loop "
for i in $btag ; do
        echo "##############################"
        echo "tagger " $i
        echo "##############################"

        cp $i ../../config/MyCutFlow_PLR.xml
        ./MacroProofMyCutFlowPLR
        dir=`ls -ltrhd backup_* | tail -1 | awk '{print $9}'`
        label=`echo $i | awk -F 'MyCutFlow_' '{print $2 }' | awk -F '.xml' '{print $1}'`
        echo $dir $label
        cp ${dir}/proof.root newp_${label}.root

done
