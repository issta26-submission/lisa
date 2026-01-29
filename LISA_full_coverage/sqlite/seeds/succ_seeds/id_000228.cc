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
//<ID> 228
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
    const char *sql = "SELECT 1;";
    sqlite3_value *val = NULL;
    int rc_open = 0;
    int rc_prep = 0;
    int rc_step = 0;
    int vtype = 0;
    void *aux = NULL;
    int rc_explain = 0;
    int rc_reset = 0;
    int rc_close = 0;

    // step 2: Open database and prepare statement
    rc_open = sqlite3_open_v2(":memory:", &db, 2, NULL);
    rc_prep = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);

    // step 3: Execute to obtain a column value, inspect its type, attach aux data, and explain stmt
    rc_step = sqlite3_step(stmt);
    val = sqlite3_column_value(stmt, 0);
    vtype = sqlite3_value_type(val);
    aux = sqlite3_malloc(16);
    memset(aux, 0, 16);
    sqlite3_set_auxdata(NULL, 0, aux, sqlite3_free);
    rc_explain = sqlite3_stmt_explain(stmt, 0);

    // step 4: Cleanup and validation
    rc_reset = sqlite3_reset(stmt);
    sqlite3_finalize(stmt);
    rc_close = sqlite3_close_v2(db);

    (void)rc_open;
    (void)rc_prep;
    (void)rc_step;
    (void)vtype;
    (void)rc_explain;
    (void)rc_reset;
    (void)rc_close;

    // API sequence test completed successfully
    return 66;
}