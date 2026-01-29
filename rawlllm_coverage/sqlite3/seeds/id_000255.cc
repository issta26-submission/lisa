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
//<ID> 255
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    sqlite3 *db = nullptr;
    int rc = sqlite3_open(":memory:", &db);
    if (rc != 0 || db == nullptr) {
        if (db) sqlite3_close(db);
        return 1;
    }

    // Create a cancel function pointer (no-op) and cancel any auto-extension with it.
    // Use a lambda with unary + to get a plain function pointer.
    auto cancel_fn = +[]() {};
    sqlite3_cancel_auto_extension(cancel_fn);

    // Allocate a small chunk of memory using sqlite3_malloc64.
    void *buf = sqlite3_malloc64((sqlite3_uint64)128);
    (void)buf; // keep quiet if unused; it's just exercised.

    // Create a simple scalar function "my_null" that always returns NULL.
    auto myfunc = +[](sqlite3_context *ctx, int argc, sqlite3_value **argv) {
        (void)argc; (void)argv;
        sqlite3_result_null(ctx);
    };
    rc = sqlite3_create_function(db, "my_null", 0, 1, nullptr, myfunc, nullptr, nullptr);
    if (rc != 0) {
        sqlite3_close(db);
        return 2;
    }

    // Invoke the function via sqlite3_get_table.
    char **results = nullptr;
    int nrow = 0;
    int ncol = 0;
    char *errmsg = nullptr;
    rc = sqlite3_get_table(db, "SELECT my_null()", &results, &nrow, &ncol, &errmsg);
    // We won't inspect results deeply; just check that the call completed.
    if (rc != 0) {
        sqlite3_close(db);
        return 3;
    }

    // Best-effort cleanup: close the database. Note: sqlite3_get_table allocates memory
    // which in a full program should be freed with sqlite3_free_table, but that function
    // is not in the mandated API list for this test. We still close the DB.
    sqlite3_close(db);

    return 66;
}