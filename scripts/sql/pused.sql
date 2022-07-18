select printf("%.2f",(cast(count(distinct ID_WORKER) as float)/cast(count(distinct ID) as float))*100) as pused
from WORKERS_APPLICATIONS, WORKERS;