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
//<ID> 461
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
    const char *version = NULL;
    char *heap_copy = NULL;
    const char *sql = "SELECT 1;";
    int rc_prepare = 0;
    int rc_backup_step = 0;
    int allocated_len = 0;
    memset(&rc_prepare, 0, sizeof(rc_prepare));
    memset(&rc_backup_step, 0, sizeof(rc_backup_step));
    memset(&allocated_len, 0, sizeof(allocated_len));

    // step 2: Initialize / Configure
    version = sqlite3_libversion();
    allocated_len = (int)(strlen(version) + 1);
    heap_copy = (char *)sqlite3_malloc(allocated_len);
    memcpy(heap_copy, version, (size_t)allocated_len);

    // step 3: Operate
    rc_prepare = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    rc_backup_step = sqlite3_backup_step(backup, 1);
    sqlite3_result_pointer(ctx, (void *)heap_copy, "sqlite_version", sqlite3_free);

    // step 4: Cleanup / Validate
    stmt = NULL;
    db = NULL;
    backup = NULL;
    ctx = NULL;
    (void)rc_prepare;
    (void)rc_backup_step;
    (void)sql;
    (void)version;
    (void)allocated_len;
    (void)heap_copy;

    // API sequence test completed successfully
    return 66;
}