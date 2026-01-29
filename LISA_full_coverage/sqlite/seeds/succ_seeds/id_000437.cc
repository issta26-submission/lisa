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
//<ID> 437
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    // step 1: Declarations / Initialize
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    sqlite3_blob *blob = NULL;
    void *buf = NULL;
    const void *filename16 = (const void *)L":memory:";
    const char *sql_select = "SELECT 1;";
    int rc_init = 0;
    int rc_open = 0;
    int rc_prepare = 0;
    int stmt_stat = 0;
    int autocommit = 0;
    int rc_blob_read = 0;

    // step 2: Setup / Configure - initialize library and open an in-memory database, prepare a statement
    rc_init = sqlite3_initialize();
    rc_open = sqlite3_open16(filename16, &db);
    rc_prepare = sqlite3_prepare_v2(db, sql_select, -1, &stmt, NULL);

    // step 3: Operate / Validate - query statement status, check autocommit, allocate buffer and attempt a blob read
    stmt_stat = sqlite3_stmt_status(stmt, 0, 0);
    autocommit = sqlite3_get_autocommit(db);
    buf = sqlite3_malloc(32);
    memset(buf, 0, 32);
    rc_blob_read = sqlite3_blob_read(blob, buf, 16, 0);

    // step 4: Cleanup - free allocated memory, close database, tidy local references
    sqlite3_realloc(buf, 0);
    sqlite3_close_v2(db);
    db = NULL;
    stmt = NULL;
    blob = NULL;
    buf = NULL;

    (void)rc_init;
    (void)rc_open;
    (void)rc_prepare;
    (void)stmt_stat;
    (void)autocommit;
    (void)rc_blob_read;
    (void)filename16;
    (void)sql_select;

    // API sequence test completed successfully
    return 66;
}