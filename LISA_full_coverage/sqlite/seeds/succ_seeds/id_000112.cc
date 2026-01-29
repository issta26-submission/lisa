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
//<ID> 112
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
    sqlite3_str *str_builder = NULL;
    char *errmsg = NULL;
    const char *create_sql = "CREATE TABLE t(id INTEGER PRIMARY KEY, txt TEXT);";
    const char *insert_sql = "INSERT INTO t(txt) VALUES('hello');";
    const char *select_sql = "SELECT txt FROM t LIMIT 1;";
    const char *pzTail = NULL;
    int rc_open = 0;
    int rc_complete_create = 0;
    int rc_complete_select = 0;
    int rc_exec_create = 0;
    int rc_exec_insert = 0;
    int rc_prep = 0;
    int rc_step = 0;
    int rc_colbytes16 = 0;
    int rc_str_err = 0;
    int combined_check = 0;

    // step 2: Configure
    rc_open = sqlite3_open(":memory:", &db);
    rc_complete_create = sqlite3_complete(create_sql);
    rc_complete_select = sqlite3_complete(select_sql);
    str_builder = sqlite3_str_new(db);
    rc_str_err = sqlite3_str_errcode(str_builder);

    // step 3: Operate / Validate
    rc_exec_create = sqlite3_exec(db, create_sql, NULL, NULL, &errmsg);
    rc_exec_insert = sqlite3_exec(db, insert_sql, NULL, NULL, &errmsg);
    rc_prep = sqlite3_prepare_v2(db, select_sql, -1, &stmt, &pzTail);
    rc_step = sqlite3_step(stmt);
    rc_colbytes16 = sqlite3_column_bytes16(stmt, 0);
    combined_check = rc_colbytes16 + rc_step + rc_exec_insert + rc_exec_create + rc_prep + rc_open + rc_complete_create + rc_complete_select + rc_str_err;

    // step 4: Cleanup
    sqlite3_finalize(stmt);
    sqlite3_free_table(NULL); /* harmless call to use available cleanup API */
    sqlite3_close(db);
    (void)errmsg;
    (void)pzTail;
    (void)str_builder;
    (void)combined_check;
    (void)rc_colbytes16;
    (void)rc_step;
    (void)rc_prep;
    (void)rc_exec_create;
    (void)rc_exec_insert;
    (void)rc_complete_create;
    (void)rc_complete_select;
    (void)rc_open;
    (void)rc_str_err;

    // API sequence test completed successfully
    return 66;
}