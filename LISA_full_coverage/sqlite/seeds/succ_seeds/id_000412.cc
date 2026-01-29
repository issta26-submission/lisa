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
//<ID> 412
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
    int rc_open = 0;
    int rc_exec1 = 0;
    int rc_exec2 = 0;
    int rc_blob_close = 0;
    sqlite3_int64 changes = 0;
    int txn_state = 0;

    // step 2: Setup - open in-memory DB
    rc_open = sqlite3_open16(filename, &db);

    // step 3: Operate - create table, insert row, inspect changes and txn state, close any blob handle
    rc_exec1 = sqlite3_exec(db, sql_create, NULL, NULL, NULL);
    rc_exec2 = sqlite3_exec(db, sql_insert, NULL, NULL, NULL);
    changes = sqlite3_changes64(db);
    txn_state = sqlite3_txn_state(db, "main");
    rc_blob_close = sqlite3_blob_close(pBlob);

    // step 4: Cleanup - close database and tidy locals
    sqlite3_close_v2(db);
    db = NULL;
    pBlob = NULL;

    (void)rc_open;
    (void)rc_exec1;
    (void)rc_exec2;
    (void)rc_blob_close;
    (void)changes;
    (void)txn_state;
    (void)errmsg;
    // API sequence test completed successfully
    return 66;
}