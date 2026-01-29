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
//<ID> 224
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    // step 1: Declarations and initialization
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    sqlite3_value *val = NULL;
    void *aux = NULL;
    const char *sql = "SELECT 1";
    int rc_open = 0;
    int rc_prep = 0;
    int rc_step = 0;
    int vtype = 0;
    int rc_explain = 0;
    sqlite3_int64 changes = 0;
    int rc_close = 0;

    // step 2: Open and configure (open an in-memory database)
    rc_open = sqlite3_open_v2(":memory:", &db, 0, NULL);

    // step 3: Operate (prepare, step, inspect value type, explain statement, set auxdata)
    rc_prep = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    rc_step = sqlite3_step(stmt);
    val = sqlite3_column_value(stmt, 0);
    vtype = sqlite3_value_type(val);
    rc_explain = sqlite3_stmt_explain(stmt, 0);
    aux = sqlite3_malloc(32);
    memset(aux, 0, 32);
    sqlite3_set_auxdata((sqlite3_context *)0, 1, aux, sqlite3_free);

    // step 4: Validate and cleanup
    sqlite3_reset(stmt);
    changes = sqlite3_changes64(db);
    rc_close = sqlite3_close_v2(db);
    (void)rc_open;
    (void)rc_prep;
    (void)rc_step;
    (void)vtype;
    (void)rc_explain;
    (void)changes;
    (void)rc_close;

    // API sequence test completed successfully
    return 66;
}