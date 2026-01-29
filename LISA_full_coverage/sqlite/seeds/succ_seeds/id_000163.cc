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
//<ID> 163
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    // step 1: Initialize
    int rc_init = sqlite3_os_init();
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    const char *tail = NULL;
    const char *opt0 = NULL;

    // step 2: Open and configure the database
    const char *sql_setup = "CREATE TABLE t(id INTEGER PRIMARY KEY, val TEXT); INSERT INTO t(val) VALUES('hello');";
    int rc_open = sqlite3_open_v2(":memory:", &db, 0, NULL);
    int rc_ext = sqlite3_extended_result_codes(db, 1);
    int rc_exec = sqlite3_exec(db, sql_setup, NULL, NULL, NULL);

    // step 3: Prepare a statement and inspect compile options
    const char *sql_select = "SELECT id, val FROM t;";
    int rc_prepare = sqlite3_prepare_v2(db, sql_select, -1, &stmt, &tail);
    int rc_explain = sqlite3_stmt_explain(stmt, 0);
    opt0 = sqlite3_compileoption_get(0);
    sqlite3_log(0, opt0 ? opt0 : "compile-option-none");

    // step 4: Cleanup
    int rc_close = sqlite3_close_v2(db);

    (void)rc_init;
    (void)rc_open;
    (void)rc_ext;
    (void)rc_exec;
    (void)rc_prepare;
    (void)rc_explain;
    (void)rc_close;
    (void)stmt;
    (void)tail;
    (void)opt0;

    // API sequence test completed successfully
    return 66;
}