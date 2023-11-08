select printf("%.2f", avg(duration/makespan)) as result
from (
    select ID as app, ID_WORKER as node, START as arrival, DURATION as duration, max(FINISHED_AT) as finished, round(max(FINISHED_AT) - START + 0.5,0) as makespan
    from APPLICATIONS, WORKERS_APPLICATIONS
    where ID = ID_APPLICATION and FINISH != 0
    group by ID_APPLICATION
)