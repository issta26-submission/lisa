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
//<ID> 115
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
    sqlite3_str *builder = NULL;
    const char *create_sql = "CREATE TABLE t(id INTEGER PRIMARY KEY, val TEXT);";
    const char *insert_sql = "INSERT INTO t(val) VALUES('hello');";
    const char *select_sql = "SELECT val FROM t LIMIT 1;";
    int rc_open = sqlite3_open(":memory:", &db);
    int complete_create = sqlite3_complete(create_sql);
    int rc_prepare = 0;
    int rc_step = 0;
    int rc_finalize = 0;
    int bytes16 = 0;
    int str_err = 0;

    // step 2: Configure (create table)
    rc_prepare = sqlite3_prepare_v2(db, create_sql, -1, &stmt, NULL);
    rc_step = sqlite3_step(stmt);
    rc_finalize = sqlite3_finalize(stmt);
    stmt = NULL;

    // step 3: Operate (insert and select; use required APIs)
    rc_prepare = sqlite3_prepare_v2(db, insert_sql, -1, &stmt, NULL);
    rc_step = sqlite3_step(stmt);
    rc_finalize = sqlite3_finalize(stmt);
    stmt = NULL;
    int complete_select = sqlite3_complete(select_sql);
    rc_prepare = sqlite3_prepare_v2(db, select_sql, -1, &stmt, NULL);
    rc_step = sqlite3_step(stmt);
    bytes16 = sqlite3_column_bytes16(stmt, 0);
    rc_finalize = sqlite3_finalize(stmt);
    stmt = NULL;

    // step 4: Validate / Cleanup
    builder = sqlite3_str_new(db);
    str_err = sqlite3_str_errcode(builder);
    int combined_check = rc_open + rc_prepare + rc_step + complete_create + complete_select + bytes16 + str_err + rc_finalize;
    (void)combined_check;
    int rc_close = sqlite3_close(db);
    (void)rc_open;
    (void)rc_prepare;
    (void)rc_step;
    (void)rc_finalize;
    (void)bytes16;
    (void)builder;
    (void)str_err;
    (void)rc_close;

    // API sequence test completed successfully
    return 66;
}