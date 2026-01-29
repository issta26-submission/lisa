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
//<ID> 460
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    // step 1: Declarations
    sqlite3_context *ctx = NULL;
    sqlite3_backup *backup = NULL;
    sqlite3_stmt *stmt = NULL;
    const char *ver = NULL;
    void *mem = NULL;
    const char *sql = "SELECT sqlite_version();";
    int rc_prepare = 0;
    int rc_backup = 0;
    int ver_len = 0;
    int alloc_size = 0;

    // step 2: Initialize / Configure
    ver = sqlite3_libversion();
    ver_len = (int)strlen(ver);
    alloc_size = ver_len + 1;
    mem = sqlite3_malloc(alloc_size);
    memset(mem, 0, alloc_size);
    memcpy(mem, ver, ver_len);
    backup = (sqlite3_backup *)sqlite3_malloc(64);

    // step 3: Operate / Validate
    rc_prepare = sqlite3_prepare((sqlite3 *)NULL, sql, -1, &stmt, NULL);
    rc_backup = sqlite3_stmt_isexplain(stmt);
    sqlite3_result_text(ctx, (const char *)mem, alloc_size, NULL);

    // step 4: Cleanup / Finalize
    sqlite3_realloc(mem, 0);
    sqlite3_realloc(backup, 0);
    stmt = NULL;
    ctx = NULL;
    (void)rc_prepare;
    (void)rc_backup;
    (void)ver;
    (void)sql;
    (void)stmt;
    (void)ctx;

    // API sequence test completed successfully
    return 66;
}