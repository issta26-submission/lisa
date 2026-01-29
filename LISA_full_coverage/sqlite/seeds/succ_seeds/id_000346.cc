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
//<ID> 346
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
    const char *sql_create = "CREATE TABLE sample(id INTEGER);";
    const char *sql_insert = "INSERT INTO sample(id) VALUES(42);";
    char tailbuf[64];
    int rc_open = 0;
    int rc_prepare = 0;
    int rc_step = 0;
    int rc_reset = 0;
    int rc_complete_create = 0;
    int rc_complete_insert = 0;
    void *mem = NULL;
    sqlite3_uint64 alloc_size = 256;
    sqlite3_mutex *mtx = NULL;

    // step 2: Setup
    memset(tailbuf, 0, sizeof(tailbuf));
    rc_open = sqlite3_open_v2(":memory:", &db, 0, NULL);
    mem = sqlite3_malloc64(alloc_size);

    // step 3: Core operations
    rc_complete_create = sqlite3_complete(sql_create);
    rc_prepare = sqlite3_prepare(db, sql_create, -1, &stmt, (const char **)NULL);
    rc_step = sqlite3_step(stmt);
    rc_reset = sqlite3_reset(stmt);
    rc_complete_insert = sqlite3_complete(sql_insert);
    rc_prepare = sqlite3_prepare(db, sql_insert, -1, &stmt, (const char **)NULL);
    rc_step = sqlite3_step(stmt);
    sqlite3_mutex_leave(mtx);

    // step 4: Cleanup
    rc_reset = sqlite3_reset(stmt);
    mem = sqlite3_realloc64(mem, 0);
    sqlite3_close(db);

    (void)rc_open;
    (void)rc_prepare;
    (void)rc_step;
    (void)rc_reset;
    (void)rc_complete_create;
    (void)rc_complete_insert;
    (void)tailbuf;
    (void)mem;
    (void)alloc_size;
    (void)mtx;
    (void)stmt;
    (void)db;

    // API sequence test completed successfully
    return 66;
}