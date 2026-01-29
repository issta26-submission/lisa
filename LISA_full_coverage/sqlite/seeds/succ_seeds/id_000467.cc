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
//<ID> 467
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    // step 1: Declarations
    sqlite3 *db = NULL;
    sqlite3_backup *backup = NULL;
    sqlite3_stmt *stmt = NULL;
    sqlite3_context *ctx = NULL;
    const char *libver = NULL;
    char *mem = NULL;
    const char *sql = "SELECT 1;";
    int rc_prepare = 0;
    int rc_backup = 0;
    int marker = 0;
    memset(&marker, 0, sizeof(marker));

    // step 2: Initialize / Configure
    libver = sqlite3_libversion();
    mem = (char *)sqlite3_malloc(128);
    memset(mem, 0, 128);

    // step 3: Operate / Validate
    rc_prepare = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    rc_backup = sqlite3_backup_step(backup, 1);
    sqlite3_result_pointer(ctx, (void *)libver, "libver", NULL);

    // step 4: Cleanup / Finalize
    sqlite3_free(mem);
    mem = NULL;
    stmt = NULL;
    db = NULL;
    ctx = NULL;
    backup = NULL;
    (void)rc_prepare;
    (void)rc_backup;
    (void)marker;
    (void)sql;
    (void)libver;

    // API sequence test completed successfully
    return 66;
}