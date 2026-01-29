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
//<ID> 274
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    // step 1: Declarations
    sqlite3 *db = NULL;
    const char *zDbName = "main";
    int rc_open = 0;
    int rc_enable_on = 0;
    int rc_busy = 0;
    int rc_status = 0;
    int rc_wal = 0;
    int rc_enable_off = 0;
    int rc_close = 0;
    int wal_nLog = 0;
    int wal_nCkpt = 0;
    int status_cur = 0;
    int status_hiwtr = 0;

    // step 2: Setup - open an in-memory database, enable extension loading, and set busy handler
    rc_open = sqlite3_open_v2(":memory:", &db, 0, NULL);
    rc_enable_on = sqlite3_enable_load_extension(db, 1);
    rc_busy = sqlite3_busy_handler(db, NULL, NULL);
    rc_status = sqlite3_status(0, &status_cur, &status_hiwtr, 0);

    // step 3: Core operations - perform a WAL checkpoint and then disable extension loading
    rc_wal = sqlite3_wal_checkpoint_v2(db, zDbName, 0, &wal_nLog, &wal_nCkpt);
    rc_enable_off = sqlite3_enable_load_extension(db, 0);

    // step 4: Validate and Cleanup - close database and tidy up
    rc_close = sqlite3_close_v2(db);
    db = NULL;

    (void)rc_open;
    (void)rc_enable_on;
    (void)rc_busy;
    (void)rc_status;
    (void)rc_wal;
    (void)rc_enable_off;
    (void)rc_close;
    (void)wal_nLog;
    (void)wal_nCkpt;
    (void)status_cur;
    (void)status_hiwtr;
    // API sequence test completed successfully
    return 66;
}