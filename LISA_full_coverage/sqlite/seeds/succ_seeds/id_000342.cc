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
//<ID> 342
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
    int rc_open = 0;
    int rc_prep = 0;
    int rc_step = 0;
    int rc_reset = 0;
    int rc_close = 0;
    sqlite3_int64 bind_val = (sqlite3_int64)4242;
    sqlite3_int64 selected_val = 0;
    sqlite3_uint64 alloc_size = (sqlite3_uint64)128;
    void *buf = NULL;
    sqlite3_mutex *mtx = NULL;
    const char *create_sql = "CREATE TABLE IF NOT EXISTS t(x INTEGER);";
    const char *insert_sql = "INSERT INTO t(x) VALUES(?);";
    const char *select_sql = "SELECT x FROM t;";
    int complete_create = 0;
    int complete_select = 0;

    // step 2: Setup - open in-memory database and allocate buffer
    rc_open = sqlite3_open_v2(":memory:", &db, 0, NULL);
    buf = sqlite3_malloc64(alloc_size);
    memset(buf, 0, (size_t)alloc_size);
    complete_create = sqlite3_complete(create_sql);
    complete_select = sqlite3_complete(select_sql);

    // step 3: Core operations - create table, insert a value, query it
    rc_prep = sqlite3_prepare_v3(db, create_sql, -1, 0u, &stmt, NULL);
    rc_step = sqlite3_step(stmt);
    rc_reset = sqlite3_reset(stmt);
    rc_prep = sqlite3_reset(stmt); /* reuse rc_prep variable to hold additional rc */
    rc_prep = sqlite3_prepare_v3(db, insert_sql, -1, 0u, &stmt, NULL);
    rc_step = sqlite3_bind_int64(stmt, 1, bind_val);
    rc_step = sqlite3_step(stmt);
    rc_reset = sqlite3_reset(stmt);
    rc_prep = sqlite3_prepare_v3(db, select_sql, -1, 0u, &stmt, NULL);
    rc_step = sqlite3_step(stmt);
    selected_val = sqlite3_column_int64(stmt, 0);
    rc_reset = sqlite3_reset(stmt);

    // step 4: Cleanup - leave mutex, free buffer, finalize and close
    sqlite3_mutex_leave(mtx);
    buf = sqlite3_realloc64(buf, 0);
    sqlite3_finalize(stmt);
    rc_close = sqlite3_close(db);

    (void)selected_val;
    (void)bind_val;
    (void)rc_open;
    (void)rc_prep;
    (void)rc_step;
    (void)rc_reset;
    (void)rc_close;
    (void)alloc_size;
    (void)complete_create;
    (void)complete_select;
    (void)buf;
    (void)mtx;

    // API sequence test completed successfully
    return 66;
}