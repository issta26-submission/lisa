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
//<ID> 414
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    // step 1: Declarations
    sqlite3 *db = NULL;
    sqlite3_blob *blob = NULL;
    const void *filename = (const void *)L":memory:";
    const char *sql_create = "CREATE TABLE t(id INTEGER PRIMARY KEY, data BLOB);";
    const char *sql_insert = "INSERT INTO t(data) VALUES(x'0102');";
    const char *sql_select = "SELECT id, length(data) FROM t;";
    char *errmsg = NULL;
    sqlite3_int64 changes = 0;
    int rc_open = 0;
    int rc_exec1 = 0;
    int rc_exec2 = 0;
    int rc_exec3 = 0;
    int txn_state = 0;
    int rc_blob_close = 0;
    int rc_db_close = 0;

    // step 2: Setup - open DB and create/insert a row
    rc_open = sqlite3_open16(filename, &db);
    rc_exec1 = sqlite3_exec(db, sql_create, NULL, NULL, NULL);
    rc_exec2 = sqlite3_exec(db, sql_insert, NULL, NULL, NULL);

    // step 3: Core operations - inspect changes and transaction state, run a select
    changes = sqlite3_changes64(db);
    txn_state = sqlite3_txn_state(db, "main");
    rc_exec3 = sqlite3_exec(db, sql_select, NULL, NULL, NULL);

    // step 4: Cleanup - attempt to close blob handle (NULL) and close DB
    rc_blob_close = sqlite3_blob_close(blob);
    rc_db_close = sqlite3_close_v2(db);
    db = NULL;
    blob = NULL;

    (void)errmsg;
    (void)changes;
    (void)rc_open;
    (void)rc_exec1;
    (void)rc_exec2;
    (void)rc_exec3;
    (void)txn_state;
    (void)rc_blob_close;
    (void)rc_db_close;

    // API sequence test completed successfully
    return 66;
}