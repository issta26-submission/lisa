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
//<ID> 386
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    // step 1: Declarations & Initialize
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    sqlite3_str *pStr = NULL;
    const char *sql_create = "CREATE TABLE t(a INTEGER);";
    const char *sql_insert = "INSERT INTO t(a) VALUES(123);";
    const char *sql_select = "SELECT a FROM t;";
    int rc_open = 0;
    int rc_prep = 0;
    int rc_step = 0;
    int rc_finalize = 0;
    int rc_coll = 0;
    int colval = 0;
    int scan_out = 0;
    char *final_text = NULL;
    const char *collation_name = "my_collation";

    rc_open = sqlite3_open(":memory:", &db);

    // step 2: Configure - register a collation and prepare a string builder
    rc_coll = sqlite3_create_collation_v2(db, collation_name, 1, NULL, NULL, NULL);
    pStr = sqlite3_str_new(db);
    sqlite3_str_appendall(pStr, "scan-info:");

    // step 3: Operate & Validate - create table, insert a row, select and inspect
    rc_prep = sqlite3_prepare_v2(db, sql_create, -1, &stmt, NULL);
    rc_step = sqlite3_step(stmt);
    rc_finalize = sqlite3_finalize(stmt);
    stmt = NULL;

    rc_prep = sqlite3_prepare_v2(db, sql_insert, -1, &stmt, NULL);
    rc_step = sqlite3_step(stmt);
    rc_finalize = sqlite3_finalize(stmt);
    stmt = NULL;

    rc_prep = sqlite3_prepare_v2(db, sql_select, -1, &stmt, NULL);
    rc_step = sqlite3_step(stmt);
    colval = sqlite3_column_int(stmt, 0);
    sqlite3_stmt_scanstatus(stmt, 0, 0, (void *)&scan_out);

    // step 4: Finalize data flow & Cleanup
    {
        char buf[64];
        int n = snprintf(buf, sizeof(buf), " a=%d s=%d", colval, scan_out);
        (void)n;
        sqlite3_str_appendall(pStr, buf);
        final_text = sqlite3_str_finish(pStr);
        pStr = NULL;
    }
    rc_finalize = sqlite3_finalize(stmt);
    stmt = NULL;
    sqlite3_close(db);
    db = NULL;

    (void)rc_open;
    (void)rc_prep;
    (void)rc_step;
    (void)rc_finalize;
    (void)rc_coll;
    (void)final_text;
    // API sequence test completed successfully
    return 66;
}