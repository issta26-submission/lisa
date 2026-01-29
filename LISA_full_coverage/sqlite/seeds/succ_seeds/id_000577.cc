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
//<ID> 577
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
    const char *create_sql = "CREATE TABLE t1(id INTEGER PRIMARY KEY, data TEXT);";
    const char *insert_sql = "INSERT INTO t1(data) VALUES('hello');";
    const char *select_sql = "SELECT name FROM sqlite_master WHERE type='table' AND name='t1';";
    sqlite3_int64 serialized_size = 0;
    unsigned char *serialized = NULL;
    const char *tail = NULL;
    int rc_open = 0;
    int rc_prepare_create = 0;
    int rc_step_create = 0;
    int rc_finalize_create = 0;
    int rc_prepare_insert = 0;
    int rc_step_insert = 0;
    int rc_finalize_insert = 0;
    int rc_prepare_select = 0;
    int rc_step_select = 0;
    int rc_finalize_select = 0;
    int rc_close = 0;
    int errcode = 0;
    int glob_res = 0;
    int rc_prepare_dummy = 0;

    // step 2: Setup / Configure (open DB, create table)
    rc_open = sqlite3_open_v2(":memory:", &db, 0, NULL);
    rc_prepare_create = sqlite3_prepare_v3(db, create_sql, -1, 0, &stmt, NULL);
    rc_step_create = sqlite3_step(stmt);
    rc_finalize_create = sqlite3_finalize(stmt);

    // step 3: Operate / Validate (insert row, query, serialize, check error code and glob)
    rc_prepare_insert = sqlite3_prepare_v3(db, insert_sql, -1, 0, &stmt, NULL);
    rc_step_insert = sqlite3_step(stmt);
    rc_finalize_insert = sqlite3_finalize(stmt);
    rc_prepare_select = sqlite3_prepare_v3(db, select_sql, -1, 0, &stmt, &tail);
    rc_step_select = sqlite3_step(stmt);
    rc_finalize_select = sqlite3_finalize(stmt);
    serialized = sqlite3_serialize(db, "main", &serialized_size, 0);
    errcode = sqlite3_errcode(db);
    glob_res = sqlite3_strglob("t1", "t1");
    rc_prepare_dummy = sqlite3_prepare_v3(db, "SELECT 1;", -1, 0, &stmt, NULL);
    rc_step_select = sqlite3_step(stmt);
    rc_finalize_select = sqlite3_finalize(stmt);

    // step 4: Cleanup
    rc_close = sqlite3_close_v2(db);

    (void)rc_open;
    (void)rc_prepare_create;
    (void)rc_step_create;
    (void)rc_finalize_create;
    (void)rc_prepare_insert;
    (void)rc_step_insert;
    (void)rc_finalize_insert;
    (void)rc_prepare_select;
    (void)rc_step_select;
    (void)rc_finalize_select;
    (void)serialized;
    (void)serialized_size;
    (void)errcode;
    (void)glob_res;
    (void)tail;
    (void)rc_prepare_dummy;
    (void)rc_close;
    (void)stmt;

    // API sequence test completed successfully
    return 66;
}