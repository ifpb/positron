#! /bin/bash

# for i counter in $(seq 1 15);
# do
#     ./waf --run "main --balanced=true --seed=$counter" 2> /dev/null | awk 'function ceil(x, y){y=int(x); return(x>y?y+1:y)} function app(x){return (x <= 3 ? x : app(x - 3))} BEGIN{ count=0 }{ if ($4 == "allocate_worker_application") {apptype=app($11); nodegroup=ceil(($8*3)/18); aux=(apptype == nodegroup ? 1 : 0); count+=aux; print $10, $11, "tipo", apptype, $7, $8, "grupo", nodegroup, "justa", aux, "count", count} }END{print "percentual total de alocacao justa", (count/39)*100 "%"}' | grep -E "justa 0" >> app_nao_justa.txt
# done

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