#! /usr/bin/awk -f

# usage example
# ./waf --run "main --seed=1 --balanced=true" 2> /dev/null | ./scripts/pfair.awk

# ceiling function
function ceil(x){
    y=int(x); 
    return(x>y?y+1:y)
} 

# recursively finds the app type (1, 2, or 3)
function app(x){
    return (x <= 3 ? x : app(x - 3))
} 

BEGIN{ 
    count=0 
}
{ 
    if ($4 == "allocate_worker_application") {
        apptype=app($11); 
        nodegroup=ceil(($8*3)/60); 
        aux=(apptype == nodegroup ? 1 : 0); 
        count+=aux; 
        # print $10, $11, "type", apptype, $7, $8, "group", nodegroup, "fair", aux, "count", count
    } 

}
END{
    # print "total percentage of fair allocation", (count/180)*100 "%";
    printf "%.2f", (count/180)*100;
}