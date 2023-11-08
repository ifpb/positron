-- usage: POSITRON_FOLDER $ sqlite3 scratch/database.db < scripts/sql/pefficient.sql

.headers on
-- .mode column
.mode csv
.output temp.csv

select count(*) as total, printf("%.2f", count(*) * 100.0 / sum(count(*)) over()) as pefficient, efficiency as gefficient
from (
    select
    case
        when POLICY like 'performance' and NAME like 'Grupo1-%' then "efficient1"
        when POLICY like 'storage' and NAME like 'Grupo2-%' then "efficient2"
        when POLICY like 'transmission' and NAME like 'Grupo3-%' then "efficient3"
    else "inefficient"
    end as efficiency
    from WORKERS, APPLICATIONS, WORKERS_APPLICATIONS
    where APPLICATIONS.ID = ID_APPLICATION AND WORKERS.ID = ID_WORKER
    -- order by app
)
group by efficiency;