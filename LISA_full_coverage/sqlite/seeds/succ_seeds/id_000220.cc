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
//<ID> 220
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    // step 1: Declarations and open database (Initialize)
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    const char *sql = "SELECT 42, 'abc';";
    int rc_open = sqlite3_open_v2(":memory:", &db, 0, NULL);

    // step 2: Prepare statement (Configure / Operate)
    int rc_prepare = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);

    // step 3: Execute, inspect value type, and set auxdata (Operate / Validate)
    int rc_step = sqlite3_step(stmt);
    sqlite3_value *col0 = sqlite3_column_value(stmt, 0);
    int col0_type = sqlite3_value_type(col0);
    sqlite3_set_auxdata((sqlite3_context *)NULL, 0, (void *)col0, (void (*)(void *))NULL);

    // step 4: Explain statement, cleanup and close (Validate / Cleanup)
    int rc_explain = sqlite3_stmt_explain(stmt, 0);
    sqlite3_reset(stmt);
    sqlite3_finalize(stmt);
    int rc_close = sqlite3_close_v2(db);

    (void)rc_open;
    (void)rc_prepare;
    (void)rc_step;
    (void)col0_type;
    (void)rc_explain;
    (void)rc_close;

    // API sequence test completed successfully
    return 66;
}