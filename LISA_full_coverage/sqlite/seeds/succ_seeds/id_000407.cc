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
//<ID> 407
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
    const char *sql_create = "CREATE TABLE t(a INTEGER PRIMARY KEY, val TEXT);";
    const char *sql_insert = "INSERT INTO t(val) VALUES('hello');";
    const char *sql_select = "SELECT a, val FROM t WHERE a=1;";
    int rc_open = 0;
    int rc_exec1 = 0;
    int rc_exec2 = 0;
    int rc_prepare = 0;
    int rc_step = 0;
    int rc_finalize = 0;
    int rc_close = 0;
    sqlite3_int64 lastid_before = 0;
    sqlite3_int64 lastid_after = 0;
    int txn_state_val = 0;
    int mem_released = 0;
    void *buf = NULL;
    const unsigned char *coltext = NULL;
    const void *filename16 = (const void *)L":memory:";

    // step 2: Setup - open DB and create/insert a row
    rc_open = sqlite3_open16(filename16, &db);
    rc_exec1 = sqlite3_exec(db, sql_create, NULL, NULL, NULL);
    rc_exec2 = sqlite3_exec(db, sql_insert, NULL, NULL, NULL);

    // step 3: Operate - read last insert id, set a new last insert id, prepare and step a select
    lastid_before = sqlite3_last_insert_rowid(db);
    sqlite3_set_last_insert_rowid(db, lastid_before + 10);
    lastid_after = sqlite3_last_insert_rowid(db);
    rc_prepare = sqlite3_prepare_v2(db, sql_select, -1, &stmt, NULL);
    rc_step = sqlite3_step(stmt);
    coltext = sqlite3_column_text(stmt, 1);

    // step 4: Validate & Cleanup - check transaction state, release memory, use realloc, finalize and close
    txn_state_val = sqlite3_txn_state(db, "main");
    mem_released = sqlite3_release_memory(1024);
    buf = sqlite3_realloc(NULL, 128);
    buf = sqlite3_realloc(buf, 0);
    rc_finalize = sqlite3_finalize(stmt);
    rc_close = sqlite3_close_v2(db);
    db = NULL;
    stmt = NULL;

    (void)rc_open;
    (void)rc_exec1;
    (void)rc_exec2;
    (void)rc_prepare;
    (void)rc_step;
    (void)rc_finalize;
    (void)rc_close;
    (void)lastid_before;
    (void)lastid_after;
    (void)txn_state_val;
    (void)mem_released;
    (void)buf;
    (void)coltext;

    // API sequence test completed successfully
    return 66;
}