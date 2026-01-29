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
//<ID> 225
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    // step 1: Declarations and open (Initialize & Configure)
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    sqlite3_value *colval = NULL;
    sqlite3_context *ctx = NULL;
    void *aux = NULL;
    int rc_open = sqlite3_open_v2(":memory:", &db, 0, NULL);
    int rc_prep = 0;
    int rc_step = 0;
    int val_type = 0;
    int rc_explain = 0;
    int rc_close = 0;

    // step 2: Operate - create table and insert a row
    rc_prep = sqlite3_prepare_v2(db, "CREATE TABLE IF NOT EXISTS t(x INTEGER);", -1, &stmt, NULL);
    rc_step = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    stmt = NULL;
    rc_prep = sqlite3_prepare_v2(db, "INSERT INTO t(x) VALUES(42);", -1, &stmt, NULL);
    rc_step = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    stmt = NULL;

    // step 3: Operate - select and inspect value, explain statement
    rc_prep = sqlite3_prepare_v2(db, "SELECT x FROM t LIMIT 1;", -1, &stmt, NULL);
    rc_step = sqlite3_step(stmt);
    colval = sqlite3_column_value(stmt, 0);
    val_type = sqlite3_value_type(colval);
    rc_explain = sqlite3_stmt_explain(stmt, 0);

    // step 4: Configure auxdata, cleanup and validate (Cleanup)
    aux = sqlite3_malloc(16);
    memset(aux, 0, 16);
    sqlite3_set_auxdata(ctx, 0, aux, sqlite3_free);
    sqlite3_finalize(stmt);
    stmt = NULL;
    sqlite3_free(aux);
    rc_close = sqlite3_close_v2(db);

    (void)rc_open;
    (void)rc_prep;
    (void)rc_step;
    (void)val_type;
    (void)rc_explain;
    (void)rc_close;
    (void)colval;
    (void)ctx;

    // API sequence test completed successfully
    return 66;
}