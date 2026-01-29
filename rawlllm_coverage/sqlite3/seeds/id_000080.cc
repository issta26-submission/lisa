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
//<ID> 80
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    sqlite3 *db = nullptr;
    // Open an in-memory database
    sqlite3_open(":memory:", &db);

    // Obtain the database mutex (may be NULL if open failed)
    sqlite3_mutex *db_mtx = nullptr;
    if (db) {
        db_mtx = sqlite3_db_mutex(db);
        (void)db_mtx; // silence unused-variable warnings in some builds
    }

    // Create a UTF-16 collation named "mycoll" using a local, non-capturing lambda
    int (*collate_fn)(void *, int, const void *, int, const void *);
    collate_fn = [](void * /*pArg*/, int nA, const void *a, int nB, const void *b) -> int {
        if (a == b && nA == nB) return 0;
        const unsigned char *pa = (const unsigned char*)a;
        const unsigned char *pb = (const unsigned char*)b;
        int nMin = nA < nB ? nA : nB;
        for (int i = 0; i < nMin; ++i) {
            if (pa[i] != pb[i]) return (int)pa[i] - (int)pb[i];
        }
        return nA - nB;
    };

    // Register the collation as UTF-16 (pass a UTF-16 string literal)
    sqlite3_create_collation16(db, (const void*)u"mycoll", 0, nullptr, collate_fn);

    // Prepare a simple UTF-16 SQL statement
    sqlite3_stmt *stmt = nullptr;
    int rc = sqlite3_prepare16_v2(db, (const void*)u"SELECT 1", -1, &stmt, nullptr);

    if (rc == 0 && stmt) {
        // Execute the statement once
        sqlite3_step(stmt);

        // Retrieve the first column value of the current row (if any)
        sqlite3_value *val = sqlite3_column_value(stmt, 0);
        (void)val; // we don't inspect it further in this test sequence

        // Demonstrate result_text usage with a (null) context and a simple C string
        sqlite3_context *ctx = nullptr;
        sqlite3_result_text(ctx, "example", -1, nullptr);

        // Reset the statement (we do not finalize here; just reset)
        sqlite3_reset(stmt);
    }

    // Close the database if opened
    if (db) {
        sqlite3_close(db);
    }

    return 66;
}