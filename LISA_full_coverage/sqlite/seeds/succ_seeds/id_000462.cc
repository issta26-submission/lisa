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
//<ID> 462
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
    const char *libver = NULL;
    void *buffer = NULL;
    const char *sql = "CREATE TABLE IF NOT EXISTS t(x); SELECT 1;";
    const char *pzTail = NULL;
    int rc_open = 0;
    int rc_prepare = 0;
    int rc_backup_step = 0;
    int rc_backup_finish = 0;
    int rc_finalize = 0;
    int rc_close = 0;

    // step 2: Initialize / Configure
    rc_open = sqlite3_open_v2(":memory:", &db, 0, NULL);
    libver = sqlite3_libversion();
    buffer = sqlite3_malloc(128);
    memset(buffer, 0, 128);

    // step 3: Operate
    rc_prepare = sqlite3_prepare_v2(db, sql, -1, &stmt, &pzTail);
    backup = sqlite3_backup_init(db, "main", db, "main");
    rc_backup_step = sqlite3_backup_step(backup, 1);
    sqlite3_result_pointer(ctx, buffer, "buffer", NULL);

    // step 4: Cleanup / Validate
    rc_finalize = sqlite3_finalize(stmt);
    rc_backup_finish = sqlite3_backup_finish(backup);
    rc_close = sqlite3_close_v2(db);
    sqlite3_free(buffer);
    stmt = NULL;
    db = NULL;
    backup = NULL;
    ctx = NULL;

    (void)libver;
    (void)rc_open;
    (void)rc_prepare;
    (void)rc_backup_step;
    (void)rc_backup_finish;
    (void)rc_finalize;
    (void)rc_close;
    (void)pzTail;
    (void)sql;

    // API sequence test completed successfully
    return 66;
}