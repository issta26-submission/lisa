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
//<ID> 22
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
    char sqlbuf[128];
    char blobbuf[64];
    char fmtbuf[64];
    int rc_open = 0;
    int rc_prep = 0;
    int rc_step = 0;
    int rc_finalize = 0;
    int rc_blob = 0;

    // step 2: Setup - open in-memory DB and create/insert using prepared statements
    sqlite3_snprintf((int)sizeof(sqlbuf), sqlbuf, "CREATE TABLE t(x);");
    rc_open = sqlite3_open_v2(":memory:", &db, 6, NULL);
    rc_prep = sqlite3_prepare_v3(db, sqlbuf, -1, 0, &stmt, NULL);
    rc_step = sqlite3_step(stmt);
    rc_finalize = sqlite3_finalize(stmt);

    sqlite3_snprintf((int)sizeof(sqlbuf), sqlbuf, "INSERT INTO t(x) VALUES(123);");
    rc_prep = sqlite3_prepare_v3(db, sqlbuf, -1, 0, &stmt, NULL);
    rc_step = sqlite3_step(stmt);
    rc_finalize = sqlite3_finalize(stmt);

    // step 3: Operate - interrupt any running operations, pause briefly, attempt a blob read and format a result
    sqlite3_interrupt(db);
    sqlite3_sleep(1);
    memset(blobbuf, 0, sizeof(blobbuf));
    rc_blob = sqlite3_blob_read(blob, blobbuf, (int)sizeof(blobbuf), 0);
    sqlite3_snprintf((int)sizeof(fmtbuf), fmtbuf, "blob_rc=%d", rc_blob);

    // step 4: Validate & Cleanup - close database and silence unused warnings
    sqlite3_close(db);

    (void)rc_open;
    (void)rc_prep;
    (void)rc_step;
    (void)rc_finalize;
    (void)rc_blob;
    (void)sqlbuf;
    (void)blobbuf;
    (void)fmtbuf;
    (void)blob;
    (void)stmt;
    (void)db;

    // API sequence test completed successfully
    return 66;
}