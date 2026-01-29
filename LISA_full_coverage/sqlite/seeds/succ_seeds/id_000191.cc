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
//<ID> 191
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    // step 1: Declarations / Initialize
    sqlite3 *db = NULL;
    const void *filename = (const void *)u":memory:";
    const char *compile_opt0 = sqlite3_compileoption_get(0);
    int rc_open = sqlite3_open16(filename, &db);

    // step 2: Configure (replace unavailable calls with simple captures)
    int autocommit_before = 0;

    // step 3: Operate - create table
    sqlite3_stmt *stmt = NULL;
    const char *sql_create = "CREATE TABLE t(id INTEGER PRIMARY KEY, data BLOB);";
    int rc_prepare_create = sqlite3_prepare_v3(db, sql_create, -1, 0, &stmt, NULL);
    int rc_step_create = sqlite3_step(stmt);
    int rc_finalize_create = sqlite3_finalize(stmt);
    stmt = NULL;

    // step 3: Operate - insert blob using a parameter and sqlite3_bind_blob64
    const unsigned char blob_data[3] = {0xAA, 0xBB, 0xCC};
    const char *sql_insert = "INSERT INTO t(data) VALUES(?);";
    int rc_prepare_insert = sqlite3_prepare_v3(db, sql_insert, -1, 0, &stmt, NULL);
    int rc_bind = sqlite3_bind_blob64(stmt, 1, blob_data, (sqlite3_uint64)3, NULL);
    int rc_step_insert = sqlite3_step(stmt);
    int rc_finalize_insert = sqlite3_finalize(stmt);
    stmt = NULL;

    // step 3: Operate - select and read blob column
    const char *sql_select = "SELECT id, data FROM t;";
    int rc_prepare_select = sqlite3_prepare_v3(db, sql_select, -1, 0, &stmt, NULL);
    int rc_step_select = sqlite3_step(stmt);
    const void *col_blob = sqlite3_column_blob(stmt, 1);
    int col_bytes = sqlite3_column_bytes(stmt, 1);
    int rc_finalize_select = sqlite3_finalize(stmt);
    stmt = NULL;

    // step 4: Validate / Cleanup
    int autocommit_after = 0;
    int rc_close = sqlite3_close_v2(db);
    db = NULL;

    (void)compile_opt0;
    (void)rc_open;
    (void)autocommit_before;
    (void)rc_prepare_create;
    (void)rc_step_create;
    (void)rc_finalize_create;
    (void)blob_data;
    (void)rc_prepare_insert;
    (void)rc_bind;
    (void)rc_step_insert;
    (void)rc_finalize_insert;
    (void)rc_prepare_select;
    (void)rc_step_select;
    (void)col_blob;
    (void)col_bytes;
    (void)rc_finalize_select;
    (void)autocommit_after;
    (void)rc_close;
    (void)sql_create;
    (void)sql_insert;
    (void)sql_select;
    (void)rc_prepare_create;
    (void)rc_prepare_insert;
    (void)rc_prepare_select;

    // API sequence test completed successfully
    return 66;
}