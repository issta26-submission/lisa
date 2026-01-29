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
//<ID> 260
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
    const unsigned char *col0 = NULL;
    const char *sqltext = NULL;
    char *buf = NULL;
    int rc_open = 0;
    int rc_prep = 0;
    int rc_step = 0;
    int rc_reset = 0;
    int rc_close = 0;
    int bufSize = 256;

    // step 2: Setup - open an in-memory database
    rc_open = sqlite3_open(":memory:", &db);

    // step 3: Core operations - prepare and step a simple SELECT, retrieve column and SQL, format into buffer
    rc_prep = sqlite3_prepare_v3(db, "SELECT 'fuzz_value' AS v, 1 AS n;", -1, 0, &stmt, NULL);
    rc_step = sqlite3_step(stmt);
    col0 = sqlite3_column_text(stmt, 0);
    sqltext = sqlite3_sql(stmt);
    buf = (char *)sqlite3_realloc64(NULL, (sqlite3_uint64)bufSize);
    sqlite3_snprintf(bufSize, buf, "SQL: %s; COL0: %s", sqltext, col0);
    sqlite3_free(buf);

    // step 4: Validate and Cleanup - reset statement and close database
    rc_reset = sqlite3_reset(stmt);
    rc_close = sqlite3_close_v2(db);
    db = NULL;
    stmt = NULL;

    (void)rc_open;
    (void)rc_prep;
    (void)rc_step;
    (void)rc_reset;
    (void)rc_close;
    (void)col0;
    (void)sqltext;

    // API sequence test completed successfully
    return 66;
}