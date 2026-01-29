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
//<ID> 119
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
    sqlite3_str *strbuf = NULL;
    const char *sql_create = "CREATE TABLE t(id INTEGER PRIMARY KEY, val TEXT);";
    const char *sql_insert = "INSERT INTO t(val) VALUES('hello');";
    const char *sql_select = "SELECT val FROM t LIMIT 1;";
    int rc_open = sqlite3_open(":memory:", &db);
    strbuf = sqlite3_str_new(db);

    // step 2: Configure / Prepare statements
    int rc_prep_create = sqlite3_prepare_v2(db, sql_create, -1, &stmt, NULL);
    int rc_step_create = sqlite3_step(stmt);
    int rc_reset_create = sqlite3_reset(stmt);
    int rc_prep_insert = sqlite3_prepare_v2(db, sql_insert, -1, &stmt, NULL);
    int rc_step_insert = sqlite3_step(stmt);
    int rc_reset_insert = sqlite3_reset(stmt);
    int rc_prep_select = sqlite3_prepare_v2(db, sql_select, -1, &stmt, NULL);

    // step 3: Operate / Validate
    int rc_step_select = sqlite3_step(stmt);
    int col_bytes16 = sqlite3_column_bytes16(stmt, 0);
    int complete_create = sqlite3_complete(sql_create);
    int complete_select = sqlite3_complete(sql_select);
    int str_errcode = sqlite3_str_errcode(strbuf);
    int combined_check = rc_open + rc_prep_create + rc_step_create + rc_reset_create + rc_prep_insert + rc_step_insert + rc_reset_insert + rc_prep_select + rc_step_select + col_bytes16 + complete_create + complete_select + str_errcode;
    (void)combined_check;

    // step 4: Cleanup
    int rc_reset_final = sqlite3_reset(stmt);
    int rc_finalize = sqlite3_finalize(stmt);
    int rc_close = sqlite3_close(db);
    (void)rc_open;
    (void)rc_prep_create;
    (void)rc_step_create;
    (void)rc_reset_create;
    (void)rc_prep_insert;
    (void)rc_step_insert;
    (void)rc_reset_insert;
    (void)rc_prep_select;
    (void)rc_step_select;
    (void)col_bytes16;
    (void)complete_create;
    (void)complete_select;
    (void)str_errcode;
    (void)strbuf;
    (void)rc_reset_final;
    (void)rc_finalize;
    (void)rc_close;
    // API sequence test completed successfully
    return 66;
}