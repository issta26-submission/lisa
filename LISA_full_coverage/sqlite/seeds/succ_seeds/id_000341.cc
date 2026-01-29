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
//<ID> 341
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
    sqlite3_mutex *mtx = NULL;
    void *buf = NULL;
    char *filename = NULL;
    const char *sql = "SELECT 1;";
    int rc_open = 0;
    int rc_prepare = 0;
    int rc_step = 0;
    int rc_complete = 0;
    int rc_reset = 0;
    int rc_close = 0;

    // step 2: Setup
    buf = sqlite3_malloc64((sqlite3_uint64)64);
    filename = (char *)buf;
    sqlite3_snprintf(64, filename, ":memory:");
    rc_open = sqlite3_open_v2(filename, &db, 0, NULL);

    // step 3: Core operations
    rc_complete = sqlite3_complete(sql);
    rc_prepare = sqlite3_prepare_v3(db, sql, -1, 0U, &stmt, NULL);
    rc_step = sqlite3_step(stmt);

    // step 4: Cleanup
    rc_reset = sqlite3_reset(stmt);
    buf = sqlite3_realloc64(buf, 0);
    sqlite3_mutex_leave(mtx);
    rc_close = sqlite3_close_v2(db);

    (void)rc_open;
    (void)rc_prepare;
    (void)rc_step;
    (void)rc_complete;
    (void)rc_reset;
    (void)rc_close;
    (void)stmt;
    (void)mtx;
    (void)filename;

    // API sequence test completed successfully
    return 66;
}