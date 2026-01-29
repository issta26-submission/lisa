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
//<ID> 269
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    // step 1: Declarations
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    int rc_open = 0;
    int rc_exec = 0;
    int rc_prep = 0;
    int rc_step = 0;
    int rc_close = 0;
    const unsigned char *col_text = NULL;
    char *buf = NULL;
    const char *stmtsql = NULL;

    // step 2: Setup - open an in-memory database and create a table with one row
    rc_open = sqlite3_open(":memory:", &db);
    rc_exec = sqlite3_exec(db, "CREATE TABLE t(x TEXT); INSERT INTO t(x) VALUES('hello');", NULL, NULL, NULL);

    // step 3: Core operations - prepare a SELECT, step to retrieve the row, read text, format into a buffer, get SQL text
    rc_prep = sqlite3_prepare_v3(db, "SELECT x FROM t;", -1, 0, &stmt, NULL);
    rc_step = sqlite3_step(stmt);
    col_text = sqlite3_column_text(stmt, 0);
    buf = (char *)sqlite3_realloc64(NULL, (sqlite3_uint64)128);
    sqlite3_snprintf(128, buf, "%s", (const char *)col_text);
    stmtsql = sqlite3_sql(stmt);

    // step 4: Validate and Cleanup - free allocated buffer, reset statement, clear bindings, and close database
    sqlite3_free(buf);
    sqlite3_reset(stmt);
    sqlite3_clear_bindings(stmt);
    rc_close = sqlite3_close_v2(db);

    (void)rc_open;
    (void)rc_exec;
    (void)rc_prep;
    (void)rc_step;
    (void)rc_close;
    (void)stmtsql;
    (void)col_text;

    // API sequence test completed successfully
    return 66;
}