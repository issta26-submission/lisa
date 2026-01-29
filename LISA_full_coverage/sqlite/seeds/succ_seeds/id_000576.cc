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
//<ID> 576
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    // step 1: Declarations / Initialize
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    const char *create_sql = "CREATE TABLE t(x INTEGER);";
    const char *insert_sql = "INSERT INTO t(x) VALUES(42);";
    const char *select_sql = "SELECT x FROM t;";
    int rc_open = sqlite3_open_v2(":memory:", &db, 0, NULL);
    int rc_p_create = 0;
    int rc_s_create = 0;
    int rc_f_create = 0;
    int rc_p_insert = 0;
    int rc_s_insert = 0;
    int rc_f_insert = 0;
    int rc_p_select = 0;
    int rc_s_select = 0;
    int rc_f_select = 0;
    sqlite3_int64 selected_value = 0;
    int err_code = 0;
    int glob_res = 0;
    unsigned char *serialized = NULL;
    sqlite3_int64 serialized_size = 0;

    // step 2: Setup / Configure (create table and insert a row)
    rc_p_create = sqlite3_prepare_v3(db, create_sql, -1, 0, &stmt, NULL);
    rc_s_create = sqlite3_step(stmt);
    rc_f_create = sqlite3_finalize(stmt);
    stmt = NULL;
    rc_p_insert = sqlite3_prepare_v3(db, insert_sql, -1, 0, &stmt, NULL);
    rc_s_insert = sqlite3_step(stmt);
    rc_f_insert = sqlite3_finalize(stmt);
    stmt = NULL;

    // step 3: Operate / Validate (select the inserted value, inspect error code, glob test, serialize attempt)
    rc_p_select = sqlite3_prepare_v3(db, select_sql, -1, 0, &stmt, NULL);
    rc_s_select = sqlite3_step(stmt);
    selected_value = sqlite3_column_int64(stmt, 0);
    rc_f_select = sqlite3_finalize(stmt);
    stmt = NULL;
    err_code = sqlite3_errcode(db);
    glob_res = sqlite3_strglob("CREATE*", create_sql);
    serialized = sqlite3_serialize(db, "nonexistent_schema_xyz", &serialized_size, 0);

    // step 4: Cleanup
    int rc_close = sqlite3_close_v2(db);
    (void)rc_open;
    (void)rc_p_create;
    (void)rc_s_create;
    (void)rc_f_create;
    (void)rc_p_insert;
    (void)rc_s_insert;
    (void)rc_f_insert;
    (void)rc_p_select;
    (void)rc_s_select;
    (void)rc_f_select;
    (void)selected_value;
    (void)err_code;
    (void)glob_res;
    (void)serialized;
    (void)serialized_size;
    (void)rc_close;

    // API sequence test completed successfully
    return 66;
}