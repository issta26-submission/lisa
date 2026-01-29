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
//<ID> 235
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    sqlite3 *db = nullptr;
    sqlite3_stmt *stmt = nullptr;
    int rc = sqlite3_open(":memory:", &db);
    if (rc != 0 || db == nullptr) {
        if (db) sqlite3_close_v2(db);
        return 1;
    }

    const char *src = sqlite3_sourceid();
    (void)src; /* use source id to avoid unused-variable warning */

    /* Prepare and execute a UTF-16 CREATE TABLE statement */
    rc = sqlite3_prepare16(db, (const void *)u"CREATE TABLE test(x INTEGER);", -1, &stmt, nullptr);
    if (rc != 0 || stmt == nullptr) {
        if (stmt) sqlite3_finalize(stmt);
        sqlite3_close_v2(db);
        return 2;
    }
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    stmt = nullptr;

    /* Attempt to recover a snapshot for the main database (may be a no-op) */
    rc = sqlite3_snapshot_recover(db, "main");
    (void)rc; /* ignore return for this test sequence */

    /* Prepare and execute a UTF-16 SELECT statement */
    rc = sqlite3_prepare16(db, (const void *)u"SELECT x FROM test;", -1, &stmt, nullptr);
    if (rc != 0 || stmt == nullptr) {
        if (stmt) sqlite3_finalize(stmt);
        sqlite3_close_v2(db);
        return 3;
    }
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    stmt = nullptr;

    sqlite3_close_v2(db);
    return 66;
}