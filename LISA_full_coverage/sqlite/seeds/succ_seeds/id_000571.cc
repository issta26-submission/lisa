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
//<ID> 571
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    // step 1: Declarations / Initialize
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    const char *sql_create = "CREATE TABLE t(a INTEGER); INSERT INTO t(a) VALUES(123);";
    const char *sql_select = "SELECT a FROM t;";
    unsigned char *serialized = NULL;
    sqlite3_int64 serialized_size = 0;
    int rc_open = 0;
    int rc_exec = 0;
    int rc_prep = 0;
    int rc_step = 0;
    int rc_finalize = 0;
    int rc_close = 0;
    int errcode = 0;
    int glob_match = 0;

    // step 2: Setup / Configure
    rc_open = sqlite3_open_v2(":memory:", &db, 0, NULL);
    rc_exec = sqlite3_exec(db, sql_create, NULL, NULL, NULL);
    rc_prep = sqlite3_prepare_v3(db, sql_select, -1, 0, &stmt, NULL);

    // step 3: Operate / Validate
    rc_step = sqlite3_step(stmt);
    errcode = sqlite3_errcode(db);
    glob_match = sqlite3_strglob("t*", "t");
    serialized = sqlite3_serialize(db, "main", &serialized_size, 0);

    // step 4: Cleanup
    rc_finalize = sqlite3_finalize(stmt);
    rc_close = sqlite3_close_v2(db);

    (void)rc_open;
    (void)rc_exec;
    (void)rc_prep;
    (void)rc_step;
    (void)errcode;
    (void)glob_match;
    (void)serialized;
    (void)serialized_size;
    (void)rc_finalize;
    (void)rc_close;

    // API sequence test completed successfully
    return 66;
}