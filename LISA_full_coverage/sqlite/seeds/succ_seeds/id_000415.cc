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
//<ID> 415
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
    char *errmsg = NULL;
    const void *filename = (const void *)L":memory:";
    const char *sql_create = "CREATE TABLE t(id INTEGER PRIMARY KEY, data BLOB);";
    const char *sql_insert = "INSERT INTO t(data) VALUES(x'010203');";
    const char *sql_delete = "DELETE FROM t;";
    int rc_open = 0;
    int rc_exec_create = 0;
    int rc_exec_insert = 0;
    int rc_exec_delete = 0;
    int rc_blob_close = 0;
    int rc_close = 0;
    sqlite3_int64 changes = 0;
    int txn_state = 0;
    char scratch[32];

    // step 2: Setup - open in-memory DB and create/insert a row
    rc_open = sqlite3_open16(filename, &db);
    rc_exec_create = sqlite3_exec(db, sql_create, NULL, NULL, &errmsg);
    rc_exec_insert = sqlite3_exec(db, sql_insert, NULL, NULL, &errmsg);

    // step 3: Core operations - inspect changes and transaction state, touch scratch buffer
    changes = sqlite3_changes64(db);
    txn_state = sqlite3_txn_state(db, "main");
    memset(scratch, 0, sizeof(scratch));

    // step 4: Cleanup - attempt to close blob handle (NULL safe usage), delete rows, close DB
    rc_blob_close = sqlite3_blob_close(blob);
    rc_exec_delete = sqlite3_exec(db, sql_delete, NULL, NULL, NULL);
    rc_close = sqlite3_close_v2(db);
    db = NULL;
    blob = NULL;

    (void)rc_open;
    (void)rc_exec_create;
    (void)rc_exec_insert;
    (void)rc_exec_delete;
    (void)rc_blob_close;
    (void)rc_close;
    (void)errmsg;
    (void)changes;
    (void)txn_state;
    (void)scratch;

    // API sequence test completed successfully
    return 66;
}