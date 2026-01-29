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
//<ID> 276
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    // step 1: Declarations & Initialization
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    int rc_open = 0;
    int rc_prep = 0;
    int rc_step = 0;
    int rc_finalize = 0;
    int rc_close = 0;
    int rc_enable = 0;
    int rc_busy = 0;
    int rc_status = 0;
    int rc_checkpoint = 0;
    int status_cur = 0;
    int status_hi = 0;
    int nLog = 0;
    int nCkpt = 0;
    const unsigned char *col_text = NULL;
    const char *pzTail = NULL;

    // step 2: Configure - open in-memory DB, set busy handler (clear), enable extensions, query a status
    rc_open = sqlite3_open_v2(":memory:", &db, 0, NULL);
    rc_busy = sqlite3_busy_handler(db, NULL, NULL);
    rc_enable = sqlite3_enable_load_extension(db, 1);
    rc_status = sqlite3_status(0, &status_cur, &status_hi, 0);

    // step 3: Operate - create table, insert a row, checkpoint WAL, select the row
    rc_prep = sqlite3_prepare_v2(db, "CREATE TABLE t(x TEXT);", -1, &stmt, &pzTail);
    rc_step = sqlite3_step(stmt);
    rc_finalize = sqlite3_finalize(stmt);
    stmt = NULL;
    rc_prep = sqlite3_prepare_v2(db, "INSERT INTO t(x) VALUES('hello');", -1, &stmt, &pzTail);
    rc_step = sqlite3_step(stmt);
    rc_finalize = sqlite3_finalize(stmt);
    stmt = NULL;
    rc_prep = sqlite3_prepare_v2(db, "PRAGMA journal_mode=WAL;", -1, &stmt, &pzTail);
    rc_step = sqlite3_step(stmt);
    rc_finalize = sqlite3_finalize(stmt);
    stmt = NULL;
    rc_checkpoint = sqlite3_wal_checkpoint_v2(db, "main", 0, &nLog, &nCkpt);
    rc_prep = sqlite3_prepare_v2(db, "SELECT x FROM t;", -1, &stmt, &pzTail);
    rc_step = sqlite3_step(stmt);
    col_text = sqlite3_column_text(stmt, 0);

    // step 4: Validate & Cleanup - finalize statement, close DB, and record results
    rc_finalize = sqlite3_finalize(stmt);
    stmt = NULL;
    rc_close = sqlite3_close_v2(db);
    db = NULL;

    (void)rc_open;
    (void)rc_busy;
    (void)rc_enable;
    (void)rc_status;
    (void)rc_prep;
    (void)rc_step;
    (void)rc_finalize;
    (void)rc_checkpoint;
    (void)rc_close;
    (void)status_cur;
    (void)status_hi;
    (void)nLog;
    (void)nCkpt;
    (void)col_text;
    (void)pzTail;

    // API sequence test completed successfully
    return 66;
}