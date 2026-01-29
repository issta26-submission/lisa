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
//<ID> 114
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
    const char *pzTail = NULL;
    const char *sql_create = "CREATE TABLE t(id INTEGER PRIMARY KEY, val TEXT);";
    const char *sql_insert = "INSERT INTO t(val) VALUES('hello');";
    const char *sql_select = "SELECT val FROM t LIMIT 1;";
    int rc_open = sqlite3_open(":memory:", &db);
    int rc_prep_create = sqlite3_prepare_v2(db, sql_create, -1, &stmt, &pzTail);
    int rc_step_create = sqlite3_step(stmt);
    int rc_finalize_create = sqlite3_finalize(stmt);

    // step 2: Configure / Insert
    int rc_prep_insert = sqlite3_prepare_v2(db, sql_insert, -1, &stmt, &pzTail);
    int rc_step_insert = sqlite3_step(stmt);
    int rc_finalize_insert = sqlite3_finalize(stmt);

    // step 3: Operate / Select / Inspect
    int rc_prep_select = sqlite3_prepare_v2(db, sql_select, -1, &stmt, &pzTail);
    int rc_step_select = sqlite3_step(stmt);
    int col_bytes16 = sqlite3_column_bytes16(stmt, 0);
    strbuf = sqlite3_str_new(db);
    int str_err = sqlite3_str_errcode(strbuf);
    int complete_flag = sqlite3_complete(sql_select);

    // step 4: Cleanup / Finalize
    int rc_finalize_select = sqlite3_finalize(stmt);
    int rc_close = sqlite3_close(db);
    (void)rc_open;
    (void)rc_prep_create;
    (void)rc_step_create;
    (void)rc_finalize_create;
    (void)rc_prep_insert;
    (void)rc_step_insert;
    (void)rc_finalize_insert;
    (void)rc_prep_select;
    (void)rc_step_select;
    (void)col_bytes16;
    (void)strbuf;
    (void)str_err;
    (void)complete_flag;
    (void)rc_finalize_select;
    (void)rc_close;
    (void)pzTail;
    // API sequence test completed successfully
    return 66;
}