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
//<ID> 144
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    // Open an in-memory database (use sqlite3_open_v2 from the provided API list)
    sqlite3 *db = nullptr;
    // Use 0 for flags to avoid relying on SQLITE_OPEN_* macros here.
    int rc = sqlite3_open_v2(":memory:", &db, 0, nullptr);

    // Prepare a non-capturing lambda and assign to the required callback type for sqlite3_collation_needed
    void (*collation_cb)(void *, sqlite3 *, int, const char *) =
        [](void *pArg, sqlite3 *pDb, int eTextRep, const char *zName)
        {
            // simple no-op callback; silence unused parameter warnings
            (void)pArg; (void)pDb; (void)eTextRep; (void)zName;
        };

    // If the DB opened, register the collation-needed callback (this demonstrates using sqlite3_collation_needed)
    if (db != nullptr) {
        sqlite3_collation_needed(db, nullptr, collation_cb);
    }

    // Create a no-op auto-extension entrypoint and cancel it (demonstrates sqlite3_cancel_auto_extension)
    void (*auto_ext_entry)() = []() { /* no-op */ };
    sqlite3_cancel_auto_extension(auto_ext_entry);

    // Prepare a simple statement and then reset it to demonstrate sqlite3_prepare_v3 + sqlite3_reset usage
    sqlite3_stmt *stmt = nullptr;
    if (db != nullptr) {
        // Prepare a trivial statement. Use -1 for nByte so the whole string is used.
        sqlite3_prepare_v3(db, "SELECT 1;", -1, 0u, &stmt, nullptr);
        if (stmt != nullptr) {
            // Reset the prepared statement (demonstrates sqlite3_reset)
            sqlite3_reset(stmt);
            // Note: sqlite3_finalize is not listed in the provided API set, so we do not call it here.
        }
    }

    // Demonstrate usage of sqlite3_value_double. We don't have a readily-constructible sqlite3_value*,
    // so show a guarded use pattern: attempt to duplicate an existing value if available, then call sqlite3_value_double.
    sqlite3_value *val = nullptr;
    sqlite3_value *dupval = nullptr;
    if (val != nullptr) {
        // If we had a real value, duplicate it (sqlite3_value_dup is in the API list)
        dupval = sqlite3_value_dup(val);
    }

    double d = 0.0;
    if (dupval != nullptr) {
        // Safely call sqlite3_value_double on a valid sqlite3_value*
        d = sqlite3_value_double(dupval);
        // Free the duplicated value (sqlite3_value_free is in the API list)
        sqlite3_value_free(dupval);
    } else {
        // No value available; set a sensible default
        d = 0.0;
    }

    // Close the database if it was opened
    if (db != nullptr) {
        sqlite3_close_v2(db);
        db = nullptr;
    }

    // Use the computed double in a no-op way to avoid unused-variable warnings
    (void)d;
    (void)rc;
    (void)stmt;

    return 66;
}