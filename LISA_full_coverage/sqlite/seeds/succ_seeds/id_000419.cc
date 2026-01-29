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
//<ID> 419
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    // step 1: Declarations
    sqlite3 *db = NULL;
    const void *filename = (const void *)L":memory:";
    const char *sql_create = "CREATE TABLE t(id INTEGER PRIMARY KEY, data BLOB);";
    const char *sql_insert = "INSERT INTO t(data) VALUES(x'0102');";
    char *errmsg = NULL;
    char *delete_sql = NULL;
    sqlite3_blob *blob = NULL;
    int rc_open = 0;
    int rc_exec_create = 0;
    int rc_exec_insert = 0;
    int rc_exec_delete = 0;
    int rc_blob_close = 0;
    sqlite3_int64 changes_after = 0;
    int txn_state = 0;

    // step 2: Setup - open DB and create/insert a row
    rc_open = sqlite3_open16(filename, &db);
    rc_exec_create = sqlite3_exec(db, sql_create, NULL, NULL, &errmsg);
    rc_exec_insert = sqlite3_exec(db, sql_insert, NULL, NULL, &errmsg);

    // step 3: Core operations / Validate - inspect changes and transaction state, build SQL using changes
    changes_after = sqlite3_changes64(db);
    txn_state = sqlite3_txn_state(db, "main");
    delete_sql = sqlite3_mprintf("DELETE FROM t WHERE id > %lld;", (long long)changes_after);
    rc_exec_delete = sqlite3_exec(db, delete_sql, NULL, NULL, &errmsg);
    rc_blob_close = sqlite3_blob_close(blob);

    // step 4: Cleanup - close DB and tidy local references
    sqlite3_close_v2(db);
    db = NULL;
    blob = NULL;
    delete_sql = NULL;
    errmsg = NULL;

    (void)rc_open;
    (void)rc_exec_create;
    (void)rc_exec_insert;
    (void)rc_exec_delete;
    (void)rc_blob_close;
    (void)changes_after;
    (void)txn_state;
    (void)delete_sql;
    (void)errmsg;

    // API sequence test completed successfully
    return 66;
}