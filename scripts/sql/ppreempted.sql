-- usage: POSITRON_FOLDER $ sqlite3 scratch/database.db < scripts/sql/preempted.sql
-- .headers on
-- .mode column

select printf("%.2f", 
cast(sum(preempted) * 100 as float)/cast(total_apps as float)) as preempted_apps 
from (
    select (select count(ID) from APPLICATIONS) as total_apps, count(*)>1 as preempted
    from WORKERS_APPLICATIONS
    group by ID_APPLICATION having count(*)>1
    
);






