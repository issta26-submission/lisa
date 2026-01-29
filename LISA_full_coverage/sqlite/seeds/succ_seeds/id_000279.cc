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
//<ID> 279
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
    int rc_busy = 0;
    int rc_enable_on = 0;
    int rc_enable_off = 0;
    int rc_ckpt = 0;
    int rc_status = 0;
    int rc_close = 0;
    int pnLog = 0;
    int pnCkpt = 0;
    int statusCurrent = 0;
    int statusHigh = 0;
    int statusOp = 0;

    // step 2: Setup - open an in-memory database
    rc_open = sqlite3_open_v2(":memory:", &db, 0, NULL);

    // step 3: Configure and Operate - set/clear busy handler, enable load_extension, checkpoint WAL, query status
    rc_busy = sqlite3_busy_handler(db, (int (*)(void *, int))NULL, NULL);
    rc_enable_on = sqlite3_enable_load_extension(db, 1);
    rc_ckpt = sqlite3_wal_checkpoint_v2(db, "main", 0, &pnLog, &pnCkpt);
    statusOp = 0;
    rc_status = sqlite3_status(statusOp, &statusCurrent, &statusHigh, 0);
    rc_enable_off = sqlite3_enable_load_extension(db, 0);

    // step 4: Cleanup and finalize
    rc_close = sqlite3_close_v2(db);
    db = NULL;

    (void)rc_open;
    (void)rc_busy;
    (void)rc_enable_on;
    (void)rc_ckpt;
    (void)pnLog;
    (void)pnCkpt;
    (void)rc_status;
    (void)statusCurrent;
    (void)statusHigh;
    (void)statusOp;
    (void)rc_enable_off;
    (void)rc_close;

    // API sequence test completed successfully
    return 66;
}