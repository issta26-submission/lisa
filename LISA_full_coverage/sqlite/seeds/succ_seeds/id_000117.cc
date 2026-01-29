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
//<ID> 117
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
    sqlite3_str *str = NULL;
    const char *create_sql = "CREATE TABLE t(id INTEGER PRIMARY KEY, val TEXT);";
    const char *insert_sql = "INSERT INTO t(val) VALUES('α');";
    const char *select_sql = "SELECT val FROM t LIMIT 1;";
    int rc_open = sqlite3_open(":memory:", &db);
    int complete_create = sqlite3_complete(create_sql);
    int complete_select = sqlite3_complete(select_sql);

    // step 2: Configure / Execute statements
    int rc_prep_create = sqlite3_prepare_v2(db, create_sql, -1, &stmt, NULL);
    int rc_step_create = sqlite3_step(stmt);
    int rc_finalize_create = sqlite3_finalize(stmt);
    int rc_prep_insert = sqlite3_prepare_v2(db, insert_sql, -1, &stmt, NULL);
    int rc_step_insert = sqlite3_step(stmt);
    int rc_finalize_insert = sqlite3_finalize(stmt);
    int rc_prep_select = sqlite3_prepare_v2(db, select_sql, -1, &stmt, NULL);

    // step 3: Operate / Validate
    int rc_step_select = sqlite3_step(stmt);
    int bytes16 = sqlite3_column_bytes16(stmt, 0);
    str = sqlite3_str_new(db);
    int str_err = sqlite3_str_errcode(str);
    int db_errcode = sqlite3_errcode(db);

    // step 4: Cleanup
    int rc_finalize_select = sqlite3_finalize(stmt);
    int rc_close = sqlite3_close(db);
    (void)rc_open;
    (void)complete_create;
    (void)complete_select;
    (void)rc_prep_create;
    (void)rc_step_create;
    (void)rc_finalize_create;
    (void)rc_prep_insert;
    (void)rc_step_insert;
    (void)rc_finalize_insert;
    (void)rc_prep_select;
    (void)rc_step_select;
    (void)bytes16;
    (void)str;
    (void)str_err;
    (void)db_errcode;
    (void)rc_finalize_select;
    (void)rc_close;

    // API sequence test completed successfully
    return 66;
}