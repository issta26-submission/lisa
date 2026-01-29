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
//<ID> 237
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    // step 1: Declarations and initialization
    sqlite3 *db = NULL;
    sqlite3_str *str_builder = NULL;
    sqlite3_stmt *stmt = NULL;
    sqlite3_context *ctx = NULL;
    sqlite3_value *val = NULL;
    sqlite3_mutex *mtx = NULL;
    const void *pzTail = NULL;
    const void *sql16 = (const void *)L"CREATE TABLE t(x INTEGER);";
    int rc_open = 0;
    int rc_prep16 = 0;
    int held = 0;
    const char *err_msg = NULL;

    // small buffer for SQL text (allocated with sqlite API)
    char *buf = NULL;
    char *snprintf_res = NULL;

    // step 2: Open in-memory DB and allocate buffer via sqlite3_malloc64
    rc_open = sqlite3_open_v2(":memory:", &db, 0x00000006, NULL);
    buf = (char *)sqlite3_malloc64(128);

    // step 3: Build SQL text into the buffer and prepare UTF-16 statement
    snprintf_res = sqlite3_snprintf(128, buf, "CREATE TABLE t(x INTEGER);");
    rc_prep16 = sqlite3_prepare16(db, sql16, -1, &stmt, &pzTail);

    // step 4: Inspect derived handles and mutex state, get error message, close DB
    sqlite3 *db_from_stmt = sqlite3_db_handle(stmt);
    const char *dbname = sqlite3_db_name(db_from_stmt, 0);
    mtx = sqlite3_db_mutex(db_from_stmt);
    held = sqlite3_mutex_notheld(mtx);
    err_msg = sqlite3_errmsg(db);
    sqlite3_close_v2(db);

    // silence unused-variable warnings while keeping straight-line code
    (void)str_builder;
    (void)ctx;
    (void)val;
    (void)snprintf_res;
    (void)buf;
    (void)rc_open;
    (void)rc_prep16;
    (void)pzTail;
    (void)held;
    (void)err_msg;
    (void)dbname;

    // API sequence test completed successfully
    return 66;
}