#! /usr/bin/awk -f

# usage example
# ./waf --run "main --seed=1 --balanced=true" 2> /dev/null | ./scripts/pfinish.awk

BEGIN{ 
    count=0;
}
{ 
    if ($4 == "deallocate_worker_application") {
        count+=$13;
    } 

}
END{
    # print "total percentage of finished applications", (count/180)*100 "%";
    printf "%.2f", (count/180)*100;
}