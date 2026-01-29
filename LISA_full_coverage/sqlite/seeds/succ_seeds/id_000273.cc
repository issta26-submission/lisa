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
//<ID> 273
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    // step 1: Declarations
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    int rc_open = 0;
    int rc_prep = 0;
    int rc_step = 0;
    int rc_finalize = 0;
    int rc_enable_ext = 0;
    int rc_busy = 0;
    int rc_status = 0;
    int rc_wal = 0;
    int rc_close = 0;
    int status_cur = 0;
    int status_hi = 0;
    int pnLog = 0;
    int pnCkpt = 0;
    double retrieved_value = 0.0;
    const char *pzTail = NULL;

    // step 2: Setup - open database and configure handlers/options
    rc_open = sqlite3_open(":memory:", &db);
    rc_enable_ext = sqlite3_enable_load_extension(db, 1);
    rc_busy = sqlite3_busy_handler(db, NULL, NULL);

    // step 3: Core operations - create table, insert a row, select and read value
    rc_prep = sqlite3_prepare_v2(db, "CREATE TABLE IF NOT EXISTS t(x REAL);", -1, &stmt, &pzTail);
    rc_step = sqlite3_step(stmt);
    rc_finalize = sqlite3_finalize(stmt);
    stmt = NULL;
    rc_prep = sqlite3_prepare_v2(db, "INSERT INTO t(x) VALUES(3.14159);", -1, &stmt, &pzTail);
    rc_step = sqlite3_step(stmt);
    rc_finalize = sqlite3_finalize(stmt);
    stmt = NULL;
    rc_prep = sqlite3_prepare_v2(db, "SELECT x FROM t LIMIT 1;", -1, &stmt, &pzTail);
    rc_step = sqlite3_step(stmt);
    retrieved_value = sqlite3_column_double(stmt, 0);
    rc_finalize = sqlite3_finalize(stmt);
    stmt = NULL;

    // step 4: Validate and Cleanup - query status, checkpoint WAL, and close
    rc_status = sqlite3_status(0, &status_cur, &status_hi, 0);
    rc_wal = sqlite3_wal_checkpoint_v2(db, "main", 0, &pnLog, &pnCkpt);
    rc_close = sqlite3_close_v2(db);
    db = NULL;

    (void)rc_open;
    (void)rc_enable_ext;
    (void)rc_busy;
    (void)rc_prep;
    (void)rc_step;
    (void)rc_finalize;
    (void)rc_status;
    (void)rc_wal;
    (void)rc_close;
    (void)status_cur;
    (void)status_hi;
    (void)pnLog;
    (void)pnCkpt;
    (void)retrieved_value;
    (void)pzTail;

    // API sequence test completed successfully
    return 66;
}