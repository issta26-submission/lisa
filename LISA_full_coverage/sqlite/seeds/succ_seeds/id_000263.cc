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
//<ID> 263
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
    const unsigned char *col_text = NULL;
    const char *stmt_sql = NULL;
    char sqlbuf[128];
    int rc_open = 0;
    int rc_prep = 0;
    int rc_step = 0;
    int rc_reset = 0;
    int rc_close = 0;

    // step 2: Setup - open an in-memory database
    rc_open = sqlite3_open(":memory:", &db);

    // step 3: Operate - prepare and execute a simple SELECT, read column and retrieve SQL text, format a buffer
    rc_prep = sqlite3_prepare_v3(db, "SELECT 'hello' AS col;", -1, 0U, &stmt, (const char **)NULL);
    rc_step = sqlite3_step(stmt);
    col_text = sqlite3_column_text(stmt, 0);
    stmt_sql = sqlite3_sql(stmt);
    sqlite3_snprintf((int)sizeof(sqlbuf), sqlbuf, stmt_sql);
    sqlite3_free((void *)NULL);

    // step 4: Validate and Cleanup - reset statement and close database (close_v2)
    rc_reset = sqlite3_reset(stmt);
    rc_close = sqlite3_close_v2(db);
    db = NULL;
    stmt = NULL;

    (void)col_text;
    (void)stmt_sql;
    (void)sqlbuf;
    (void)rc_open;
    (void)rc_prep;
    (void)rc_step;
    (void)rc_reset;
    (void)rc_close;

    // API sequence test completed successfully
    return 66;
}