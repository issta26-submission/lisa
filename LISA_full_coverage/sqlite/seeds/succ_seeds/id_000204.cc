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
//<ID> 204
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    /* step 1: Declarations & Initialize */
    sqlite3 *db = NULL;
    char *errmsg = NULL;
    const char *create_and_insert_sql = "CREATE TABLE t(id INTEGER PRIMARY KEY, val INTEGER); INSERT INTO t(val) VALUES(42);";
    int rc_open = sqlite3_open16((const void*)":memory:", &db);

    /* step 2: Execute DDL+DML using sqlite3_get_table (ignore results) */
    int rc_get1 = sqlite3_get_table(db, create_and_insert_sql, NULL, NULL, NULL, &errmsg);

    /* step 3: Query (use sqlite3_get_table to fetch the value) */
    char **results = NULL;
    int nrow = 0;
    int ncol = 0;
    int rc_get2 = sqlite3_get_table(db, "SELECT val FROM t WHERE id=1;", &results, &nrow, &ncol, &errmsg);
    /* result layout: first ncol entries are column names, data starts at index ncol */
    char *val_str = results ? results[ncol] : NULL;
    int col_val = 0;
    if (val_str) { col_val = (int)strtol(val_str, NULL, 10); } /* single-line conversion; no loops */

    /* step 4: Inspect changes, attempt to release memory, inspect changes again, format a message */
    int changes_before_release = sqlite3_changes(db);
    int mem_released = sqlite3_db_release_memory(db);
    int changes_after_release = sqlite3_changes(db);
    char *msg = sqlite3_mprintf("changes_before=%d after=%d", changes_before_release, changes_after_release);

    /* Cleanup */
    sqlite3_free(msg);
    sqlite3_free(errmsg);
    sqlite3_free(results);
    int rc_close = sqlite3_close_v2(db);

    (void)rc_open;
    (void)rc_get1;
    (void)rc_get2;
    (void)col_val;
    (void)changes_before_release;
    (void)mem_released;
    (void)changes_after_release;
    (void)rc_close;

    return 66;
}