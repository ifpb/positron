-- usage: POSITRON_FOLDER $ sqlite3 scratch/database.db < scripts/sql/pfair.sql

.headers on
-- .mode column
.mode csv
.output temp.csv

select count(*) as total, printf("%.2f", count(*) * 100.0 / sum(count(*)) over()) as pfair, fairness as gfair
from (
    select
    case
        when POLICY like 'performance' and NAME like 'Grupo1-%' then "fair1"
        when POLICY like 'storage' and NAME like 'Grupo2-%' then "fair2"
        when POLICY like 'transmission' and NAME like 'Grupo3-%' then "fair3"
    else "unfair"
    end as fairness
    from WORKERS, APPLICATIONS, WORKERS_APPLICATIONS
    where APPLICATIONS.ID = ID_APPLICATION AND WORKERS.ID = ID_WORKER
    -- order by app
)
group by fairness;