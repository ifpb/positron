#! /bin/bash

# usage example, from POSITRON workdir
# ./scripts/run.sh

turns=$(seq 1 5)
workdir=$PWD
resultsdir="$workdir/scripts/experiments/results"
scenarios='30nodes 60nodes 90nodes 120nodes 150nodes 180nodes'

for scenario in $scenarios; 
do
    echo "pfBal,peBal,pfSat,peSat" > $resultsdir/pfair-$scenario.txt
    echo "puBal,puSat" > $resultsdir/pused-$scenario.txt
    ./scripts/change-input.sh $scenario
    totalofnodes=${scenario%nodes}

    for turn in $turns; 
    do

        ./waf --run "main --balanced=true --seed=$turn --powerless=true" 2> /dev/null > log-bal.tmp
        testeBal=$(sqlite3 scratch/database.db < scripts/sql/pused.sql)
        ./waf --run "main --balanced=false --seed=$turn --powerless=true" 2> /dev/null > log-sat.tmp
        testeSat=$(sqlite3 scratch/database.db < scripts/sql/pused.sql)
        echo "$testeBal,$testeSat" >> $resultsdir/pused-$scenario.txt
        

        resultBal=$(cat log-bal.tmp | ./scripts/pfair.awk $totalofnodes)
        resultSat=$(cat log-sat.tmp | ./scripts/pfair.awk $totalofnodes)
        finishBal=$(cat log-bal.tmp | ./scripts/pfinish.awk)
        finishSat=$(cat log-sat.tmp | ./scripts/pfinish.awk)
        echo "$resultBal,$finishBal,$resultSat,$finishSat" >> $resultsdir/pfair-$scenario.txt

    done
    # mv log-bal.tmp log-bal-$scenario.tmp
    # mv log-sat.tmp log-sat-$scenario.tmp
    rm log-bal.tmp log-sat.tmp
done