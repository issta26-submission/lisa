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
//<ID> 143
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

static void storeptr(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    /* Safely store the database handle as the function result using a real context. */
    sqlite3 *db = sqlite3_context_db_handle(ctx);
    sqlite3_result_pointer(ctx, (void *)db, "db_ptr", NULL);
}

int test_sqlite3_api_sequence() {
    /* step 1: Declarations / Initialize */
    sqlite3 *db = NULL;
    int rc_open = sqlite3_open(":memory:", &db);
    sqlite3_str *str = sqlite3_str_new(db);
    char *errmsg = NULL;
    const char *errtxt = NULL;
    int mem_before = sqlite3_db_release_memory(db);
    sqlite3_int64 changes_before = sqlite3_changes64(db);

    /* step 2: Configure - register function and create table and insert a row (callback will call sqlite3_result_pointer) */
    int rc_create = sqlite3_create_function(db, "storeptr", 0, 1, NULL, storeptr, NULL, NULL);
    const char *sql_setup = "CREATE TABLE t(id INTEGER PRIMARY KEY, val TEXT); INSERT INTO t(val) VALUES('hello'); SELECT storeptr();";
    int rc_exec = sqlite3_exec(db, sql_setup, NULL, NULL, &errmsg);

    /* step 3: Operate / Validate - reset string builder, get last error text, and ask for memory release */
    sqlite3_str_reset(str);
    errtxt = (const char *)sqlite3_errmsg16(db);
    int mem_after = sqlite3_db_release_memory(db);
    sqlite3_int64 changes_after = sqlite3_changes64(db);

    /* step 4: Cleanup */
    int rc_close = sqlite3_close(db);

    (void)rc_open;
    (void)str;
    (void)errmsg;
    (void)errtxt;
    (void)mem_before;
    (void)mem_after;
    (void)changes_before;
    (void)changes_after;
    (void)rc_exec;
    (void)rc_create;
    (void)rc_close;

    return 66;
}