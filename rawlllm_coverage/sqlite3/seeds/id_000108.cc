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
//<ID> 108
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    sqlite3 *db = nullptr;
    // Open an in-memory database
    int rc = sqlite3_open(":memory:", &db);

    // Prepare a CREATE TABLE using sqlite3_prepare_v2 (UTF-8)
    sqlite3_stmt *stmt = nullptr;
    const char *sql_create = "CREATE TABLE t(x INTEGER);";
    rc = sqlite3_prepare_v2(db, sql_create, -1, &stmt, nullptr);
    if (rc == 0 && stmt) {
        // Execute the CREATE TABLE
        sqlite3_step(stmt);
        sqlite3_finalize(stmt);
        stmt = nullptr;
    }

    // Prepare an INSERT using sqlite3_prepare16_v3 (UTF-16)
    const char16_t sql16[] = u"INSERT INTO t(x) VALUES (42);";
    rc = sqlite3_prepare16_v3(db, (const void *)sql16, -1, 0, &stmt, nullptr);
    if (rc == 0 && stmt) {
        // Execute the INSERT
        sqlite3_step(stmt);
        sqlite3_finalize(stmt);
        stmt = nullptr;
    }

    // Prepare and execute a SELECT to exercise sqlite3_step again
    const char *sql_select = "SELECT x FROM t;";
    rc = sqlite3_prepare_v2(db, sql_select, -1, &stmt, nullptr);
    if (rc == 0 && stmt) {
        // Step to read the row (if any)
        sqlite3_step(stmt);
        sqlite3_finalize(stmt);
        stmt = nullptr;
    }

    // Demonstrate sqlite3_drop_modules with a keep-list (null-terminated)
    const char *keep_list[] = { "mod_keep", nullptr };
    sqlite3_drop_modules(db, keep_list);

    // Call sqlite3_result_int (provide a null context pointer here
    // to show usage; in real extension code this would be a valid context)
    sqlite3_context *ctx = nullptr;
    sqlite3_result_int(ctx, 123);

    // Return success sentinel
    return 66;
}