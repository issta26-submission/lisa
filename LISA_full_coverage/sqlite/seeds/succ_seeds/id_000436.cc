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
//<ID> 436
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    // step 1: Declarations / Initialize
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt_create = NULL;
    sqlite3_stmt *stmt_select = NULL;
    sqlite3_blob *blob = NULL;
    const void *filename = (const void *)L":memory:";
    const char *sql_create = "CREATE TABLE t(id INTEGER PRIMARY KEY, data BLOB);";
    const char *sql_insert = "INSERT INTO t(id, data) VALUES(1, x'010203');";
    const char *sql_select = "SELECT data FROM t WHERE id=1;";
    void *buffer = NULL;
    int rc_open = 0;
    int rc_prepare_create = 0;
    int rc_prepare_insert = 0;
    int rc_prepare_select = 0;
    int stmt_stat = 0;
    int autocommit = 0;
    int rc_blob_read = 0;

    // step 2: Setup - open DB and prepare statements
    rc_open = sqlite3_open16(filename, &db);
    rc_prepare_create = sqlite3_prepare_v2(db, sql_create, -1, &stmt_create, NULL);
    rc_prepare_insert = sqlite3_prepare_v2(db, sql_insert, -1, &stmt_select, NULL);
    rc_prepare_select = sqlite3_prepare_v2(db, sql_select, -1, &stmt_select, NULL);

    // step 3: Operate / Validate - allocate memory, inspect statement status, autocommit, and attempt blob read
    buffer = sqlite3_malloc(64);
    memset(buffer, 0, 64);
    stmt_stat = sqlite3_stmt_status(stmt_select, 0, 1);
    autocommit = sqlite3_get_autocommit(db);
    rc_blob_read = sqlite3_blob_read(blob, buffer, 16, 0);

    // step 4: Cleanup - free allocated memory, close DB, clear local references
    buffer = sqlite3_realloc(buffer, 0);
    sqlite3_close(db);
    db = NULL;
    stmt_create = NULL;
    stmt_select = NULL;
    blob = NULL;

    (void)rc_open;
    (void)rc_prepare_create;
    (void)rc_prepare_insert;
    (void)rc_prepare_select;
    (void)stmt_stat;
    (void)autocommit;
    (void)rc_blob_read;
    (void)filename;
    (void)sql_create;
    (void)sql_insert;
    (void)sql_select;

    // API sequence test completed successfully
    return 66;
}