#! /bin/bash

# usage example, from POSITRON workdir
# ./scripts/run-powerless-JISA.sh

turns=$(seq 1 30)
workdir=$PWD
resultsdir="$workdir/scripts/experiments/results/JISA/powerless"
scenarios='30nodes 60nodes 90nodes 120nodes 150nodes 180nodes'
# lossscenarios='10 20 30'
lossscenarios='30e'
loss='30'

for lossscenario in $lossscenarios;
do

    for scenario in $scenarios; 
    do
        # echo "pfBal,peBal,pfSat,peSat" > $resultsdir/$lossscenario/pfair-$scenario.txt
        echo "puBal,puSat" > $resultsdir/$lossscenario/pused-$scenario.txt
        echo "ppBal,ppSat" > $resultsdir/$lossscenario/ppreempted-$scenario.txt
        echo "msBal,msSat" > $resultsdir/$lossscenario/makespan-$scenario.txt
        ./scripts/change-input.sh $scenario
        totalofnodes=${scenario%nodes}
    
        for turn in $turns; 
        do
    
            ./ns3 run "main --balanced=true --seed=$turn --powerless=true --loss=$loss" > log-bal.tmp 2>&1
            sqlite3 scratch/database.db < scripts/sql/pfair.sql
            mv temp.csv $resultsdir/$lossscenario/pfair-$scenario-bal-$turn.csv
            testUsedBal=$(sqlite3 scratch/database.db < scripts/sql/pused.sql)
            testPreemptedBal=$(sqlite3 scratch/database.db < scripts/sql/ppreempted.sql)
            testMakespanBal=$(sqlite3 scratch/database.db < scripts/sql/makespan.sql)
             
            ./ns3 run "main --balanced=false --seed=$turn --powerless=true --loss=$loss" > log-sat.tmp 2>&1
            sqlite3 scratch/database.db < scripts/sql/pfair.sql
            mv temp.csv $resultsdir/$lossscenario/pfair-$scenario-sat-$turn.csv
            testUsedSat=$(sqlite3 scratch/database.db < scripts/sql/pused.sql)
            testPreemptedSat=$(sqlite3 scratch/database.db < scripts/sql/ppreempted.sql)
            testMakespanSat=$(sqlite3 scratch/database.db < scripts/sql/makespan.sql)
    
            echo "$testUsedBal,$testUsedSat" >> $resultsdir/$lossscenario/pused-$scenario.txt
            echo "$testPreemptedBal,$testPreemptedSat" >> $resultsdir/$lossscenario/ppreempted-$scenario.txt
            echo "$testMakespanBal,$testMakespanSat" >> $resultsdir/$lossscenario/makespan-$scenario.txt
             
        done

        rm log-bal.tmp log-sat.tmp
    done
    
done