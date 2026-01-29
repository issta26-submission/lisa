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
//<ID> 272
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
    int rc_status = 0;
    int rc_enable = 0;
    int rc_busy = 0;
    int rc_wal = 0;
    int rc_close = 0;
    int status_cur = 0;
    int status_hi = 0;
    int wal_nLog = 0;
    int wal_nCkpt = 0;
    const char *filename = ":memory:";

    // step 2: Setup - open an in-memory database
    rc_open = sqlite3_open_v2(filename, &db, 6, NULL);

    // step 3: Configure - query a status value and use it to decide extension enabling and set busy handler
    rc_status = sqlite3_status(0, &status_cur, &status_hi, 0);
    rc_enable = sqlite3_enable_load_extension(db, (status_cur & 1));
    rc_busy = sqlite3_busy_handler(db, (int (*)(void*,int))NULL, NULL);

    // step 4: Operate, validate and cleanup - perform a WAL checkpoint and close the database
    rc_wal = sqlite3_wal_checkpoint_v2(db, "main", 0, &wal_nLog, &wal_nCkpt);
    rc_close = sqlite3_close_v2(db);
    db = NULL;

    (void)rc_open;
    (void)rc_status;
    (void)rc_enable;
    (void)rc_busy;
    (void)rc_wal;
    (void)rc_close;
    (void)status_cur;
    (void)status_hi;
    (void)wal_nLog;
    (void)wal_nCkpt;
    (void)filename;

    // API sequence test completed successfully
    return 66;
}