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
//<ID> 270
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    // step 1: Declarations and Initialize
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    const char *tail = NULL;
    const unsigned char *col_text = NULL;
    int rc_open = 0;
    int rc_enable = 0;
    int rc_busy = 0;
    int rc_exec = 0;
    int rc_prep = 0;
    int rc_step = 0;
    int rc_finalize = 0;
    int rc_close = 0;
    int rc_checkpoint = 0;
    int nLog = 0;
    int nCkpt = 0;
    int status_cur = 0;
    int status_hi = 0;

    rc_open = sqlite3_open_v2(":memory:", &db, 0, NULL);

    // step 2: Configure
    rc_enable = sqlite3_enable_load_extension(db, 1);
    rc_busy = sqlite3_busy_handler(db, NULL, NULL);

    // step 3: Operate and Validate
    rc_exec = sqlite3_exec(db, "CREATE TABLE t(x INTEGER); INSERT INTO t(x) VALUES(42);", NULL, NULL, NULL);
    rc_prep = sqlite3_prepare_v2(db, "SELECT x FROM t;", -1, &stmt, &tail);
    rc_step = sqlite3_step(stmt);
    col_text = sqlite3_column_text(stmt, 0);
    rc_finalize = sqlite3_finalize(stmt);
    stmt = NULL;
    rc_checkpoint = sqlite3_wal_checkpoint_v2(db, "main", 0, &nLog, &nCkpt);
    sqlite3_status(0, &status_cur, &status_hi, 1);

    // step 4: Cleanup
    rc_close = sqlite3_close_v2(db);
    db = NULL;

    (void)tail;
    (void)col_text;
    (void)rc_open;
    (void)rc_enable;
    (void)rc_busy;
    (void)rc_exec;
    (void)rc_prep;
    (void)rc_step;
    (void)rc_finalize;
    (void)rc_checkpoint;
    (void)nLog;
    (void)nCkpt;
    (void)status_cur;
    (void)status_hi;
    (void)rc_close;

    // API sequence test completed successfully
    return 66;
}