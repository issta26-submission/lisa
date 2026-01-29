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
//<ID> 466
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
    const char *tail = NULL;
    const char *ver = NULL;
    char *vercopy = NULL;
    int rc_prepare = 0;
    int rc_backup = 0;
    int verlen = 0;
    const char *sql = "SELECT sqlite_version();";

    memset(&db, 0, sizeof(db));
    memset(&stmt, 0, sizeof(stmt));
    memset(&backup, 0, sizeof(backup));
    memset(&ctx, 0, sizeof(ctx));
    memset(&tail, 0, sizeof(tail));
    memset(&ver, 0, sizeof(ver));
    memset(&vercopy, 0, sizeof(vercopy));
    memset(&rc_prepare, 0, sizeof(rc_prepare));
    memset(&rc_backup, 0, sizeof(rc_backup));
    memset(&verlen, 0, sizeof(verlen));

    // step 2: Initialize / Configure
    ver = sqlite3_libversion();
    verlen = (int)strlen(ver);
    vercopy = (char *)sqlite3_malloc(verlen + 1);
    memcpy(vercopy, ver, verlen + 1);

    // Note: do not allocate opaque sqlite3 types -- keep them NULL.
    // step 3: Operate / Validate
    rc_prepare = sqlite3_prepare_v2(db, sql, -1, &stmt, &tail);
    rc_backup = sqlite3_backup_step(backup, 1);
    sqlite3_result_pointer(ctx, (void *)vercopy, "libver", NULL);

    // step 4: Cleanup / Finalize
    sqlite3_free((void *)vercopy);
    sqlite3_free((void *)stmt);
    sqlite3_free((void *)backup);
    sqlite3_free((void *)db);
    sqlite3_free((void *)ctx);

    db = NULL;
    stmt = NULL;
    backup = NULL;
    ctx = NULL;
    ver = NULL;
    vercopy = NULL;
    tail = NULL;
    (void)sql;
    (void)rc_prepare;
    (void)rc_backup;
    (void)verlen;

    // API sequence test completed successfully
    return 66;
}