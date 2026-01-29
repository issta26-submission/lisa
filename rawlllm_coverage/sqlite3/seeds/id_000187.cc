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
//<ID> 187
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    sqlite3 *db = nullptr;
    int rc = sqlite3_open(":memory:", &db);
    if (rc != 0) {
        if (db) sqlite3_close(db);
        return 1;
    }

    // Install a rollback hook (no-op). Use a captureless lambda converted to function pointer.
    void (*rollback_fn)(void*) = [](void *p){ (void)p; };
    void *prev_hook = sqlite3_rollback_hook(db, rollback_fn, (void*)0x1);

    // Call sqlite3_complete16 with a UTF-16 string literal.
    // Use a char16_t literal and cast to const void* as required by the API.
    int complete = sqlite3_complete16((const void *)u"SELECT 1;");

    // Prepare a simple statement using the UTF-16 API and then reset its scan status.
    sqlite3_stmt *stmt = nullptr;
    const void *pzTail = nullptr;
    rc = sqlite3_prepare16(db, (const void *)u"SELECT 1;", -1, &stmt, &pzTail);
    if (rc == 0 && stmt != nullptr) {
        // Reset scan status (no-op for a freshly prepared statement, but valid usage).
        sqlite3_stmt_scanstatus_reset(stmt);
        // Finalize the statement when done.
        sqlite3_finalize(stmt);
        stmt = nullptr;
    }

    // Call sqlite3_str_value with a null pointer (we pass a null sqlite3_str*).
    // This demonstrates the API usage; avoid dereferencing the returned char*.
    char *str_val = sqlite3_str_value((sqlite3_str*)0);
    (void)str_val; // silence unused-variable warnings

    // Demonstrate cancelling an auto-extension. Provide a captureless lambda converted to a void(*)().
    void (*auto_ext_fn)() = [](){ /* no-op auto-extension entry point */ };
    int cancel_rc = sqlite3_cancel_auto_extension(auto_ext_fn);
    (void)cancel_rc;

    // Unset the rollback hook by restoring previous hook (or NULL).
    sqlite3_rollback_hook(db, nullptr, nullptr);

    // Close the database.
    sqlite3_close(db);
    db = nullptr;

    return 66;
}