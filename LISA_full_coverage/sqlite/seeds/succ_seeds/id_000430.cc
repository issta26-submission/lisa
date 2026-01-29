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
//<ID> 430
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    // step 1: Declarations
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    sqlite3_blob *blob = NULL;
    void *buffer = NULL;
    void *realloc_result = NULL;
    const void *filename = (const void *)L":memory:";
    const char *sql_create = "CREATE TABLE t(id INTEGER PRIMARY KEY, data BLOB);";
    int rc_open = 0;
    int rc_prepare = 0;
    int autocommit = 0;
    int stmt_status = 0;
    int blob_read_rc = 0;

    // step 2: Initialize / Configure
    rc_open = sqlite3_open16(filename, &db);
    autocommit = sqlite3_get_autocommit(db);
    buffer = sqlite3_malloc(128);

    // step 3: Operate / Validate
    rc_prepare = sqlite3_prepare_v2(db, sql_create, -1, &stmt, NULL);
    stmt_status = sqlite3_stmt_status(stmt, 0, 1);
    blob_read_rc = sqlite3_blob_read(blob, buffer, 0, 0);

    // step 4: Cleanup
    realloc_result = sqlite3_realloc(buffer, 0);
    sqlite3_close_v2(db);
    db = NULL;
    stmt = NULL;
    blob = NULL;
    buffer = NULL;
    realloc_result = NULL;

    (void)rc_open;
    (void)rc_prepare;
    (void)autocommit;
    (void)stmt_status;
    (void)blob_read_rc;
    (void)sql_create;
    (void)filename;

    // API sequence test completed successfully
    return 66;
}