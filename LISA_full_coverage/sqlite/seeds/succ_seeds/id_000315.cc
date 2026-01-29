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
//<ID> 315
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
    const void *pzTail = NULL;
    const void *sql_create16 = (const void *)L"CREATE TABLE t1(id INTEGER);";
    const void *sql_insert16 = (const void *)L"INSERT INTO t1(id) VALUES(1);";
    const void *sql_select16 = (const void *)L"SELECT id FROM t1;";
    const void *col_origin16 = NULL;
    const char *uri = "file::memory:?cache=shared";
    const char *uri_cache_value = NULL;
    int rc_open = 0;
    int rc_prep = 0;
    int rc_step = 0;
    int rc_reset = 0;
    int opt_used = 0;

    // step 2: Initialize / Configure
    rc_open = sqlite3_open_v2(":memory:", &db, 0, NULL);
    opt_used = sqlite3_compileoption_used("ENABLE_FTS3");
    uri_cache_value = sqlite3_uri_parameter((sqlite3_filename)uri, "cache");

    // step 3: Operate / Validate
    rc_prep = sqlite3_prepare16_v2(db, sql_create16, -1, &stmt_create, &pzTail);
    rc_step = sqlite3_step(stmt_create);
    rc_reset = sqlite3_reset(stmt_create);

    rc_prep = sqlite3_prepare16_v2(db, sql_insert16, -1, &stmt_insert, &pzTail);
    rc_step = sqlite3_step(stmt_insert);
    rc_reset = sqlite3_reset(stmt_insert);

    rc_prep = sqlite3_prepare16_v2(db, sql_select16, -1, &stmt_select, &pzTail);
    rc_step = sqlite3_step(stmt_select);
    col_origin16 = sqlite3_column_origin_name16(stmt_select, 0);

    // step 4: Cleanup
    rc_reset = sqlite3_reset(stmt_select);
    rc_reset = sqlite3_reset(stmt_insert);
    rc_reset = sqlite3_reset(stmt_create);
    sqlite3_close(db);

    (void)rc_open;
    (void)rc_prep;
    (void)rc_step;
    (void)rc_reset;
    (void)opt_used;
    (void)uri_cache_value;
    (void)col_origin16;
    (void)pzTail;
    (void)sql_create16;
    (void)sql_insert16;
    (void)sql_select16;

    // API sequence test completed successfully
    return 66;
}