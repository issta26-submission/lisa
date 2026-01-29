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
//<ID> 579
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    // step 1: Declarations
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt_create = NULL;
    sqlite3_stmt *stmt_select = NULL;
    const char *tail = NULL;
    int rc_open = 0;
    int rc_prep_create = 0;
    int rc_step_create = 0;
    int rc_finalize_create = 0;
    int rc_prep_select = 0;
    int rc_step_select = 0;
    int rc_finalize_select = 0;
    int rc_close = 0;
    int errcode = 0;
    int glob_result = 0;
    unsigned char *serialized = NULL;
    sqlite3_int64 serialized_size = 0;

    // step 2: Setup / Configure
    rc_open = sqlite3_open_v2(":memory:", &db, 0, NULL);
    rc_prep_create = sqlite3_prepare_v3(db, "CREATE TABLE t(id INTEGER PRIMARY KEY, value TEXT);", -1, 0, &stmt_create, &tail);
    rc_step_create = sqlite3_step(stmt_create);
    rc_finalize_create = sqlite3_finalize(stmt_create);

    // step 3: Operate / Validate
    rc_prep_select = sqlite3_prepare_v3(db, "SELECT count(*) FROM t;", -1, 0, &stmt_select, NULL);
    rc_step_select = sqlite3_step(stmt_select);
    rc_finalize_select = sqlite3_finalize(stmt_select);
    errcode = sqlite3_errcode(db);
    glob_result = sqlite3_strglob("t*", "t");
    serialized = sqlite3_serialize(db, "nonexistent_schema", &serialized_size, 0);

    // step 4: Cleanup
    rc_close = sqlite3_close_v2(db);

    (void)rc_open;
    (void)rc_prep_create;
    (void)rc_step_create;
    (void)rc_finalize_create;
    (void)rc_prep_select;
    (void)rc_step_select;
    (void)rc_finalize_select;
    (void)rc_close;
    (void)errcode;
    (void)glob_result;
    (void)serialized;
    (void)serialized_size;
    (void)tail;
    (void)stmt_create;
    (void)stmt_select;
    (void)db;

    // API sequence test completed successfully
    return 66;
}