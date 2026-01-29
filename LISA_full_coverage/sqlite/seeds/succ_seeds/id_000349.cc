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
//<ID> 349
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
    void *buffer = NULL;
    sqlite3_mutex *mtx = NULL;
    const char *create_sql = "CREATE TABLE IF NOT EXISTS demo(id INTEGER PRIMARY KEY);";
    const char *insert_sql = "INSERT INTO demo(id) VALUES(1);";
    const char *select_sql = "SELECT id FROM demo;";
    int rc_open = 0;
    int rc_prepare = 0;
    int rc_step = 0;
    int complete_create = 0;
    int complete_select = 0;
    sqlite3_uint64 alloc_size = (sqlite3_uint64)256;

    // step 2: Setup - open an in-memory database
    rc_open = sqlite3_open_v2(":memory:", &db, 0, NULL);

    // step 3: Core operations - validate SQL completeness, prepare and step statements, allocate memory, and leave a mutex
    complete_create = sqlite3_complete(create_sql);
    rc_prepare = sqlite3_prepare_v3(db, create_sql, -1, 0u, &stmt, NULL);
    rc_step = sqlite3_step(stmt);
    rc_prepare = sqlite3_prepare_v3(db, insert_sql, -1, 0u, &stmt, NULL);
    rc_step = sqlite3_step(stmt);
    rc_prepare = sqlite3_prepare_v3(db, select_sql, -1, 0u, &stmt, NULL);
    rc_step = sqlite3_step(stmt);
    buffer = sqlite3_malloc64(alloc_size);
    if (buffer) {
        memset(buffer, 0, (size_t)alloc_size);
    }
    sqlite3_mutex_leave(mtx);
    complete_select = sqlite3_complete("SELECT 1;");

    // step 4: Cleanup - close database and tidy up
    sqlite3_reset(stmt);
    sqlite3_close(db);

    (void)buffer;
    (void)mtx;
    (void)rc_open;
    (void)rc_prepare;
    (void)rc_step;
    (void)complete_create;
    (void)complete_select;
    (void)alloc_size;
    (void)create_sql;
    (void)insert_sql;
    (void)select_sql;
    // API sequence test completed successfully
    return 66;
}