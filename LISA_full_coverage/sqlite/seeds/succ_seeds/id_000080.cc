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
//<ID> 80
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    // step 1: Declarations / Initialize
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt_create = NULL;
    sqlite3_stmt *stmt_insert = NULL;
    sqlite3_stmt *stmt_select = NULL;
    sqlite3_blob *blob = NULL;
    const char *pzTail = NULL;
    char *expanded_sql = NULL;
    int rc_open = 0;
    int rc_prep_create = 0;
    int rc_step_create = 0;
    int rc_prep_insert = 0;
    int rc_step_insert = 0;
    int rc_prep_select = 0;
    int rc_step_select = 0;
    int rc_blob_open = 0;
    int compileopt_used = 0;

    // step 2: Configure
    rc_open = sqlite3_open(":memory:", &db);
    compileopt_used = sqlite3_compileoption_used("ENABLE_COLUMN_METADATA");

    // step 3: Operate / Validate
    rc_prep_create = sqlite3_prepare_v2(db, "CREATE TABLE t(a);", -1, &stmt_create, &pzTail);
    rc_step_create = sqlite3_step(stmt_create);

    rc_prep_insert = sqlite3_prepare_v2(db, "INSERT INTO t VALUES(1);", -1, &stmt_insert, &pzTail);
    rc_step_insert = sqlite3_step(stmt_insert);

    rc_prep_select = sqlite3_prepare_v2(db, "SELECT a FROM t;", -1, &stmt_select, &pzTail);
    expanded_sql = sqlite3_expanded_sql(stmt_select);
    rc_step_select = sqlite3_step(stmt_select);

    rc_blob_open = sqlite3_blob_open(db, "main", "t", "a", 1, 0, &blob);
    sqlite3_blob_close(blob);

    // step 4: Cleanup
    sqlite3_close_v2(db);
    sqlite3_realloc(expanded_sql, 0);

    (void)rc_open;
    (void)rc_prep_create;
    (void)rc_step_create;
    (void)rc_prep_insert;
    (void)rc_step_insert;
    (void)rc_prep_select;
    (void)rc_step_select;
    (void)rc_blob_open;
    (void)compileopt_used;
    (void)stmt_create;
    (void)stmt_insert;
    (void)stmt_select;
    (void)pzTail;
    (void)blob;

    // API sequence test completed successfully
    return 66;
}