-- usage: POSITRON_FOLDER $ sqlite3 scratch/database.db < scripts/sql/test-worker-selection.sql

.headers on

SELECT
    worker,
    battery,
    initial_consumption,
    current_consumption,
    cpu_remaining,
    memory_remaining,
    transmission,
    storage_remaining
FROM (
SELECT
    WORKERS.ID AS worker,
    WORKERS.CPU AS cpu,
    WORKERS.CPU - COALESCE(SUM(APPLICATIONS.CPU), 0) AS cpu_remaining,
    WORKERS.MEMORY AS memory,
    WORKERS.MEMORY - COALESCE(SUM(APPLICATIONS.MEMORY), 0) AS memory_remaining,
    WORKERS.STORAGE AS storage,
    WORKERS.STORAGE - COALESCE(SUM(APPLICATIONS.STORAGE), 0) AS storage_remaining,
    WORKERS.POWER AS battery,
    WORKERS.TRANSMISSION AS transmission,
    WORKERS.INITIAL_CONSUMPTION AS initial_consumption,
    WORKERS.CURRENT_CONSUMPTION + (COUNT(APPLICATIONS.ID) * WORKERS.INITIAL_CONSUMPTION) AS current_consumption,
    COUNT(APPLICATIONS.ID) AS application_quantity
FROM WORKERS
LEFT JOIN WORKERS_APPLICATIONS ON WORKERS_APPLICATIONS.ID_WORKER == WORKERS.ID
LEFT JOIN APPLICATIONS ON APPLICATIONS.ID == WORKERS_APPLICATIONS.ID_APPLICATION AND APPLICATIONS.FINISH == 0
GROUP BY
    WORKERS.ID
ORDER BY
    -- application_quantity ASC,    -- if balanced
    -- cpu_remaining DESC
    -- storage_remaining DESC
    transmission DESC
)
WHERE
    -- cpu_remaining >= 0.4         -- performance
    cpu_remaining >= 0.2            -- storage or transmission
    AND
    -- memory_remaining >= 0.4      -- performance
    memory_remaining >= 0.2         -- storage or transmission
    AND
    storage_remaining >= 2.0        -- performance or transmission
    -- storage_remaining >= 8.0     -- storage
    AND
    battery > 0
LIMIT 1;