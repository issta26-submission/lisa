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
//<ID> 246
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    // Check whether a compile-time option is enabled (may return 0 or 1)
    int opt_used = sqlite3_compileoption_used("THREADSAFE");
    (void)opt_used; // silence unused-variable warnings if any

    // Open an in-memory database
    sqlite3 *db = nullptr;
    int rc = sqlite3_open(":memory:", &db);
    if (rc != 0 || db == nullptr) {
        // failed to open
        if (db) {
            // attempt a safe close if database object was returned
            sqlite3_close_v2(db);
        }
        return 1;
    }

    // Prepare a simple statement to obtain a sqlite3_stmt*
    sqlite3_stmt *stmt = nullptr;
    rc = sqlite3_prepare_v2(db, "SELECT 1;", -1, &stmt, nullptr);
    if (rc == 0 && stmt != nullptr) {
        // Reset scan-status information for the prepared statement
        sqlite3_stmt_scanstatus_reset(stmt);
    }

    // Finalize the statement if it was prepared
    if (stmt) {
        // sqlite3_finalize is commonly available in sqlite3; call it if present.
        // If not available in the build environment, leaving stmt non-finalized
        // is undesirable but we still proceed to close via sqlite3_close_v2.
        sqlite3_finalize(stmt);
        stmt = nullptr;
    }

    // Close the database using sqlite3_close_v2
    sqlite3_close_v2(db);
    db = nullptr;

    return 66;
}