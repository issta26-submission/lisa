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
//<ID> 204
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    sqlite3 *db = nullptr;
    sqlite3_stmt *stmtCreate = nullptr;
    sqlite3_stmt *insStmt = nullptr;
    sqlite3_stmt *stmt16 = nullptr;
    int rc = 0;

    // Open an in-memory database. Use the default flags macro constants.
    rc = sqlite3_open_v2(":memory:", &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, nullptr);
    if (rc != SQLITE_OK || db == nullptr) {
        return 1;
    }

    // Prepare and execute a CREATE TABLE statement using prepare_v2.
    rc = sqlite3_prepare_v2(db, "CREATE TABLE t(x BLOB);", -1, &stmtCreate, nullptr);
    if (rc == SQLITE_OK && stmtCreate != nullptr) {
        // Execute the statement. Ignore the detailed result, but call step to run it.
        sqlite3_step(stmtCreate);
        // Note: sqlite3_finalize is not used here (not listed for mandatory use).
        // The statement object may be left for sqlite3_close_v2 to clean up later.
    }

    // Build an INSERT statement into a buffer using sqlite3_snprintf.
    char sqlBuf[128];
    // sqlite3_snprintf returns a pointer to the buffer, but we ignore that return here.
    sqlite3_snprintf((int)sizeof(sqlBuf), sqlBuf, "INSERT INTO t VALUES(?);");

    // Prepare the INSERT statement (with a parameter) using prepare_v2.
    rc = sqlite3_prepare_v2(db, sqlBuf, -1, &insStmt, nullptr);
    if (rc == SQLITE_OK && insStmt != nullptr) {
        // Bind a zero-filled BLOB of 10 bytes to parameter index 1.
        sqlite3_bind_zeroblob(insStmt, 1, 10);

        // Query some statement status counters for the prepared statement.
        // Use op=0 and resetFlg=0 as generic parameters (valid integers).
        (void)sqlite3_stmt_status(insStmt, 0, 0);

        // Execute the insert.
        sqlite3_step(insStmt);
    }

    // Prepare a UTF-16 query using sqlite3_prepare16.
    // Use a char16_t literal (UTF-16) and cast to const void* as required by the API.
    const char16_t select16[] = u"SELECT length(x) FROM t;";
    rc = sqlite3_prepare16(db, (const void *)select16, -1, &stmt16, nullptr);
    if (rc == SQLITE_OK && stmt16 != nullptr) {
        // Call stmt_status on the UTF-16 prepared statement as well.
        (void)sqlite3_stmt_status(stmt16, 0, 0);

        // Step once to advance the statement (will produce SQLITE_ROW for the select).
        sqlite3_step(stmt16);
    }

    // Close the database (requests a deferred close if statements still exist).
    sqlite3_close_v2(db);
    db = nullptr;

    return 66;
}