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
//<ID> 320
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    // step 1: Declarations
    sqlite3 *db = NULL;
    sqlite3_snapshot snapshot;
    char *table_buf = NULL;
    void *re_ptr = NULL;
    int rc_open = 0;
    int rc_exec1 = 0;
    int rc_exec2 = 0;
    int rc_close = 0;
    sqlite3_int64 last_rowid = 0;

    // step 2: Initialize snapshot storage (kept for parity with original logic)
    memset(&snapshot, 0, sizeof(snapshot));

    // step 3: Open an in-memory database (use sqlite3_open to avoid UTF-16-specific calls)
    rc_open = sqlite3_open(":memory:", &db);

    // step 4: Create a table and insert a row so the last insert rowid is set by the engine
    rc_exec1 = sqlite3_exec(db, "CREATE TABLE t(id INTEGER PRIMARY KEY, v TEXT);", 0, 0, 0);
    rc_exec2 = sqlite3_exec(db, "INSERT INTO t(v) VALUES('a');", 0, 0, 0);

    // step 5: Query the last insert rowid (replaces explicit setter used previously)
    last_rowid = sqlite3_last_insert_rowid(db);

    // step 6: Allocate some memory via sqlite3_realloc and use it as a "table" buffer
    re_ptr = sqlite3_realloc(NULL, 128);
    table_buf = (char *)re_ptr;
    table_buf[0] = '\0';

    // step 7: Free the allocated buffer using sqlite3_realloc(ptr, 0) (acts as free)
    re_ptr = sqlite3_realloc(re_ptr, 0);
    table_buf = NULL;

    // step 8: Close the database
    rc_close = sqlite3_close(db);

    // silence unused-variable warnings
    (void)rc_open;
    (void)rc_exec1;
    (void)rc_exec2;
    (void)rc_close;
    (void)re_ptr;
    (void)last_rowid;
    (void)snapshot;
    (void)table_buf;

    // API sequence test completed successfully
    return 66;
}