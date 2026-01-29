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
//<ID> 400
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
    char *errmsg = NULL;
    void *buf = NULL;
    void *buf2 = NULL;
    const char *sql_create = "CREATE TABLE t(a INTEGER);";
    const char *sql_insert = "INSERT INTO t(a) VALUES(7);";
    const char *sql_select = "SELECT a FROM t LIMIT 1;";
    int rc_open = 0;
    int rc_exec1 = 0;
    int rc_exec2 = 0;
    int rc_prepare = 0;
    int rc_step = 0;
    int rc_reset = 0;
    int rc_finalize = 0;
    int rc_close = 0;
    int txn_state = 0;
    int released = 0;
    int colval = 0;
    sqlite3_int64 set_rowid = 12345;

    // step 2: Setup - open DB and create/insert a row
    rc_open = sqlite3_open(":memory:", &db);
    rc_exec1 = sqlite3_exec(db, sql_create, NULL, NULL, &errmsg);
    rc_exec2 = sqlite3_exec(db, sql_insert, NULL, NULL, &errmsg);

    // step 3: Operate - adjust last rowid, inspect transaction state, query, realloc, and release memory
    sqlite3_set_last_insert_rowid(db, set_rowid);
    txn_state = sqlite3_txn_state(db, "main");
    rc_prepare = sqlite3_prepare_v2(db, sql_select, -1, &stmt, NULL);
    rc_step = sqlite3_step(stmt);
    colval = sqlite3_column_int(stmt, 0);
    buf = sqlite3_realloc(NULL, 64);
    buf2 = sqlite3_realloc(buf, 128);
    released = sqlite3_release_memory(colval + 256);

    // step 4: Cleanup - reset/finalize statement, free messages, free buffers and close DB
    rc_reset = sqlite3_reset(stmt);
    rc_finalize = sqlite3_finalize(stmt);
    stmt = NULL;
    if (errmsg) { sqlite3_free(errmsg); errmsg = NULL; }
    sqlite3_realloc(buf2, 0);
    rc_close = sqlite3_close_v2(db);
    db = NULL;

    (void)rc_open;
    (void)rc_exec1;
    (void)rc_exec2;
    (void)rc_prepare;
    (void)rc_step;
    (void)rc_reset;
    (void)rc_finalize;
    (void)rc_close;
    (void)txn_state;
    (void)released;
    (void)colval;
    (void)set_rowid;
    (void)buf;
    (void)buf2;

    // API sequence test completed successfully
    return 66;
}