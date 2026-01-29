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
//<ID> 469
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
    void *mem = NULL;
    const char *pzTail = NULL;
    const char *sql = "SELECT 1;";
    int rc_prepare = 0;
    int rc_backup = 0;
    size_t ver_len = 0;
    memset(&ver_len, 0, sizeof(ver_len));

    // step 2: Initialize / Configure
    version = sqlite3_libversion();
    ver_len = strlen(version);
    mem = sqlite3_malloc((int)(ver_len + 1));
    memset(mem, 0, ver_len + 1);
    memcpy(mem, version, ver_len);

    // step 3: Operate / Validate
    rc_prepare = sqlite3_prepare_v2(db, sql, -1, &stmt, &pzTail);
    sqlite3_result_pointer(ctx, mem, "libversion", NULL);
    rc_backup = sqlite3_backup_step(backup, 1);

    // step 4: Cleanup / Finalize
    sqlite3_free(mem);
    stmt = NULL;
    db = NULL;
    backup = NULL;
    ctx = NULL;

    (void)rc_prepare;
    (void)rc_backup;
    (void)version;
    (void)pzTail;
    (void)sql;
    (void)ver_len;

    // API sequence test completed successfully
    return 66;
}