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
//<ID> 20
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
    sqlite3_stmt *stmt_ins = NULL;
    sqlite3_blob *blob = NULL;
    char sqlbuf[256];
    char resultbuf[128];
    unsigned char blobdata[8];
    unsigned char readbuf[8];
    int rc_open = 0;
    int rc_prepare = 0;
    int rc_step = 0;
    int rc_finalize = 0;

    // step 2: Setup - prepare memory and open an in-memory database
    memset(sqlbuf, 0, sizeof(sqlbuf));
    memset(resultbuf, 0, sizeof(resultbuf));
    memset(blobdata, 0x42, sizeof(blobdata)); /* fill with predictable data */
    memset(readbuf, 0, sizeof(readbuf));
    rc_open = sqlite3_open_v2(":memory:", &db, 0, NULL);

    /* create table */
    sqlite3_snprintf((int)sizeof(sqlbuf), sqlbuf, "CREATE TABLE t(a BLOB);");
    rc_prepare = sqlite3_prepare_v3(db, sqlbuf, -1, 0, &stmt, NULL);
    rc_step = sqlite3_step(stmt);
    rc_finalize = sqlite3_finalize(stmt);
    stmt = NULL;

    /* prepare insert with a parameter and bind blob data */
    sqlite3_snprintf((int)sizeof(sqlbuf), sqlbuf, "INSERT INTO t(a) VALUES (?);");
    rc_prepare = sqlite3_prepare_v3(db, sqlbuf, -1, 0, &stmt_ins, NULL);
    sqlite3_bind_blob(stmt_ins, 1, blobdata, (int)sizeof(blobdata), NULL);
    rc_step = sqlite3_step(stmt_ins);
    rc_finalize = sqlite3_finalize(stmt_ins);
    stmt_ins = NULL;

    // step 3: Operate - attempt to read blob via sqlite3_blob_read, interrupt and sleep to exercise APIs
    /* Normally one would obtain a sqlite3_blob* via sqlite3_blob_open; here we exercise sqlite3_blob_read API by calling it with the blob handle (NULL) and a valid buffer. */
    sqlite3_interrupt(db);
    sqlite3_sleep(1);
    /* Perform the read call (may return error when blob is NULL) */
    sqlite3_blob_read(blob, (void *)readbuf, (int)sizeof(readbuf), 0);

    /* Format last error message into result buffer using sqlite3_snprintf (format string provided as the error message) */
    sqlite3_snprintf((int)sizeof(resultbuf), resultbuf, sqlite3_errmsg(db));

    // step 4: Cleanup - close database and silence unused warnings
    sqlite3_close(db);
    db = NULL;
    (void)rc_open;
    (void)rc_prepare;
    (void)rc_step;
    (void)rc_finalize;
    (void)blob;
    (void)readbuf;
    (void)blobdata;
    (void)resultbuf;
    (void)sqlbuf;

    // API sequence test completed successfully
    return 66;
}