#! /bin/bash

# usage example, from POSITRON workdir
# ./scripts/run-powerfull-JISA.sh

turns=$(seq 1 30)
workdir=$PWD
resultsdir="$workdir/scripts/experiments/results/JISA/powerfull"
scenarios='30nodes 60nodes 90nodes 120nodes 150nodes 180nodes'

for scenario in $scenarios; 
do
    # echo "pfBal,peBal,pfSat,peSat" > $resultsdir/pfair-$scenario.txt
    echo "puBal,puSat" > $resultsdir/pused-$scenario.txt
    echo "ppBal,ppSat" > $resultsdir/ppreempted-$scenario.txt
    echo "msBal,msSat" > $resultsdir/makespan-$scenario.txt
    ./scripts/change-input.sh $scenario
    totalofnodes=${scenario%nodes}

    for turn in $turns; 
    do

        ./ns3 run "main --balanced=true --seed=$turn --powerless=false" > log-bal.tmp 2>&1
        sqlite3 scratch/database.db < scripts/sql/pfair.sql
        mv temp.csv $resultsdir/pfair-$scenario-bal-$turn.csv
        testUsedBal=$(sqlite3 scratch/database.db < scripts/sql/pused.sql)
        testPreemptedBal=$(sqlite3 scratch/database.db < scripts/sql/ppreempted.sql)
        testMakespanBal=$(sqlite3 scratch/database.db < scripts/sql/makespan.sql)

        ./ns3 run "main --balanced=false --seed=$turn --powerless=false" > log-sat.tmp 2>&1
        sqlite3 scratch/database.db < scripts/sql/pfair.sql
        mv temp.csv $resultsdir/pfair-$scenario-sat-$turn.csv
        testUsedSat=$(sqlite3 scratch/database.db < scripts/sql/pused.sql)
        testPreemptedSat=$(sqlite3 scratch/database.db < scripts/sql/ppreempted.sql)
        testMakespanSat=$(sqlite3 scratch/database.db < scripts/sql/makespan.sql)

        echo "$testUsedBal,$testUsedSat" >> $resultsdir/pused-$scenario.txt
        echo "$testPreemptedBal,$testPreemptedSat" >> $resultsdir/ppreempted-$scenario.txt
        echo "$testMakespanBal,$testMakespanSat" >> $resultsdir/makespan-$scenario.txt

    done
    rm log-bal.tmp log-sat.tmp
done