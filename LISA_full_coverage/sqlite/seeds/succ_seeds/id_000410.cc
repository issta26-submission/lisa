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
//<ID> 410
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
    const char *sql_create = "CREATE TABLE t(id INTEGER PRIMARY KEY, txt TEXT);";
    const char *sql_insert = "INSERT INTO t(txt) VALUES('one'),('two');";
    int rc_open = 0;
    int rc_exec_create = 0;
    int rc_exec_insert = 0;
    int rc_close = 0;
    int rc_busy_timeout = 0;
    int rc_blob_close = 0;
    sqlite3_int64 changes = 0;
    int txn_state = 0;

    // step 2: Setup - open in-memory DB and create/insert rows
    rc_open = sqlite3_open16(filename, &db);
    rc_exec_create = sqlite3_exec(db, sql_create, NULL, NULL, NULL);
    rc_exec_insert = sqlite3_exec(db, sql_insert, NULL, NULL, NULL);

    // step 3: Core operations / Validate - inspect changes and transaction state, configure busy timeout, close any blob handle
    changes = sqlite3_changes64(db);
    rc_busy_timeout = sqlite3_busy_timeout(db, (int)changes);
    txn_state = sqlite3_txn_state(db, "main");
    rc_blob_close = sqlite3_blob_close(blob);

    // step 4: Cleanup - close database and tidy local references
    rc_close = sqlite3_close_v2(db);
    db = NULL;
    blob = NULL;

    (void)rc_open;
    (void)rc_exec_create;
    (void)rc_exec_insert;
    (void)rc_busy_timeout;
    (void)rc_blob_close;
    (void)rc_close;
    (void)changes;
    (void)txn_state;

    // API sequence test completed successfully
    return 66;
}