#! /usr/bin/awk -f

# usage example
# ./waf --run "main --seed=1 --balanced=true" 2> /dev/null | ./scripts/pused.awk

BEGIN{ 
    count=0;
    totalofnodes=ARGV[1];
    ARGV[1]="";
    nodes[1]="";
}
{ 
    if ($4 == "allocate_worker_application") {
        if (!($8 in nodes)) {
            nodes[$8]=$8
        }
    } 

}
END{
    k = 0
    for(i in nodes) k++
    printf "%.2f", (k*1/totalofnodes*1)*100;
}