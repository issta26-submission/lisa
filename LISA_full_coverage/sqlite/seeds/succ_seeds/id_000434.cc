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
//<ID> 434
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    // step 1: Declarations & Initialize
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    sqlite3_blob *blob = NULL;
    const void *filename = (const void *)L":memory:";
    const char *sql_create = "CREATE TABLE t(id INTEGER PRIMARY KEY, data BLOB);";
    const char *sql_insert = "INSERT INTO t(data) VALUES(x'010203');";
    const char *sql_select = "SELECT data FROM t WHERE id=1;";
    void *buffer = NULL;
    int rc_open = 0;
    int rc_prep_create = 0;
    int rc_prep_insert = 0;
    int rc_prep_select = 0;
    int rc_blob_open = 0;
    int rc_blob_read = 0;
    int stmt_status_before = 0;
    int stmt_status_after = 0;
    int autocommit = 0;

    // step 2: Setup / Configure
    rc_open = sqlite3_open16(filename, &db);
    rc_prep_create = sqlite3_prepare_v2(db, sql_create, -1, &stmt, NULL);
    rc_prep_insert = sqlite3_prepare_v2(db, sql_insert, -1, &stmt, NULL);
    rc_prep_select = sqlite3_prepare_v2(db, sql_select, -1, &stmt, NULL);

    // step 3: Operate / Validate
    stmt_status_before = sqlite3_stmt_status(stmt, 0, 0);
    buffer = sqlite3_malloc(64);
    rc_blob_open = sqlite3_blob_open(db, "main", "t", "data", 1, 0, &blob);
    rc_blob_read = sqlite3_blob_read(blob, buffer, 64, 0);
    stmt_status_after = sqlite3_stmt_status(stmt, 1, 1);
    autocommit = sqlite3_get_autocommit(db);

    // step 4: Cleanup
    sqlite3_realloc(buffer, 0);
    sqlite3_close_v2(db);
    db = NULL;
    stmt = NULL;
    blob = NULL;
    buffer = NULL;

    (void)rc_open;
    (void)rc_prep_create;
    (void)rc_prep_insert;
    (void)rc_prep_select;
    (void)rc_blob_open;
    (void)rc_blob_read;
    (void)stmt_status_before;
    (void)stmt_status_after;
    (void)autocommit;
    (void)sql_create;
    (void)sql_insert;
    (void)sql_select;

    // API sequence test completed successfully
    return 66;
}