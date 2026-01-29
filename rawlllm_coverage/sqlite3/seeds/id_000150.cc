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
//<ID> 150
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    sqlite3 *db = nullptr;
    sqlite3_stmt *stmt = nullptr;
    sqlite3_stmt *ins = nullptr;
    sqlite3_blob *blob = nullptr;
    int rc = 0;

    // Open an in-memory database
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == nullptr) {
        return 1;
    }

    // Create a simple table with a BLOB column
    const char *create_sql = "CREATE TABLE t1(id INTEGER PRIMARY KEY, data BLOB);";
    rc = sqlite3_prepare_v3(db, create_sql, -1, 0, &stmt, NULL);
    if (rc != SQLITE_OK || stmt == nullptr) {
        sqlite3_close_v2(db);
        return 2;
    }
    // Execute the CREATE TABLE
    rc = sqlite3_step(stmt);
    // Clear bindings on stmt (no bindings used, but valid API usage)
    sqlite3_clear_bindings(stmt);
    // Not calling finalize here because only limited APIs are required/available in the provided list.
    stmt = nullptr; // drop local reference

    // Prepare an INSERT statement with a parameter and bind text
    rc = sqlite3_prepare_v3(db, "INSERT INTO t1(data) VALUES(?);", -1, 0, &ins, NULL);
    if (rc != SQLITE_OK || ins == nullptr) {
        sqlite3_close_v2(db);
        return 3;
    }
    // Bind a small text value as the blob content
    rc = sqlite3_bind_text(ins, 1, "hello", 5, NULL);
    // Execute the INSERT
    rc = sqlite3_step(ins);
    // Clear bindings after use
    sqlite3_clear_bindings(ins);
    // Drop local reference to the insert stmt
    ins = nullptr;

    // Prepare a SELECT to retrieve the column and to demonstrate column_table_name16 usage
    rc = sqlite3_prepare_v3(db, "SELECT data FROM t1 WHERE id=1;", -1, 0, &stmt, NULL);
    if (rc != SQLITE_OK || stmt == nullptr) {
        sqlite3_close_v2(db);
        return 4;
    }
    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        // Use sqlite3_column_table_name16 to query the origin table name for the first column
        const void *tblname16 = sqlite3_column_table_name16(stmt, 0);
        (void)tblname16; // silence unused-variable warning; demonstration only
    }
    // Clear bindings (even though none were bound) to exercise the API
    sqlite3_clear_bindings(stmt);

    // Open a blob handle to the row we just inserted (rowid = 1)
    rc = sqlite3_blob_open(db, "main", "t1", "data", 1, 1, &blob);
    if (rc != SQLITE_OK || blob == nullptr) {
        // still attempt snapshot recover and close before returning error
        sqlite3_snapshot_recover(db, "main");
        sqlite3_close_v2(db);
        return 5;
    }

    // Allocate a small buffer using sqlite3_malloc and write it into the blob
    int buflen = 6;
    void *buf = sqlite3_malloc(buflen);
    if (buf == nullptr) {
        // attempt cleanup
        sqlite3_snapshot_recover(db, "main");
        sqlite3_close_v2(db);
        return 6;
    }
    // Fill the buffer with some bytes and a null terminator
    unsigned char *bytes = (unsigned char *)buf;
    bytes[0] = 'A'; bytes[1] = 'B'; bytes[2] = 'C'; bytes[3] = 'D'; bytes[4] = '!'; bytes[5] = '\0';

    // Write into blob starting at offset 0
    rc = sqlite3_blob_write(blob, buf, buflen - 1, 0);
    (void)rc; // ignore return value here; demonstration of API usage

    // Invoke snapshot recover on the main database schema as part of the sequence
    sqlite3_snapshot_recover(db, "main");

    // Close the DB (sqlite3_close_v2 will clean up remaining resources)
    sqlite3_close_v2(db);

    // Return 66 to indicate success as required
    return 66;
}