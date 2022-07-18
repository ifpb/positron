#! /bin/bash

# usage example, from POSITRON workdir
# ./scripts/run.sh

turns=$(seq 1 30)
workdir=$PWD
resultsdir="$workdir/scripts/experiments/results/powerfull"
scenarios='30nodes 60nodes 90nodes 120nodes 150nodes 180nodes'

for scenario in $scenarios; 
do
    # echo "pfBal,peBal,pfSat,peSat" > $resultsdir/pfair-$scenario.txt
    echo "puBal,puSat" > $resultsdir/pused-$scenario.txt
    ./scripts/change-input.sh $scenario
    totalofnodes=${scenario%nodes}

    for turn in $turns; 
    do

        ./waf --run "main --balanced=true --seed=$turn --powerless=false" > log-bal.tmp 2>&1
        sqlite3 scratch/database.db < scripts/sql/pfair.sql
        mv temp.csv $resultsdir/pfair-$scenario-bal-$turn.csv
        testeBal=$(sqlite3 scratch/database.db < scripts/sql/pused.sql)

        ./waf --run "main --balanced=false --seed=$turn --powerless=false" > log-sat.tmp 2>&1
        sqlite3 scratch/database.db < scripts/sql/pfair.sql
        mv temp.csv $resultsdir/pfair-$scenario-sat-$turn.csv
        testeSat=$(sqlite3 scratch/database.db < scripts/sql/pused.sql)
        
        echo "$testeBal,$testeSat" >> $resultsdir/pused-$scenario.txt
        
        # resultBal=$(cat log-bal.tmp | ./scripts/backup/pfair.awk $totalofnodes)
        # resultSat=$(cat log-sat.tmp | ./scripts/backup/pfair.awk $totalofnodes)
        # finishBal=$(cat log-bal.tmp | ./scripts/backup/pfinish.awk)
        # finishSat=$(cat log-sat.tmp | ./scripts/backup/pfinish.awk)
        # echo "$resultBal,$finishBal,$resultSat,$finishSat" >> $resultsdir/pfair-$scenario.txt

    done
    rm log-bal.tmp log-sat.tmp
done