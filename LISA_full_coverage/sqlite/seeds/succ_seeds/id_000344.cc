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
//<ID> 344
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

int test_sqlite3_api_sequence() {
    // step 1: Declarations
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt_create = NULL;
    sqlite3_stmt *stmt_insert = NULL;
    sqlite3_stmt *stmt_select = NULL;
    const char *sql_create = "CREATE TABLE test(id INTEGER PRIMARY KEY, val INTEGER);";
    const char *sql_insert = "INSERT INTO test(val) VALUES(123);";
    const char *sql_select = "SELECT val FROM test WHERE id=1;";
    int rc_open = 0;
    int rc_prep_create = 0;
    int rc_step_create = 0;
    int rc_prep_insert = 0;
    int rc_step_insert = 0;
    int rc_prep_select = 0;
    int rc_step_select = 0;
    int complete_insert = 0;
    void *buffer = NULL;
    sqlite3_uint64 allocSize = 128;
    sqlite3_mutex *mtx = NULL;
    sqlite3_int64 fetched_val = 0;
    const char *expanded_sql = NULL;

    // step 2: Setup - open an in-memory database
    rc_open = sqlite3_open_v2(":memory:", &db, 0, NULL);

    // step 3: Core operations - validate SQL completeness, prepare and execute statements, allocate memory, use mutex correctly
    complete_insert = sqlite3_complete(sql_insert);
    rc_prep_create = sqlite3_prepare_v3(db, sql_create, -1, 0, &stmt_create, NULL);
    rc_step_create = sqlite3_step(stmt_create);
    rc_prep_insert = sqlite3_prepare_v3(db, sql_insert, -1, 0, &stmt_insert, NULL);
    rc_step_insert = sqlite3_step(stmt_insert);
    rc_prep_select = sqlite3_prepare_v3(db, sql_select, -1, 0, &stmt_select, NULL);
    rc_step_select = sqlite3_step(stmt_select);
    fetched_val = sqlite3_column_int64(stmt_select, 0);
    expanded_sql = sqlite3_expanded_sql(stmt_select);
    buffer = sqlite3_malloc64(allocSize);
    memset(buffer, 0, (size_t)allocSize);
    sqlite3_snprintf((int)allocSize, (char *)buffer, "fetched=%lld", (long long)fetched_val);

    // Use a proper sqlite3 mutex: allocate, enter, leave, free
    mtx = sqlite3_mutex_alloc(0);
    sqlite3_mutex_enter(mtx);
    sqlite3_mutex_leave(mtx);
    sqlite3_mutex_free(mtx);

    // step 4: Cleanup - reset statements and close database
    sqlite3_reset(stmt_create);
    sqlite3_reset(stmt_insert);
    sqlite3_reset(stmt_select);
    sqlite3_close_v2(db);

    (void)rc_open;
    (void)rc_prep_create;
    (void)rc_step_create;
    (void)rc_prep_insert;
    (void)rc_step_insert;
    (void)rc_prep_select;
    (void)rc_step_select;
    (void)complete_insert;
    (void)buffer;
    (void)allocSize;
    (void)mtx;
    (void)fetched_val;
    (void)expanded_sql;
    (void)stmt_create;
    (void)stmt_insert;
    (void)stmt_select;

    // API sequence test completed successfully
    return 66;
}