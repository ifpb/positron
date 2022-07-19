-- usage: POSITRON_FOLDER $ sqlite3 scratch/database.db < scripts/sql/select-all-WA.sql

.headers on
.mode column

select ID_APPLICATION, ID_WORKER, PERFORMED_AT, FINISHED_AT 
from WORKERS_APPLICATIONS
order by ID_APPLICATION;