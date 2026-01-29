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
//<ID> 313
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    // step 1: Declarations
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt_create = NULL;
    sqlite3_stmt *stmt_insert = NULL;
    sqlite3_stmt *stmt_select = NULL;
    const void *sql_create = (const void *)L"CREATE TABLE t(a INTEGER, b TEXT);";
    const void *sql_insert = (const void *)L"INSERT INTO t(a,b) VALUES(1, 'x');";
    const void *sql_select = (const void *)L"SELECT a, b FROM t;";
    const void *pzTail = NULL;
    const void *col_origin = NULL;
    const char *uri_param = NULL;
    int rc_open = 0;
    int rc_prepare = 0;
    int rc_step = 0;
    int rc_reset = 0;
    int rc_finalize = 0;
    int rc_close = 0;
    int compile_opt_used = 0;

    // step 2: Setup - open in-memory DB, inspect a compile option and a URI parameter
    rc_open = sqlite3_open_v2(":memory:", &db, 0, NULL);
    compile_opt_used = sqlite3_compileoption_used("ENABLE_FTS5");
    uri_param = sqlite3_uri_parameter((sqlite3_filename)":memory:", "cache");

    // step 3: Operate - create table, insert a row, select and inspect column origin
    rc_prepare = sqlite3_prepare16_v2(db, sql_create, -1, &stmt_create, &pzTail);
    rc_step = sqlite3_step(stmt_create);
    rc_finalize = sqlite3_finalize(stmt_create);

    rc_prepare = sqlite3_prepare16_v2(db, sql_insert, -1, &stmt_insert, &pzTail);
    rc_step = sqlite3_step(stmt_insert);
    rc_finalize = sqlite3_finalize(stmt_insert);

    rc_prepare = sqlite3_prepare16_v2(db, sql_select, -1, &stmt_select, &pzTail);
    rc_step = sqlite3_step(stmt_select);
    col_origin = sqlite3_column_origin_name16(stmt_select, 0);
    rc_reset = sqlite3_reset(stmt_select);
    rc_finalize = sqlite3_finalize(stmt_select);

    // step 4: Cleanup - close database and silence unused-variable warnings
    rc_close = sqlite3_close(db);

    (void)rc_open;
    (void)rc_prepare;
    (void)rc_step;
    (void)rc_reset;
    (void)rc_finalize;
    (void)rc_close;
    (void)compile_opt_used;
    (void)uri_param;
    (void)col_origin;
    (void)pzTail;
    (void)sql_create;
    (void)sql_insert;
    (void)sql_select;

    // API sequence test completed successfully
    return 66;
}