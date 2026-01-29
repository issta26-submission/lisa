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
//<ID> 25
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

int test_sqlite3_api_sequence() {
    // step 1: Declarations / initialize
    sqlite3 *db = NULL;
    sqlite3_blob *blob = NULL;
    sqlite3_stmt *stmt = NULL;
    char *sqlBuf = NULL;
    char dataBuf[64];
    int rc_open = 0;
    int rc_prep = 0;
    int rc_step = 0;
    int rc_reset = 0;
    int rc_blob_read = 0;

    // step 2: Setup — allocate buffer, format SQL, open DB, create table
    sqlBuf = (char *)sqlite3_malloc64(128ULL);
    memset(sqlBuf, 0, 128);
    sqlite3_snprintf(128, sqlBuf, "CREATE TABLE t(id INTEGER PRIMARY KEY, data BLOB);");
    rc_open = sqlite3_open_v2(":memory:", &db, 0, NULL);
    rc_prep = sqlite3_prepare_v2(db, (const char *)sqlBuf, -1, &stmt, NULL);
    rc_step = sqlite3_step(stmt);
    rc_reset = sqlite3_reset(stmt);

    // step 3: Operate — insert blob, interrupt and sleep, attempt blob_read
    sqlite3_snprintf(128, sqlBuf, "INSERT INTO t(data) VALUES(x'01020304');");
    rc_prep = sqlite3_prepare_v2(db, (const char *)sqlBuf, -1, &stmt, NULL);
    rc_step = sqlite3_step(stmt);
    sqlite3_interrupt(db);
    sqlite3_sleep(1);
    memset(dataBuf, 0, sizeof(dataBuf));
    rc_blob_read = sqlite3_blob_read(blob, dataBuf, 16, 0);

    // step 4: Cleanup — reset stmt, close db, free buffer
    rc_reset = sqlite3_reset(stmt);
    sqlite3_free(sqlBuf);
    sqlite3_close_v2(db);

    (void)blob;
    (void)rc_open;
    (void)rc_prep;
    (void)rc_step;
    (void)rc_reset;
    (void)rc_blob_read;
    (void)dataBuf;

    // API sequence test completed successfully
    return 66;
}