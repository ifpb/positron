#! /bin/bash

# usage example, from POSITRON workdir
# ./scripts/run-powerless.sh

turns=$(seq 1 1)
workdir=$PWD
resultsdir="$workdir/scripts/experiments/results/IJNM23/powerless"
scenarios='30nodes 60nodes 90nodes 120nodes 150nodes 180nodes'
lossscenarios='10 20 30'

for loss in $lossscenarios;
do

    for scenario in $scenarios; 
    do
        # echo "pfBal,peBal,pfSat,peSat" > $resultsdir/$loss/pfair-$scenario.txt
        echo "puBal,puSat" > $resultsdir/$loss/pused-$scenario.txt
        echo "ppBal,ppSat" > $resultsdir/$loss/ppreempted-$scenario.txt
        ./scripts/change-input.sh $scenario
        totalofnodes=${scenario%nodes}
    
        for turn in $turns; 
        do
    
            ./ns3 run "main --balanced=true --seed=$turn --powerless=true --loss=$loss" > log-bal.tmp 2>&1
            sqlite3 scratch/database.db < scripts/sql/pfair.sql
            mv temp.csv $resultsdir/$loss/pfair-$scenario-bal-$turn.csv
            testUsedBal=$(sqlite3 scratch/database.db < scripts/sql/pused.sql)
            testPreemptedBal=$(sqlite3 scratch/database.db < scripts/sql/ppreempted.sql)
             
            ./ns3 run "main --balanced=false --seed=$turn --powerless=true --loss=$loss" > log-sat.tmp 2>&1
            sqlite3 scratch/database.db < scripts/sql/pfair.sql
            mv temp.csv $resultsdir/$loss/pfair-$scenario-sat-$turn.csv
            testUsedSat=$(sqlite3 scratch/database.db < scripts/sql/pused.sql)
            testPreemptedSat=$(sqlite3 scratch/database.db < scripts/sql/ppreempted.sql)
    
            echo "$testUsedBal,$testUsedSat" >> $resultsdir/$loss/pused-$scenario.txt
            echo "$testPreemptedBal,$testPreemptedSat" >> $resultsdir/$loss/ppreempted-$scenario.txt
             
        done

        rm log-bal.tmp log-sat.tmp
    done
    
done