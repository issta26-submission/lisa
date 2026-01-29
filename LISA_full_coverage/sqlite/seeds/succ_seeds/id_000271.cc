#include <sqlite3.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstring>
#include <fcntl.h>
//<ID> 271
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    // step 1: Declarations
    sqlite3 *db = NULL;
    int rc_open = 0;
    int rc_enable = 0;
    int rc_busy = 0;
    int rc_status1 = 0;
    int rc_wal = 0;
    int rc_status2 = 0;
    int rc_close = 0;
    int status_current = 0;
    int status_highwater = 0;
    int pnLog = -1;
    int pnCkpt = -1;
    const char *errstr = NULL;

    // step 2: Setup - open an in-memory database and configure extension loading and busy handler
    rc_open = sqlite3_open_v2(":memory:", &db, 0, NULL);
    rc_enable = sqlite3_enable_load_extension(db, 0);
    rc_busy = sqlite3_busy_handler(db, NULL, NULL);

    // step 3: Operate & Validate - query status, perform a WAL checkpoint, query status again and get an error string
    rc_status1 = sqlite3_status(0, &status_current, &status_highwater, 0);
    rc_wal = sqlite3_wal_checkpoint_v2(db, "main", 0, &pnLog, &pnCkpt);
    errstr = sqlite3_errstr(rc_wal);
    rc_status2 = sqlite3_status(0, &status_current, &status_highwater, 1);

    // step 4: Cleanup - close database and tidy up local variables
    rc_close = sqlite3_close_v2(db);
    db = NULL;

    (void)rc_open;
    (void)rc_enable;
    (void)rc_busy;
    (void)rc_status1;
    (void)rc_wal;
    (void)rc_status2;
    (void)rc_close;
    (void)status_current;
    (void)status_highwater;
    (void)pnLog;
    (void)pnCkpt;
    (void)errstr;

    // API sequence test completed successfully
    return 66;
}