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
//<ID> 578
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
    const char *create_sql = "CREATE TABLE t1(id INTEGER PRIMARY KEY, val TEXT);";
    const char *insert_sql = "INSERT INTO t1(val) VALUES('alpha');";
    const char *select_sql = "SELECT id, val FROM t1;";
    int rc_open = 0;
    int rc_prep = 0;
    int rc_step = 0;
    int rc_final = 0;
    int db_errcode = 0;
    int glob_match = 0;
    unsigned char *serialized = NULL;
    sqlite3_int64 ser_size = 0;
    char temp_buf[32];
    memset(temp_buf, 0, sizeof(temp_buf));

    // step 2: Setup / Configure
    rc_open = sqlite3_open_v2(":memory:", &db, 0, NULL);
    rc_prep = sqlite3_prepare_v3(db, create_sql, -1, 0u, &stmt, &pzTail);
    rc_step = sqlite3_step(stmt);
    rc_final = sqlite3_finalize(stmt);
    rc_prep = sqlite3_prepare_v3(db, insert_sql, -1, 0u, &stmt, &pzTail);
    rc_step = sqlite3_step(stmt);
    rc_final = sqlite3_finalize(stmt);
    rc_prep = sqlite3_prepare_v3(db, select_sql, -1, 0u, &stmt, &pzTail);

    // step 3: Operate / Validate
    rc_step = sqlite3_step(stmt);
    rc_final = sqlite3_finalize(stmt);
    db_errcode = sqlite3_errcode(db);
    glob_match = sqlite3_strglob("SELECT *", select_sql);
    serialized = sqlite3_serialize(db, "main", &ser_size, 0u);

    // step 4: Cleanup
    sqlite3_close_v2(db);

    (void)rc_open;
    (void)rc_prep;
    (void)rc_step;
    (void)rc_final;
    (void)db_errcode;
    (void)glob_match;
    (void)serialized;
    (void)ser_size;
    (void)temp_buf;
    (void)pzTail;
    (void)create_sql;
    (void)insert_sql;
    (void)select_sql;
    // API sequence test completed successfully
    return 66;
}