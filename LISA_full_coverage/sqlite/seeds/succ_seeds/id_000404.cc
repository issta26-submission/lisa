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
//<ID> 404
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
    const char *sql_create = "CREATE TABLE t(a INTEGER);";
    const char *sql_insert = "INSERT INTO t(a) VALUES(42);";
    const char *sql_select = "SELECT a FROM t LIMIT 1;";
    int rc_open = 0;
    int rc_exec1 = 0;
    int rc_exec2 = 0;
    int rc_prepare = 0;
    int rc_step = 0;
    int rc_reset = 0;
    int rc_finalize = 0;
    int rc_close = 0;
    sqlite3_int64 set_rowid = 12345;
    sqlite3_int64 got_rowid = 0;
    int colval = 0;
    int txn_state = 0;
    void *mem = NULL;
    void *mem2 = NULL;
    int release_result = 0;
    int tmp_len = 64;

    // step 2: Setup - open DB and create/insert a row
    rc_open = sqlite3_open(":memory:", &db);
    rc_exec1 = sqlite3_exec(db, sql_create, NULL, NULL, NULL);
    rc_exec2 = sqlite3_exec(db, sql_insert, NULL, NULL, NULL);

    // step 3: Operate - set last insert rowid, prepare/select and inspect, check txn state
    sqlite3_set_last_insert_rowid(db, set_rowid);
    got_rowid = sqlite3_last_insert_rowid(db);

    rc_prepare = sqlite3_prepare_v2(db, sql_select, -1, &stmt, NULL);
    rc_step = sqlite3_step(stmt);
    colval = sqlite3_column_int(stmt, 0);
    rc_reset = sqlite3_reset(stmt);
    rc_finalize = sqlite3_finalize(stmt);
    stmt = NULL;

    txn_state = sqlite3_txn_state(db, "main");

    // step 4: Validate resources and Cleanup - allocate/reallocate/free memory, release memory, close DB
    mem = sqlite3_realloc(NULL, tmp_len);
    if (mem) memset(mem, 0, tmp_len);
    mem2 = sqlite3_realloc(mem, tmp_len * 2);
    if (mem2) memset(mem2, 'A', tmp_len * 2);
    mem2 = sqlite3_realloc(mem2, 0);
    mem = NULL;
    release_result = sqlite3_release_memory(4096);
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
    (void)set_rowid;
    (void)got_rowid;
    (void)colval;
    (void)txn_state;
    (void)mem;
    (void)mem2;
    (void)release_result;
    (void)tmp_len;

    // API sequence test completed successfully
    return 66;
}