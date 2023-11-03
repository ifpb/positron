-- usage: POSITRON_FOLDER $ sqlite3 scratch/database.db < scripts/sql/draft.sql

.headers on
.mode column
-- .mode csv
-- .output g3_times.csv

-- select ID, ID_WORKER, (PERFORMED_AT - START) as diffSTART, (DURATION - (FINISHED_AT - PERFORMED_AT)) as diffDuration
-- from APPLICATIONS, WORKERS_APPLICATIONS 
-- where ID = ID_APPLICATION AND DURATION != (FINISHED_AT - PERFORMED_AT) 
-- order by ID;

-- select ID, ID_WORKER, (DURATION - (FINISHED_AT - PERFORMED_AT)) as diffDuration
-- from APPLICATIONS, WORKERS_APPLICATIONS 
-- where ID = ID_APPLICATION AND (DURATION - (FINISHED_AT - PERFORMED_AT) > 1) 
-- order by ID;

-- select ID, ID_WORKER, (PERFORMED_AT - START) as diffSTART
-- from APPLICATIONS, WORKERS_APPLICATIONS 
-- where ID = ID_APPLICATION AND ((PERFORMED_AT - START) > 0) 
-- order by ID;

-- select ID as app, ID_WORKER as node, DURATION as duration, round(FINISHED_AT - PERFORMED_AT,1) as runtime
-- from APPLICATIONS, WORKERS_APPLICATIONS
-- where ID = ID_APPLICATION and runtime >= DURATION
-- order by ID;

-- select count(*) as total
-- from (
--     select APPLICATIONS.ID as app,
--         case
--             when POLICY like 'performance' then "1"
--             when POLICY like 'storage' then "2"
--             when POLICY like 'transmission' then "3"
--             else "error!!!"
--         end as perfil,
--         WORKERS.ID as node,
--             case
--             when NAME like 'Grupo1-%' then "1"
--             when NAME like 'Grupo2-%' then "2"
--             when NAME like 'Grupo3-%' then "3"
--             else "error!!!"
--         end as grupo
--     from WORKERS, APPLICATIONS, WORKERS_APPLICATIONS
--     where APPLICATIONS.ID = ID_APPLICATION AND WORKERS.ID = ID_WORKER
--     order by app
--     )
-- where perfil != grupo;

-- select count(*) as total, printf("%.2f", count(*) * 100.0 / sum(count(*)) over()) as pfair, fairness as gfair
-- from (
--     select
--     case
--         when POLICY like 'performance' and NAME like 'Grupo1-%' then "fair1"
--         when POLICY like 'storage' and NAME like 'Grupo2-%' then "fair2"
--         when POLICY like 'transmission' and NAME like 'Grupo3-%' then "fair3"
--     else "unfair"
--     end as fairness
--     from WORKERS, APPLICATIONS, WORKERS_APPLICATIONS
--     where APPLICATIONS.ID = ID_APPLICATION AND WORKERS.ID = ID_WORKER
--     -- order by app
-- )
-- group by fairness;

-- select inicio, fim
-- from (
--     select ID_APPLICATION as app, ID_WORKER as worker, PERFORMED_AT as inicio, FINISHED_AT as fim,
--         case
--             when POLICY like 'performance' and NAME like 'Grupo1-%' then "G1"
--             when POLICY like 'storage' and NAME like 'Grupo2-%' then "G2"
--             when POLICY like 'transmission' and NAME like 'Grupo3-%' then "G3"
--         else "error"
--         end as grupo
--     from WORKERS, APPLICATIONS, WORKERS_APPLICATIONS
--     where APPLICATIONS.ID = ID_APPLICATION AND WORKERS.ID = ID_WORKER
--     -- order by grupo, inicio
-- )
-- where grupo = "G3"
-- order by inicio;

-- update WORKERS_APPLICATIONS set FINISHED_AT = 2000 where ID_APPLICATION = 2;


-- select count(*) from (select * from APPLICATIONS where FINISH = 0 and START < 6197 and ID in (select ID_APPLICATION from WORKERS_APPLICATIONS where FINISHED_AT != 0));
-- select * from APPLICATIONS where FINISH = 0 and START < 6197 and ID in (select ID_APPLICATION from WORKERS_APPLICATIONS where FINISHED_AT != 0);

-- select * from APPLICATIONS --where FINISH = 0
-- select * from WORKERS order by ID;
-- select * from WORKERS_APPLICATIONS order by ID_APPLICATION;

-- with WORKERS_APPLICATIONS_FILTRED as (
    -- select *, max(ID_APPLICATION)
    -- from WORKERS_APPLICATIONS
-- )
-- select ID as app, ID_WORKER as node, START as arrival, DURATION as duration, round(FINISHED_AT - START + 0.5,0) as makespan, (DURATION/(round(FINISHED_AT - START + 0.5,0))) as result
-- from APPLICATIONS, WORKERS_APPLICATIONS
-- where ID_APPLICATION in (
--     select max(ID_APPLICATION)
--     from WORKERS_APPLICATIONS
--     group by ID_APPLICATION
-- ) and ID = ID_APPLICATION
-- order by ID;

-- select ID as app, ID_WORKER as node, START as arrival, DURATION as duration, max(FINISHED_AT) as finished, round(max(FINISHED_AT) - START + 0.5,0) as makespan
-- from APPLICATIONS, WORKERS_APPLICATIONS
-- where ID = ID_APPLICATION
-- group by ID_APPLICATION