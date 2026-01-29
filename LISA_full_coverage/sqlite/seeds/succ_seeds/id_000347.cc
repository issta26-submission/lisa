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
//<ID> 347
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
    void *mem = NULL;
    const char *sql = "CREATE TABLE IF NOT EXISTS test(id INTEGER PRIMARY KEY, value TEXT);";
    char buf[64];
    int rc_open = 0;
    int rc_prepare = 0;
    int rc_step = 0;
    int rc_complete_res = 0;
    int rc_reset = 0;
    int rc_close = 0;

    memset(buf, 0, sizeof(buf));

    // step 2: Setup - open database, allocate memory and validate SQL completeness
    rc_open = sqlite3_open_v2(":memory:", &db, 0, NULL);
    mem = sqlite3_malloc64((sqlite3_uint64)128);
    rc_complete_res = sqlite3_complete(sql);

    // step 3: Core operations - prepare statement, execute it and manipulate mutex/memory
    rc_prepare = sqlite3_prepare(db, sql, -1, &stmt, NULL);
    rc_step = sqlite3_step(stmt);
    sqlite3_mutex_leave(mtx);
    mem = sqlite3_realloc64(mem, (sqlite3_uint64)0);

    // step 4: Cleanup - reset statement and close database
    rc_reset = sqlite3_reset(stmt);
    rc_close = sqlite3_close_v2(db);

    (void)buf;
    (void)rc_open;
    (void)rc_prepare;
    (void)rc_step;
    (void)rc_complete_res;
    (void)rc_reset;
    (void)rc_close;
    (void)stmt;
    (void)mtx;
    (void)mem;
    (void)sql;

    // API sequence test completed successfully
    return 66;
}