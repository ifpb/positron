#! /bin/bash

# usage example, from POSITRON workdir
# ./scripts/run.sh

turns=$(seq 1 5)
workdir="/usr/local/src/ns3-workspace/positron"
resultsdir="$workdir/scripts/experiments/results"
scenarios='60nodes 90nodes 120nodes 150nodes 180nodes'

for scenario in $scenarios; 
do
    echo "pfBal,pfSat" > $resultsdir/pfair-$scenario.txt
    echo "puBal,puSat" > $resultsdir/pused-$scenario.txt
    ./scripts/change-input.sh $scenario
    totalofnodes=${scenario%nodes}

    for turn in $turns; 
    do
        ./waf --run "main --balanced=true --seed=$turn" 2> /dev/null > log-bal.tmp
        resultBal=$(cat log-bal.tmp | ./scripts/pfair.awk $totalofnodes)
        ./waf --run "main --balanced=false --seed=$turn" 2> /dev/null > log-sat.tmp
        resultSat=$(cat log-sat.tmp | ./scripts/pfair.awk $totalofnodes)
        echo "$resultBal,$resultSat" >> $resultsdir/pfair-$scenario.txt
    done
    # mv log-bal.tmp log-bal-$scenario.tmp
    # mv log-sat.tmp log-sat-$scenario.tmp
    rm log-bal.tmp log-sat.tmp
done