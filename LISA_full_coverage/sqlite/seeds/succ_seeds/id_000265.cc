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
//<ID> 265
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
    const char *pzTail = NULL;
    const unsigned char *col_text = NULL;
    const char *stmt_sql = NULL;
    void *buf = NULL;
    int rc_open = 0;
    int rc_prep = 0;
    int rc_step = 0;
    int rc_close = 0;

    // step 2: Setup - open an in-memory database
    rc_open = sqlite3_open(":memory:", &db);

    // step 3: Core operations - create table, insert a row, select and read column text, copy SQL and column into a buffer
    rc_prep = sqlite3_prepare_v3(db, "CREATE TABLE IF NOT EXISTS t(x TEXT);", -1, 0, &stmt, &pzTail);
    rc_step = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    stmt = NULL;
    rc_prep = sqlite3_prepare_v3(db, "INSERT INTO t(x) VALUES('hello');", -1, 0, &stmt, &pzTail);
    rc_step = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    stmt = NULL;
    rc_prep = sqlite3_prepare_v3(db, "SELECT x FROM t;", -1, 0, &stmt, &pzTail);
    rc_step = sqlite3_step(stmt);
    col_text = sqlite3_column_text(stmt, 0);
    stmt_sql = sqlite3_sql(stmt);
    buf = sqlite3_realloc64(NULL, (sqlite3_uint64)256);
    sqlite3_snprintf(256, (char *)buf, "SQL: %s; Col: %s", stmt_sql ? stmt_sql : "(null)", col_text ? (const char *)col_text : "(null)");

    // step 4: Cleanup - free buffer, finalize statement, and close database
    sqlite3_free(buf);
    sqlite3_finalize(stmt);
    stmt = NULL;
    rc_close = sqlite3_close_v2(db);
    db = NULL;

    (void)rc_open;
    (void)rc_prep;
    (void)rc_step;
    (void)rc_close;
    (void)pzTail;
    (void)col_text;
    (void)stmt_sql;

    // API sequence test completed successfully
    return 66;
}