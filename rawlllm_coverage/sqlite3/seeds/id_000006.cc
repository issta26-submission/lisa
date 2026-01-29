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
//<ID> 6
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    // Forward-declare a few commonly used SQLite APIs that may not have
    // been listed explicitly in the prompt but are part of the sqlite3 API.
    // These declarations allow calls below without additional headers here.
    extern int sqlite3_step(struct sqlite3_stmt*);
    extern int sqlite3_finalize(struct sqlite3_stmt*);
    extern int sqlite3_close(struct sqlite3*);
    // Open an in-memory database.
    sqlite3 *db = nullptr;
    int rc = sqlite3_open_v2(":memory:", &db, 0, nullptr);
    if (rc != 0 || db == nullptr) {
        // If open failed, try to clean up and still return the required value.
        if (db) sqlite3_close(db);
        return 66;
    }

    // Track whether the collation-needed callback was invoked.
    int collation_needed_called = 0;
    // Non-capturing lambda convertible to function pointer for the collation callback.
    auto collation_cb = +[](void *pArg, sqlite3 *pDb, int enc, const void *zName) {
        int *flag = static_cast<int*>(pArg);
        if (flag) *flag = 1;
        // Do not perform other DB operations here; keep callback minimal.
    };
    // Register the collation-needed callback (16-bit name variant).
    sqlite3_collation_needed16(db, &collation_needed_called, collation_cb);

    // Find the default VFS and attempt to unregister it (best-effort).
    sqlite3_vfs *vfs = sqlite3_vfs_find(nullptr);
    if (vfs) {
        // sqlite3_vfs_unregister returns an int status; ignore result (best-effort).
        sqlite3_vfs_unregister(vfs);
    }

    // Define a scalar SQL function "myfunc" that multiplies its first numeric
    // argument by 2.0 and returns the result. The function uses sqlite3_value_double.
    auto myfunc = +[](sqlite3_context *ctx, int argc, sqlite3_value **argv) {
        double d = 0.0;
        if (argc > 0 && argv && argv[0]) {
            // Use the required API to obtain a double from the first argument.
            d = sqlite3_value_double(argv[0]);
        }
        // Return the computed double result.
        sqlite3_result_double(ctx, d * 2.0);
    };
    // Register the function with the database. Use a simple eTextRep value of 1.
    sqlite3_create_function(db, "myfunc", 1, 1, nullptr, myfunc, nullptr, nullptr);

    // Prepare a statement that calls our function. This uses the required API.
    sqlite3_stmt *stmt = nullptr;
    rc = sqlite3_prepare_v2(db, "SELECT myfunc(3.14);", -1, &stmt, nullptr);
    if (rc == 0 && stmt != nullptr) {
        // Execute the statement to trigger the custom function (best-effort).
        // sqlite3_step will cause our myfunc to be invoked, which calls
        // sqlite3_value_double internally.
        (void)sqlite3_step(stmt);
        // Finalize the prepared statement to release resources.
        sqlite3_finalize(stmt);
    }

    // Close the database connection.
    sqlite3_close(db);

    // Successful completion as requested by the problem statement.
    return 66;
}