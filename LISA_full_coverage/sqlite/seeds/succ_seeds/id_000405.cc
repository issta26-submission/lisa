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
//<ID> 405
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
    const void *filename16 = (const void*)L":memory:";
    const char *sql_create = "CREATE TABLE t(a INTEGER);";
    const char *sql_insert = "INSERT INTO t(a) VALUES(7);";
    int rc_open = 0;
    int rc_prepare = 0;
    int rc_step = 0;
    int rc_finalize = 0;
    int rc_close = 0;
    int rc_release = 0;
    int txn_state = 0;
    sqlite3_int64 last_id = 0;
    void *mem = NULL;

    // step 2: Setup - open DB and create table
    rc_open = sqlite3_open16(filename16, &db);
    rc_prepare = sqlite3_prepare_v2(db, sql_create, -1, &stmt, NULL);
    rc_step = sqlite3_step(stmt);
    rc_finalize = sqlite3_finalize(stmt);
    stmt = NULL;

    // step 3: Operate - insert a row, set last insert rowid, check txn state, memory ops
    rc_prepare = sqlite3_prepare_v2(db, sql_insert, -1, &stmt, NULL);
    rc_step = sqlite3_step(stmt);
    rc_finalize = sqlite3_finalize(stmt);
    stmt = NULL;
    last_id = (sqlite3_int64)77;
    sqlite3_set_last_insert_rowid(db, last_id);
    txn_state = sqlite3_txn_state(db, "main");
    mem = sqlite3_realloc(NULL, 64);
    memset(mem, 0, 64);
    ((char *)mem)[0] = 'X';
    mem = sqlite3_realloc(mem, 128);
    ((char *)mem)[1] = 'Y';
    rc_release = sqlite3_release_memory(1024);

    // step 4: Cleanup and validation - free memory and close DB
    mem = sqlite3_realloc(mem, 0);
    rc_close = sqlite3_close_v2(db);
    db = NULL;

    (void)rc_open;
    (void)rc_prepare;
    (void)rc_step;
    (void)rc_finalize;
    (void)rc_close;
    (void)rc_release;
    (void)txn_state;
    (void)last_id;
    (void)mem;

    // API sequence test completed successfully
    return 66;
}