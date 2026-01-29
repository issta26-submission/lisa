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
//<ID> 227
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    sqlite3 *db = nullptr;
    sqlite3_stmt *stmt = nullptr;
    int rc = sqlite3_open(":memory:", &db);
    if (rc != 0 || db == nullptr) {
        return 1;
    }

    rc = sqlite3_prepare(db, "SELECT 1", -1, &stmt, nullptr);
    if (rc != 0 || stmt == nullptr) {
        sqlite3_close_v2(db);
        return 1;
    }

    /* Execute the prepared statement once. Accept SQLITE_ROW (100) or SQLITE_DONE (101). */
    int step_rc = sqlite3_step(stmt);
    if (step_rc != 100 && step_rc != 101) {
        sqlite3_finalize(stmt);
        sqlite3_close_v2(db);
        return 1;
    }

    /* Reset the statement and clear any bindings (no-op for this simple query). */
    rc = sqlite3_reset(stmt);
    if (rc != 0) {
        sqlite3_finalize(stmt);
        sqlite3_close_v2(db);
        return 1;
    }

    rc = sqlite3_clear_bindings(stmt);
    if (rc != 0) {
        sqlite3_finalize(stmt);
        sqlite3_close_v2(db);
        return 1;
    }

    /* Reset scan-status counters associated with the statement (no return value). */
    sqlite3_stmt_scanstatus_reset(stmt);

    /* Call sqlite3_keyword_name to exercise that API (index 0 chosen arbitrarily). */
    const char *kwname = nullptr;
    int kwlen = 0;
    int krc = sqlite3_keyword_name(0, &kwname, &kwlen);
    /* We do not require the keyword call to succeed for the overall test, but we invoked it. */

    /* Finalize and close. */
    rc = sqlite3_finalize(stmt);
    stmt = nullptr;
    if (rc != 0) {
        sqlite3_close_v2(db);
        return 1;
    }

    rc = sqlite3_close_v2(db);
    if (rc != 0) {
        return 1;
    }

    (void)krc; /* silence unused-variable warnings about krc if needed */
    (void)kwname;
    (void)kwlen;

    return 66;
}