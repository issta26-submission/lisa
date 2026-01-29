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
//<ID> 408
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
    const char *sql_create = "CREATE TABLE t(id INTEGER PRIMARY KEY, val TEXT);";
    const char *sql_insert = "INSERT INTO t(val) VALUES('hello');";
    const char *sql_select = "SELECT id, val FROM t WHERE id=1;";
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
    int txn_state = 0;
    void *buffer = NULL;
    void *buffer_freed = NULL;
    int mem_released = 0;
    const unsigned char *coltext = NULL;
    int colid = 0;

    // step 2: Setup - open DB and create/insert a row
    rc_open = sqlite3_open(":memory:", &db);
    rc_exec1 = sqlite3_exec(db, sql_create, NULL, NULL, NULL);
    rc_exec2 = sqlite3_exec(db, sql_insert, NULL, NULL, NULL);

    // step 3: Operate - prepare/select/read, set last insert rowid, check txn, allocate memory
    rc_prepare = sqlite3_prepare_v2(db, sql_select, -1, &stmt, NULL);
    rc_step = sqlite3_step(stmt);
    colid = sqlite3_column_int(stmt, 0);
    coltext = sqlite3_column_text(stmt, 1);
    sqlite3_set_last_insert_rowid(db, set_rowid);
    got_rowid = sqlite3_last_insert_rowid(db);
    txn_state = sqlite3_txn_state(db, "main");
    buffer = sqlite3_realloc(NULL, 128);
    memset(buffer, 0, 128);
    memcpy(buffer, coltext ? (const void *)coltext : (const void *)"", 127);
    mem_released = sqlite3_release_memory(1024);

    // step 4: Validate and Cleanup - reset/finalize stmt, free buffer, close DB
    rc_reset = sqlite3_reset(stmt);
    rc_finalize = sqlite3_finalize(stmt);
    stmt = NULL;
    buffer_freed = sqlite3_realloc(buffer, 0);
    buffer = NULL;
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
    (void)txn_state;
    (void)buffer_freed;
    (void)mem_released;
    (void)colid;
    (void)coltext;

    return 66; // API sequence test completed successfully
}