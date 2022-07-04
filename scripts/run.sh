#! /bin/bash

# usage example, from POSITRON workdir
# ./scripts/run.sh

turns=$(seq 1 1)
workdir=$PWD
resultsdir="$workdir/scripts/experiments/results"
scenarios='30nodes 60nodes 90nodes 120nodes 150nodes 180nodes'

for scenario in $scenarios; 
do
    echo "pfBal,pfSat" > $resultsdir/pfair-$scenario.txt
    echo "puBal,puSat" > $resultsdir/pused-$scenario.txt
    echo "pfnBal,pfnSat" > $resultsdir/pfinish-$scenario.txt
    ./scripts/change-input.sh $scenario
    totalofnodes=${scenario%nodes}

    for turn in $turns; 
    do
        ./waf --run "main --balanced=true --seed=$turn" 2> /dev/null > log-bal.tmp
        ./waf --run "main --balanced=false --seed=$turn" 2> /dev/null > log-sat.tmp

        resultBal=$(cat log-bal.tmp | ./scripts/pfair.awk $totalofnodes)
        resultSat=$(cat log-sat.tmp | ./scripts/pfair.awk $totalofnodes)
        echo "$resultBal,$resultSat" >> $resultsdir/pfair-$scenario.txt

        resultBal=$(cat log-bal.tmp | ./scripts/pused.awk $totalofnodes)
        resultSat=$(cat log-sat.tmp | ./scripts/pused.awk $totalofnodes)
        echo "$resultBal,$resultSat" >> $resultsdir/pused-$scenario.txt

        resultBal=$(cat log-bal.tmp | ./scripts/pfinish.awk)
        resultSat=$(cat log-sat.tmp | ./scripts/pfinish.awk)
        echo "$resultBal,$resultSat" >> $resultsdir/pfinish-$scenario.txt
    done
    # mv log-bal.tmp log-bal-$scenario.tmp
    # mv log-sat.tmp log-sat-$scenario.tmp
    rm log-bal.tmp log-sat.tmp
done