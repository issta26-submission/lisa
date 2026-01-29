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
//<ID> 447
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    const char *sql_explain = "EXPLAIN SELECT 1;";
    int rc_open = 0;
    int rc_prep = 0;
    int rc_step = 0;
    const char *sql_text = NULL;
    char *expanded = NULL;
    const char *col0 = NULL;
    double val0 = 0.0;

    rc_open = sqlite3_open_v2(":memory:", &db, 0, NULL);
    rc_prep = sqlite3_prepare(db, sql_explain, -1, &stmt, NULL);
    expanded = sqlite3_expanded_sql(stmt);
    sql_text = sqlite3_sql(stmt);
    rc_step = sqlite3_step(stmt);
    col0 = sqlite3_column_name(stmt, 0);
    val0 = sqlite3_column_double(stmt, 0);
    sqlite3_finalize(stmt);
    sqlite3_free(expanded);

    (void)rc_open;
    (void)rc_prep;
    (void)rc_step;
    (void)sql_text;
    (void)col0;
    (void)val0;

    stmt = NULL;
    db = NULL;

    return 66;
}