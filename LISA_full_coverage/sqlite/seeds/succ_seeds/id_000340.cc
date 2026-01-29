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
//<ID> 340
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
    const char *pzTail = NULL;
    const char *sql_text = NULL;
    char sqlbuf[128];
    void *mem_block = NULL;
    sqlite3_mutex *mtx = NULL;
    sqlite3_uint64 alloc_size = (sqlite3_uint64)sizeof(void *);
    int rc_open = 0;
    int rc_prepare = 0;
    int rc_step = 0;
    int rc_reset = 0;
    int rc_close = 0;
    int complete_res = 0;

    memset(sqlbuf, 0, sizeof(sqlbuf));

    // step 2: Setup - open an in-memory database
    rc_open = sqlite3_open_v2(":memory:", &db, 0x00000002 | 0x00000004, NULL);

    // step 3: Core operations - build SQL, validate completeness, prepare and step
    sqlite3_snprintf(sizeof(sqlbuf), sqlbuf, "SELECT 1;");
    sql_text = sqlbuf;
    complete_res = sqlite3_complete(sql_text);
    rc_prepare = sqlite3_prepare_v3(db, sql_text, -1, 0u, &stmt, &pzTail);
    rc_step = sqlite3_step(stmt);
    rc_reset = sqlite3_reset(stmt);

    // allocate a small block and treat it as a mutex pointer, then operate on it safely and free it
    mem_block = sqlite3_malloc64(alloc_size ? alloc_size : 1);
    mtx = (sqlite3_mutex *)mem_block;
    // Do a safe non-mutex-specific operation on the allocated block to avoid asserting on bogus mutex pointers.
    sqlite3_randomness((int)alloc_size, mem_block);
    mem_block = sqlite3_realloc64(mem_block, 0);

    // step 4: Cleanup - close database and tidy up
    rc_close = sqlite3_close_v2(db);

    (void)rc_open;
    (void)rc_prepare;
    (void)rc_step;
    (void)rc_reset;
    (void)rc_close;
    (void)complete_res;
    (void)pzTail;
    (void)stmt;
    (void)sql_text;
    (void)mtx;
    (void)alloc_size;

    // API sequence test completed successfully
    return 66;
}