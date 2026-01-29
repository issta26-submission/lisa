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
//<ID> 402
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
    void *tmp_buf = NULL;
    const unsigned char *col_text = NULL;
    const char *sql_create = "CREATE TABLE t(id INTEGER PRIMARY KEY, val TEXT);";
    const char *sql_insert = "INSERT INTO t(val) VALUES('alpha');";
    const char *sql_select = "SELECT id, val FROM t WHERE id=1;";
    int rc_open = 0;
    int rc_exec = 0;
    int rc_prepare = 0;
    int rc_step = 0;
    int rc_finalize = 0;
    int rc_close = 0;
    int rc_release = 0;
    sqlite3_int64 set_id = (sqlite3_int64)98765;
    sqlite3_int64 got_id = 0;
    int txn_state = 0;
    int id_from_row = 0;

    // step 2: Setup - open DB, create table and insert a row
    rc_open = sqlite3_open(":memory:", &db);
    rc_exec = sqlite3_exec(db, sql_create, NULL, NULL, NULL);
    rc_prepare = sqlite3_prepare_v2(db, sql_insert, -1, &stmt, NULL);
    rc_step = sqlite3_step(stmt);
    rc_finalize = sqlite3_finalize(stmt);
    stmt = NULL;

    // step 3: Operate - set last insert rowid, query it, and exercise txn/memory APIs
    sqlite3_set_last_insert_rowid(db, set_id);
    got_id = sqlite3_last_insert_rowid(db);
    txn_state = sqlite3_txn_state(db, "main");
    rc_release = sqlite3_release_memory(1024);
    tmp_buf = sqlite3_realloc(NULL, 256);
    memset(tmp_buf, 0, 256);
    sqlite3_snprintf(256, (char *)tmp_buf, "%lld", (long long)got_id);
    rc_prepare = sqlite3_prepare_v2(db, sql_select, -1, &stmt, NULL);
    rc_step = sqlite3_step(stmt);
    col_text = sqlite3_column_text(stmt, 1);
    id_from_row = sqlite3_column_int(stmt, 0);

    // step 4: Cleanup - finalize, free realloc buffer, close DB and tidy locals
    rc_finalize = sqlite3_finalize(stmt);
    stmt = NULL;
    sqlite3_realloc(tmp_buf, 0);
    rc_close = sqlite3_close_v2(db);
    db = NULL;

    (void)rc_open;
    (void)rc_exec;
    (void)rc_prepare;
    (void)rc_step;
    (void)rc_finalize;
    (void)rc_close;
    (void)rc_release;
    (void)set_id;
    (void)got_id;
    (void)txn_state;
    (void)tmp_buf;
    (void)col_text;
    (void)id_from_row;

    // API sequence test completed successfully
    return 66;
}