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
//<ID> 411
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    // Step 1: Declarations
    sqlite3 *db = NULL;
    sqlite3_blob *blob = NULL;
    const void *filename = (const void *)L":memory:";
    char *errmsg = NULL;
    const char *sql_create = "CREATE TABLE t(id INTEGER PRIMARY KEY, val TEXT);";
    const char *sql_insert = "INSERT INTO t(val) VALUES('hello');";
    sqlite3_int64 changes = 0;
    int rc_open = 0;
    int rc_exec1 = 0;
    int rc_exec2 = 0;
    int rc_blob_close = 0;
    int rc_close = 0;
    int txn_state = 0;
    char scratch[32];
    memset(scratch, 0, sizeof(scratch));

    // step 2: Setup - open DB and create/insert
    rc_open = sqlite3_open16(filename, &db);
    rc_exec1 = sqlite3_exec(db, sql_create, NULL, NULL, &errmsg);
    rc_exec2 = sqlite3_exec(db, sql_insert, NULL, NULL, &errmsg);

    // step 3: Core operations - inspect changes, transaction state, and close blob handle
    changes = sqlite3_changes64(db);
    txn_state = sqlite3_txn_state(db, "main");
    rc_blob_close = sqlite3_blob_close(blob);

    // step 4: Cleanup - close DB and tidy up
    rc_close = sqlite3_close_v2(db);
    db = NULL;
    blob = NULL;
    memset(scratch, 0, sizeof(scratch));

    (void)rc_open;
    (void)rc_exec1;
    (void)rc_exec2;
    (void)changes;
    (void)txn_state;
    (void)rc_blob_close;
    (void)rc_close;
    (void)errmsg;
    (void)scratch;

    // API sequence test completed successfully
    return 66;
}