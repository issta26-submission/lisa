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
//<ID> 266
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
    int rc_open = 0;
    int rc_prep = 0;
    int rc_step = 0;
    int rc_close = 0;
    const unsigned char *coltext = NULL;
    const char *stmt_sql = NULL;
    char *scratch = NULL;

    // step 2: Setup - open in-memory database and allocate a scratch buffer
    rc_open = sqlite3_open(":memory:", &db);
    scratch = (char *)sqlite3_realloc64(NULL, (sqlite3_uint64)256);
    memset(scratch, 0, 256);

    // step 3: Core operations - create table, insert a row, select the row, and capture text/SQL
    sqlite3_snprintf(256, scratch, "CREATE TABLE t(x TEXT);");
    rc_prep = sqlite3_prepare_v3(db, scratch, -1, 0, &stmt, &pzTail);
    rc_step = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    stmt = NULL;

    sqlite3_snprintf(256, scratch, "INSERT INTO t(x) VALUES('hello');");
    rc_prep = sqlite3_prepare_v3(db, scratch, -1, 0, &stmt, &pzTail);
    rc_step = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    stmt = NULL;

    sqlite3_snprintf(256, scratch, "SELECT x FROM t;");
    rc_prep = sqlite3_prepare_v3(db, scratch, -1, 0, &stmt, &pzTail);
    rc_step = sqlite3_step(stmt);
    coltext = sqlite3_column_text(stmt, 0);
    stmt_sql = sqlite3_sql(stmt);
    sqlite3_snprintf(256, scratch, "col=%s;sql=%s", (const char *)coltext, stmt_sql);

    // step 4: Validate and Cleanup - finalize statement, free buffers, and close database
    sqlite3_finalize(stmt);
    stmt = NULL;
    sqlite3_free(scratch);
    scratch = NULL;
    rc_close = sqlite3_close_v2(db);
    db = NULL;

    (void)pzTail;
    (void)rc_open;
    (void)rc_prep;
    (void)rc_step;
    (void)rc_close;
    (void)coltext;
    (void)stmt_sql;

    // API sequence test completed successfully
    return 66;
}