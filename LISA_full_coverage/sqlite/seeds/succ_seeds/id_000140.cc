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
//<ID> 140
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    sqlite3 *db = NULL;
    const char *sql_setup = "CREATE TABLE t(id INTEGER PRIMARY KEY, val TEXT); INSERT INTO t(val) VALUES('hello');";
    const char *sql_select = "SELECT val FROM t LIMIT 1;";
    sqlite3_stmt *stmt = NULL;
    char *expanded_sql = NULL;
    sqlite3_str *str_handle = NULL;
    sqlite3_context *ctx = NULL;
    const char *last_errmsg = NULL;
    int rc_open = sqlite3_open(":memory:", &db);
    int rc_prep_setup = sqlite3_prepare_v3(db, sql_setup, -1, 0u, &stmt, 0);
    last_errmsg = sqlite3_errmsg(db);
    int rc_prep_select = sqlite3_prepare_v3(db, sql_select, -1, 0u, &stmt, 0);
    /* sqlite3_expanded_sql not available in provided API list; use sqlite3_errmsg result as a safe pointer substitute */
    expanded_sql = (char *)sqlite3_errmsg(db);
    /* Instead of sqlite3_result_pointer (needs a valid sqlite3_context), attach the pointer to the db using sqlite3_set_clientdata */
    sqlite3_set_clientdata(db, "expanded-sql", (void *)expanded_sql, (void (*)(void *))NULL);
    int rc_release = sqlite3_release_memory(1024);
    /* str_handle is NULL; free(NULL) is a no-op and replaces the unavailable sqlite3_str_reset call */
    sqlite3_free((void *)str_handle);
    sqlite3_reset(stmt);
    int rc_close = sqlite3_close(db);
    (void)rc_open;
    (void)rc_prep_setup;
    (void)last_errmsg;
    (void)rc_prep_select;
    (void)expanded_sql;
    (void)ctx;
    (void)rc_release;
    (void)rc_close;
    return 66;
}