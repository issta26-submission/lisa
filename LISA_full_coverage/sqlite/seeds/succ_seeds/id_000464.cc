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
//<ID> 464
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
    sqlite3_backup *backup = NULL;
    sqlite3_context *ctx = NULL;
    void *buffer = NULL;
    const char *sql = "SELECT 1;";
    const void *pzTail = NULL;
    const char *version = NULL;
    int rc_init = 0;
    int rc_prepare = 0;
    int rc_finalize = 0;
    int alloc_size = 64;
    memset(&rc_init, 0, sizeof(rc_init));
    memset(&rc_prepare, 0, sizeof(rc_prepare));

    // step 2: Initialize / Configure
    buffer = sqlite3_realloc(NULL, alloc_size);
    memset(buffer, 0, alloc_size);
    version = sqlite3_libversion();
    rc_init = sqlite3_initialize();

    // step 3: Operate / Validate
    rc_prepare = sqlite3_prepare16_v3(db, (const void *)sql, -1, 0u, &stmt, &pzTail);
    rc_finalize = sqlite3_finalize(stmt);

    // step 4: Cleanup / Finalize
    sqlite3_realloc(buffer, 0); /* free the buffer via sqlite3_realloc(ptr,0) */
    stmt = NULL;
    db = NULL;
    backup = NULL;
    ctx = NULL;
    (void)sql;
    (void)pzTail;
    (void)version;
    (void)rc_init;
    (void)rc_prepare;
    (void)rc_finalize;
    (void)alloc_size;

    return 66;
}