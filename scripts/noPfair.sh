# for i counter in $(seq 1 15);
# do
#     ./waf --run "main --balanced=true --seed=$counter" 2> /dev/null | awk 'function ceil(x, y){y=int(x); return(x>y?y+1:y)} function app(x){return (x <= 3 ? x : app(x - 3))} BEGIN{ count=0 }{ if ($4 == "allocate_worker_application") {apptype=app($11); nodegroup=ceil(($8*3)/18); aux=(apptype == nodegroup ? 1 : 0); count+=aux; print $10, $11, "tipo", apptype, $7, $8, "grupo", nodegroup, "justa", aux, "count", count} }END{print "percentual total de alocacao justa", (count/39)*100 "%"}' | grep -E "justa 0" >> app_nao_justa.txt
# done
for i in $(seq 1 15);
do
    ./waf --run "main --balanced=true --seed=$i" 2> /dev/null | awk 'function ceil(x, y){y=int(x); return(x>y?y+1:y)} function app(x){return (x <= 3 ? x : app(x - 3))} BEGIN{ count=0 }{ if ($4 == "allocate_worker_application") {apptype=app($11); nodegroup=ceil(($8*3)/18); aux=(apptype == nodegroup ? 1 : 0); count+=aux; print $10, $11, "tipo", apptype, $7, $8, "grupo", nodegroup, "justa", aux, "count", count} }END{print "percentual total de alocacao justa", (count/39)*100 "%"}' | grep -E "justa 0" >> app_nao_justa.txt

    cat app_nao_justa.txt | awk -v r="$i" '
    BEGIN{ count_1=0;  count_2=0;  count_3=0 } 
    { if ($4 == "1") { count_1+=1 }}
    { if ($4 == "2") { count_2+=1 }} 
    { if ($4 == "3") { count_3+=1 }} 
    END{print "round " r " - qtde não justa " "tipo 1: " count_1 " tipo 2: " count_2 " tipo 3: " count_3}
    '
    echo -n >app_nao_justa.txt
done