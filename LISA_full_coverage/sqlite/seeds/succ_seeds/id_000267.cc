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
//<ID> 267
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    // step 1: Declarations and initialization
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    char sql_buf[256];
    char *errmsg = NULL;
    const unsigned char *col_text = NULL;
    const char *stmt_sql = NULL;
    int rc_open = 0;
    int rc_exec = 0;
    int rc_prep = 0;
    int rc_step = 0;
    int rc_err = 0;
    int rc_close = 0;

    // step 2: Setup - open an in-memory database and create a table with one row
    rc_open = sqlite3_open(":memory:", &db);
    rc_exec = sqlite3_exec(db, "CREATE TABLE t(x TEXT); INSERT INTO t(x) VALUES('hello');", NULL, NULL, &errmsg);
    sqlite3_free(errmsg);
    errmsg = NULL;

    // step 3: Core operations - prepare and step a SELECT, read column text and statement SQL
    rc_prep = sqlite3_prepare_v3(db, "SELECT x FROM t;", -1, 0, &stmt, NULL);
    rc_step = sqlite3_step(stmt);
    col_text = sqlite3_column_text(stmt, 0);
    stmt_sql = sqlite3_sql(stmt);

    // step 4: Validate and Cleanup - format a summary into a buffer, finalize statement, provoke an error to obtain an errmsg and free it, then close the DB
    sqlite3_snprintf(sizeof(sql_buf), sql_buf, "stmt_sql=%s; col_text=%s", stmt_sql, (const char*)col_text);
    sqlite3_finalize(stmt);
    stmt = NULL;
    rc_err = sqlite3_exec(db, "THIS IS INVALID SQL;", NULL, NULL, &errmsg);
    sqlite3_free(errmsg);
    errmsg = NULL;
    rc_close = sqlite3_close_v2(db);
    db = NULL;

    (void)sql_buf;
    (void)rc_open;
    (void)rc_exec;
    (void)rc_prep;
    (void)rc_step;
    (void)rc_err;
    (void)rc_close;
    (void)col_text;
    (void)stmt_sql;

    // API sequence test completed successfully
    return 66;
}