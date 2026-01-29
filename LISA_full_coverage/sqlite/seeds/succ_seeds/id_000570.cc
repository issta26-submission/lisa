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
//<ID> 570
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
    const char *sql_create = "CREATE TABLE t1(id INTEGER PRIMARY KEY, data TEXT);";
    const char *sql_insert = "INSERT INTO t1(data) VALUES('hello');";
    const char *pzTail = NULL;
    unsigned char *serialized = NULL;
    sqlite3_int64 serialized_size = 0;
    const char *schema_name = "main";
    const char *glob_pattern = "t*";
    int rc_open = 0;
    int rc_prepare = 0;
    int rc_step = 0;
    int rc_finalize = 0;
    int rc_prepare2 = 0;
    int rc_step2 = 0;
    int rc_finalize2 = 0;
    int glob_res = 0;
    int last_err = 0;

    // step 2: Setup / Configure
    rc_open = sqlite3_open_v2(":memory:", &db, 0, NULL);
    rc_prepare = sqlite3_prepare_v3(db, sql_create, -1, 0u, &stmt, &pzTail);
    rc_step = sqlite3_step(stmt);
    rc_finalize = sqlite3_finalize(stmt);
    stmt = NULL;
    rc_prepare2 = sqlite3_prepare_v3(db, sql_insert, -1, 0u, &stmt, &pzTail);
    rc_step2 = sqlite3_step(stmt);
    rc_finalize2 = sqlite3_finalize(stmt);
    stmt = NULL;

    // step 3: Operate / Validate
    serialized = sqlite3_serialize(db, schema_name, &serialized_size, 0u);
    glob_res = sqlite3_strglob(glob_pattern, "t1");
    last_err = sqlite3_errcode(db);

    // step 4: Cleanup
    sqlite3_close_v2(db);
    (void)serialized; /* serialized buffer allocated by sqlite3_serialize; left for caller or test harness */
    (void)serialized_size;
    (void)rc_open;
    (void)rc_prepare;
    (void)rc_step;
    (void)rc_finalize;
    (void)rc_prepare2;
    (void)rc_step2;
    (void)rc_finalize2;
    (void)glob_res;
    (void)last_err;
    (void)stmt;
    (void)pzTail;
    (void)schema_name;
    (void)sql_create;
    (void)sql_insert;

    // API sequence test completed successfully
    return 66;
}