-- usage: POSITRON_FOLDER $ sqlite3 scratch/database.db < scripts/sql/select-all-W.sql

.headers on
.mode column

select * from WORKERS;