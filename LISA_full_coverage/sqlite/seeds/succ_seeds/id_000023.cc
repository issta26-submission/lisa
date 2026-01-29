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
//<ID> 23
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    // step 1: Declarations / Initialize
    sqlite3 *db = NULL;
    sqlite3_blob *blob = NULL;
    int rc_open = 0;
    int rc_exec_create = 0;
    int rc_exec_insert = 0;
    int rc_blob_open = 0;
    int rc_blob_read = 0;
    int rc_close = 0;
    sqlite3_int64 rowid = 1;
    char sqlbuf[128];
    char blobbuf[64];
    int blob_bytes = 0;

    // step 2: Open in-memory database and create a table with a zeroblob row
    rc_open = sqlite3_open_v2(":memory:", &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL);
    rc_exec_create = sqlite3_exec(db, "CREATE TABLE t(x BLOB);", 0, 0, 0);
    rc_exec_insert = sqlite3_exec(db, "INSERT INTO t(x) VALUES (zeroblob(8));", 0, 0, 0);

    // step 3: Open a blob handle, read blob contents, format a message, sleep and interrupt
    rc_blob_open = sqlite3_blob_open(db, "main", "t", "x", rowid, 0, &blob);
    blob_bytes = sqlite3_blob_bytes(blob);
    rc_blob_read = sqlite3_blob_read(blob, blobbuf, blob_bytes < (int)sizeof(blobbuf) ? blob_bytes : (int)sizeof(blobbuf), 0);
    sqlite3_snprintf(sizeof(sqlbuf), sqlbuf, "blob_read_rc=%d bytes=%d", rc_blob_read, blob_bytes);
    sqlite3_sleep(1);
    sqlite3_interrupt(db);

    // step 4: Cleanup resources and finalize
    sqlite3_blob_close(blob);
    rc_close = sqlite3_close(db);
    memset(blobbuf, 0, sizeof(blobbuf));
    memset(sqlbuf, 0, sizeof(sqlbuf));
    (void)rc_open;
    (void)rc_exec_create;
    (void)rc_exec_insert;
    (void)rc_blob_open;
    (void)rc_blob_read;
    (void)rc_close;
    (void)blob_bytes;
    (void)rowid;
    (void)db;
    (void)blob;

    // API sequence test completed successfully
    return 66;
}