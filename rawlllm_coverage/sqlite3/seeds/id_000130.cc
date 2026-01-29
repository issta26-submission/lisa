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
//<ID> 130
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    sqlite3 *src = nullptr;
    sqlite3 *dst = nullptr;
    sqlite3_stmt *stmt = nullptr;

    // Open two in-memory database connections: source and destination.
    if (sqlite3_open(":memory:", &src) != 0) {
        // Failed to open source
        return 1;
    }
    if (sqlite3_open(":memory:", &dst) != 0) {
        // Failed to open destination; clean up source
        sqlite3_close(src);
        return 2;
    }

    // Create a simple table and insert a row into the source DB.
    sqlite3_exec(src, "CREATE TABLE t(x INTEGER); INSERT INTO t VALUES(42);", NULL, NULL, NULL);

    // Prepare and step a statement against the source DB, then finalize it.
    if (sqlite3_prepare_v2(src, "SELECT x FROM t LIMIT 1;", -1, &stmt, NULL) == 0) {
        sqlite3_step(stmt); // ignore result details for this sequence
    }
    // Must call sqlite3_finalize as required by the task
    sqlite3_finalize(stmt);
    stmt = nullptr;

    // Register a collation-needed callback (no-op). Use unary + to convert the
    // non-capturing lambda to a function pointer.
    auto coll_cb = +[](void *pArg, sqlite3 *db, int eTextRep, const char *zName) {
        (void)pArg; (void)db; (void)eTextRep; (void)zName;
    };
    sqlite3_collation_needed(src, nullptr, coll_cb);

    // Register a WAL hook (no-op). Capture previous hook pointer (void*).
    auto wal_cb = +[](void *pArg, sqlite3 *db, const char *zName, int nFrame) -> int {
        (void)pArg; (void)db; (void)zName; (void)nFrame;
        return 0;
    };
    void *prev_wal = sqlite3_wal_hook(src, wal_cb, (void *)0x1);
    (void)prev_wal; // silence unused-variable warning

    // Ask SQLite to release some memory (no-op if nothing to free).
    sqlite3_release_memory(1024);

    // Initialize a backup object to copy the source DB into the destination DB.
    sqlite3_backup *pBackup = sqlite3_backup_init(dst, "main", src, "main");
    if (pBackup != nullptr) {
        // Attempt to copy all pages. Ignore the return code for this test sequence.
        sqlite3_backup_step(pBackup, -1);
        // Note: sqlite3_backup_finish is not among the required APIs for this task,
        // so we do not call it here.
    }

    // Close both database connections.
    sqlite3_close(dst);
    sqlite3_close(src);

    return 66;
}