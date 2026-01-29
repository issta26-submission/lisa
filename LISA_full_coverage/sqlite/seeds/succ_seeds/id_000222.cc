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
//<ID> 222
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    // step 1: Declarations and initialize
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    sqlite3_value *val = NULL;
    void *aux = NULL;
    const char *sql = "SELECT 1;";
    int rc_open = 0;
    int rc_prepare = 0;
    int rc_step = 0;
    int val_type = 0;
    int rc_explain = 0;
    int rc_reset = 0;
    int rc_close = 0;

    // step 2: Open an in-memory database (Initialize / Configure)
    rc_open = sqlite3_open_v2(":memory:", &db, 0, NULL);

    // step 3: Prepare, execute, inspect value type, set auxdata, and explain the statement (Operate)
    rc_prepare = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    rc_step = sqlite3_step(stmt);
    val = sqlite3_column_value(stmt, 0);
    val_type = sqlite3_value_type(val);
    aux = sqlite3_malloc(32);
    sqlite3_set_auxdata(NULL, 0, aux, sqlite3_free);
    rc_explain = sqlite3_stmt_explain(stmt, 0);

    // step 4: Validate and cleanup
    rc_reset = sqlite3_reset(stmt);
    sqlite3_free(aux);
    rc_close = sqlite3_close_v2(db);

    (void)rc_open;
    (void)rc_prepare;
    (void)rc_step;
    (void)val_type;
    (void)rc_explain;
    (void)rc_reset;
    (void)rc_close;
    (void)stmt;
    (void)val;
    // API sequence test completed successfully
    return 66;
}