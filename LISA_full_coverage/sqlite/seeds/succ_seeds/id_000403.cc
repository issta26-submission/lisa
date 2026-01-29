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
//<ID> 403
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    // step 1: Declarations and Initialization
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    const char *sql_create = "CREATE TABLE t(id INTEGER PRIMARY KEY, val TEXT);";
    const char *sql_insert = "INSERT INTO t(val) VALUES('one'),('two');";
    const char *sql_select = "SELECT id, val FROM t WHERE id=1;";
    int rc_open = 0;
    int rc_exec_create = 0;
    int rc_exec_insert = 0;
    int rc_prepare = 0;
    int rc_step = 0;
    int rc_finalize = 0;
    int rc_close = 0;
    const unsigned char *coltext = NULL;
    int colid = 0;
    sqlite3_int64 lastid_before = 0;
    sqlite3_int64 lastid_after = 0;
    int txn_state_before = 0;
    int txn_state_after = 0;
    int release_rc = 0;
    void *mem_ptr = NULL;

    rc_open = sqlite3_open(":memory:", &db);
    rc_exec_create = sqlite3_exec(db, sql_create, NULL, NULL, NULL);
    rc_exec_insert = sqlite3_exec(db, sql_insert, NULL, NULL, NULL);

    // step 2: Configuration (set last insert id, check txn state, request memory release)
    sqlite3_set_last_insert_rowid(db, (sqlite3_int64)12345);
    lastid_before = sqlite3_last_insert_rowid(db);
    txn_state_before = sqlite3_txn_state(db, "main");
    release_rc = sqlite3_release_memory(4096);

    // step 3: Operate (prepare/step/inspect and use sqlite3_realloc for a string buffer)
    rc_prepare = sqlite3_prepare_v2(db, sql_select, -1, &stmt, NULL);
    rc_step = sqlite3_step(stmt);
    colid = sqlite3_column_int(stmt, 0);
    coltext = sqlite3_column_text(stmt, 1);
    mem_ptr = sqlite3_realloc(NULL, 64);
    sqlite3_snprintf(64, (char *)mem_ptr, "row id=%d val=%s", colid, coltext ? (const char *)coltext : "(null)");
    mem_ptr = sqlite3_realloc(mem_ptr, 128);
    sqlite3_snprintf(128, (char *)mem_ptr, "updated -> id=%d,val=%s,lastid_before=%lld", colid, coltext ? (const char *)coltext : "(null)", (long long)lastid_before);
    sqlite3_set_last_insert_rowid(db, (sqlite3_int64)colid);
    lastid_after = sqlite3_last_insert_rowid(db);
    txn_state_after = sqlite3_txn_state(db, "main");

    // step 4: Validate and Cleanup
    rc_finalize = sqlite3_finalize(stmt);
    stmt = NULL;
    mem_ptr = sqlite3_realloc(mem_ptr, 0);
    rc_close = sqlite3_close_v2(db);
    db = NULL;

    (void)rc_open;
    (void)rc_exec_create;
    (void)rc_exec_insert;
    (void)rc_prepare;
    (void)rc_step;
    (void)rc_finalize;
    (void)rc_close;
    (void)coltext;
    (void)colid;
    (void)lastid_before;
    (void)lastid_after;
    (void)txn_state_before;
    (void)txn_state_after;
    (void)release_rc;
    (void)mem_ptr;

    // API sequence test completed successfully
    return 66;
}