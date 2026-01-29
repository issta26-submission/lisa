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
//<ID> 417
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    // step 1: Declarations
    sqlite3 *db = NULL;
    sqlite3_blob *pBlob = NULL;
    const void *filename = (const void *)L":memory:";
    const char *sql_create = "CREATE TABLE t(id INTEGER PRIMARY KEY, data BLOB);";
    const char *sql_insert = "INSERT INTO t(data) VALUES(x'010203');";
    char *errmsg = NULL;
    sqlite3_int64 changes = 0;
    int rc_open = 0;
    int rc_exec_create = 0;
    int rc_exec_insert = 0;
    int rc_blob_close = 0;
    int rc_close = 0;
    int txn_state = 0;

    // step 2: Setup - open an in-memory database and create/insert a row
    rc_open = sqlite3_open16(filename, &db);
    rc_exec_create = sqlite3_exec(db, sql_create, NULL, NULL, NULL);
    rc_exec_insert = sqlite3_exec(db, sql_insert, NULL, NULL, NULL);

    // step 3: Operate / Validate - inspect changes and transaction state, close any blob handle
    changes = sqlite3_changes64(db);
    txn_state = sqlite3_txn_state(db, "main");
    rc_blob_close = sqlite3_blob_close(pBlob);

    // step 4: Cleanup - close database and clear local references
    rc_close = sqlite3_close_v2(db);
    db = NULL;
    pBlob = NULL;
    errmsg = NULL;

    (void)rc_open;
    (void)rc_exec_create;
    (void)rc_exec_insert;
    (void)changes;
    (void)txn_state;
    (void)rc_blob_close;
    (void)rc_close;
    (void)errmsg;

    // API sequence test completed successfully
    return 66;
}