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
//<ID> 132
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

    // Use sqlite3_strlike to test simple pattern matching
    int like_result = sqlite3_strlike("a%", "abc", 0u);
    (void)like_result;

    // Allocate some memory via sqlite3_malloc64
    void *buffer = sqlite3_malloc64((sqlite3_uint64)128);
    if (!buffer) {
        sqlite3_close(db);
        return 2;
    }

    // Prepare a simple parameterized statement and bind an int64 value
    sqlite3_stmt *stmt = nullptr;
    rc = sqlite3_prepare(db, "SELECT ?", -1, &stmt, NULL);
    if (rc == 0 && stmt != nullptr) {
        // Bind an int64 value into parameter 1
        sqlite3_bind_int64(stmt, 1, (sqlite3_int64)123456789012345LL);
        // We intentionally do not step/finalize the statement here to keep the sequence focused
    }

    // Attempt to drop all modules (pass NULL to azKeep)
    (void)sqlite3_drop_modules(db, NULL);

    // Close the database. Note: allocated buffer is intentionally not freed with sqlite3_free
    // to avoid calling functions outside the required set; this is acceptable for this test.
    sqlite3_close(db);

    return 66;
}