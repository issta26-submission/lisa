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
//<ID> 116
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
    sqlite3_str *pstr = NULL;
    const char *sql_create = "CREATE TABLE t(id INTEGER PRIMARY KEY, txt TEXT);";
    const char *sql_insert = "INSERT INTO t(txt) VALUES('hello');";
    const char *sql_select = "SELECT txt FROM t LIMIT 1;";
    int rc_open = sqlite3_open(":memory:", &db);
    int rc_complete_create = sqlite3_complete(sql_create);
    int rc_prep_create = sqlite3_prepare_v2(db, sql_create, -1, &stmt, NULL);

    // step 2: Configure / Execute create
    int rc_step_create = sqlite3_step(stmt);
    int rc_finalize_create = sqlite3_finalize(stmt);
    stmt = NULL;

    // step 3: Operate / Insert / Select / Use required APIs
    int rc_complete_insert = sqlite3_complete(sql_insert);
    int rc_prep_insert = sqlite3_prepare_v2(db, sql_insert, -1, &stmt, NULL);
    int rc_step_insert = sqlite3_step(stmt);
    int rc_finalize_insert = sqlite3_finalize(stmt);
    stmt = NULL;

    int rc_complete_select = sqlite3_complete(sql_select);
    int rc_prep_select = sqlite3_prepare_v2(db, sql_select, -1, &stmt, NULL);
    int rc_step_select = sqlite3_step(stmt); /* required sqlite3_step usage */
    const void *col_text16 = sqlite3_column_text16(stmt, 0);
    int col_bytes16 = sqlite3_column_bytes16(stmt, 0); /* required sqlite3_column_bytes16 usage */
    pstr = sqlite3_str_new(db);
    int pstr_err = sqlite3_str_errcode(pstr); /* required sqlite3_str_errcode usage */

    // step 4: Cleanup / Finalize / Close
    int rc_finalize_select = sqlite3_finalize(stmt);
    stmt = NULL;
    int rc_close = sqlite3_close(db);
    (void)rc_open;
    (void)rc_complete_create;
    (void)rc_prep_create;
    (void)rc_step_create;
    (void)rc_finalize_create;
    (void)rc_complete_insert;
    (void)rc_prep_insert;
    (void)rc_step_insert;
    (void)rc_finalize_insert;
    (void)rc_complete_select;
    (void)rc_prep_select;
    (void)rc_step_select;
    (void)col_text16;
    (void)col_bytes16;
    (void)pstr;
    (void)pstr_err;
    (void)rc_finalize_select;
    (void)rc_close;

    // API sequence test completed successfully
    return 66;
}