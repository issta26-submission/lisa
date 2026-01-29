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
//<ID> 343
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
    sqlite3_mutex *mtx = NULL;
    void *buffer = NULL;
    const char *sql_create = "CREATE TABLE t(a INTEGER);";
    const char *sql_insert = "INSERT INTO t(a) VALUES(1);";
    const char *sql_select = "SELECT a FROM t;";
    int rc_open = 0;
    int rc_prep_create = 0;
    int rc_step_create = 0;
    int rc_prep_insert = 0;
    int rc_step_insert = 0;
    int rc_prep_select = 0;
    int rc_step_select = 0;
    int compl_create = 0;
    int compl_insert = 0;
    sqlite3_int64 queried_value = 0;

    // step 2: Setup - open in-memory DB and allocate memory
    rc_open = sqlite3_open_v2(":memory:", &db, 0, NULL);
    buffer = sqlite3_malloc64((sqlite3_uint64)128);

    // step 3: Core operations - validate SQL, create table, insert row, query value
    compl_create = sqlite3_complete(sql_create);
    compl_insert = sqlite3_complete(sql_insert);
    rc_prep_create = sqlite3_prepare_v3(db, sql_create, -1, 0, &stmt_create, NULL);
    rc_step_create = sqlite3_step(stmt_create);
    rc_prep_insert = sqlite3_prepare_v3(db, sql_insert, -1, 0, &stmt_insert, NULL);
    rc_step_insert = sqlite3_step(stmt_insert);
    rc_prep_select = sqlite3_prepare_v3(db, sql_select, -1, 0, &stmt_select, NULL);
    rc_step_select = sqlite3_step(stmt_select);
    queried_value = sqlite3_column_int64(stmt_select, 0);

    // step 4: Cleanup - free allocated buffer, release mutex (if any) and close DB
    sqlite3_realloc64(buffer, (sqlite3_uint64)0);
    sqlite3_mutex_leave(mtx);
    sqlite3_close(db);

    (void)rc_open;
    (void)rc_prep_create;
    (void)rc_step_create;
    (void)rc_prep_insert;
    (void)rc_step_insert;
    (void)rc_prep_select;
    (void)rc_step_select;
    (void)compl_create;
    (void)compl_insert;
    (void)queried_value;
    (void)stmt_create;
    (void)stmt_insert;
    (void)stmt_select;
    (void)mtx;

    // API sequence test completed successfully
    return 66;
}