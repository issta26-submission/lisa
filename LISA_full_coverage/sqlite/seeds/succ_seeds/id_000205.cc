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
//<ID> 205
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

int test_sqlite3_api_sequence() {
    /* step 1: Declarations & Initialize */
    sqlite3 *db = NULL;
    const void *filename16 = (const void *)L":memory:";
    int rc_open = sqlite3_open16(filename16, &db);

    /* step 2: Configure */
    int rc_ext_codes = sqlite3_extended_result_codes(db, 1);

    /* step 3: Operate using APIs that execute SQL without explicit sqlite3_step */
    char **results = NULL;
    int rows = 0;
    int cols = 0;
    char *errmsg = NULL;
    const char *sql_create = "CREATE TABLE t(id INTEGER PRIMARY KEY, val INTEGER);";
    const char *sql_insert = "INSERT INTO t(val) VALUES(42);";
    int rc_get_create = sqlite3_get_table(db, sql_create, &results, &rows, &cols, &errmsg);
    int rc_get_insert = sqlite3_get_table(db, sql_insert, &results, &rows, &cols, &errmsg);

    /* query a total change count */
    int total_changes = sqlite3_total_changes(db);

    /* query a memory-related API to mimic release/inspection */
    sqlite3_int64 mem_used = sqlite3_memory_used();

    /* Ensure non-NULL, accessible storage for opaque types before calling functions
       that expect a non-NULL pointer. Allocate and zero-initialize to avoid
       assertions inside SQLite which assume a valid pointer. */
    sqlite3_value *val = (sqlite3_value *)malloc(512);
    if (val) memset(val, 0, 512);
    int v_int = sqlite3_value_int(val);

    sqlite3_mutex *mtx = (sqlite3_mutex *)malloc(128);
    if (mtx) memset(mtx, 0, 128);
    int mtx_is_held = sqlite3_mutex_held(mtx);

    /* step 4: Validate & Cleanup */
    const char *err_msg = sqlite3_errmsg(db);

    /* Free ephemeral resources. sqlite3_get_table may allocate errmsg and results.
       Attempt to free errmsg using sqlite3_free if non-NULL; otherwise fall back
       to free for the local allocations. Note: sqlite3_free is part of the
       public API in sqlite3.h. */
    if (errmsg) sqlite3_free(errmsg);
    /* sqlite3_get_table allocates results via sqlite3_malloc; attempt to free via
       sqlite3_free if results is non-NULL. Each row/column string is owned by results
       pointer returned. For fuzz-driver simplicity we free the top-level pointer. */
    if (results) sqlite3_free(results);

    /* Close DB if opened. sqlite3_close is part of the public API. */
    int rc_close = sqlite3_close(db);

    /* free our locally allocated opaque storage */
    free(val);
    free(mtx);

    /* Silence unused variable warnings while keeping the straight-line sequence */
    (void)rc_open;
    (void)rc_ext_codes;
    (void)rc_get_create;
    (void)rc_get_insert;
    (void)total_changes;
    (void)mem_used;
    (void)v_int;
    (void)mtx_is_held;
    (void)err_msg;
    (void)rc_close;
    (void)db;
    (void)rows;
    (void)cols;
    (void)sql_create;
    (void)sql_insert;
    (void)results;

    /* API sequence test completed successfully */
    return 66;
}