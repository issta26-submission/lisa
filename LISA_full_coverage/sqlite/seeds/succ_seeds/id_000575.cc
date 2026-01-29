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
//<ID> 575
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
    const char *sql_all = "CREATE TABLE t1(id INTEGER PRIMARY KEY, data TEXT); INSERT INTO t1(data) VALUES('hello'); SELECT 'hello';";
    const char *pzTail = NULL;
    char *expanded = NULL;
    unsigned char *serialized = NULL;
    sqlite3_int64 serialized_size = 0;
    int rc_open = 0;
    int rc_prepare = 0;
    int rc_step = 0;
    int rc_finalize = 0;
    int rc_close = 0;
    int rc_err = 0;
    int glob_match = 0;

    // step 2: Setup / Configure
    rc_open = sqlite3_open_v2(":memory:", &db, 0, NULL);
    rc_prepare = sqlite3_prepare_v3(db, sql_all, -1, 0, &stmt, &pzTail);

    // step 3: Operate / Validate
    rc_step = sqlite3_step(stmt);
    expanded = sqlite3_expanded_sql(stmt);
    glob_match = sqlite3_strglob("*hello*", expanded);
    serialized = sqlite3_serialize(db, "main", &serialized_size, 0);
    rc_err = sqlite3_errcode(db);

    // step 4: Cleanup
    rc_finalize = sqlite3_finalize(stmt);
    if (expanded) sqlite3_free(expanded);
    if (serialized) sqlite3_free((void *)serialized);
    rc_close = sqlite3_close_v2(db);

    (void)rc_open;
    (void)rc_prepare;
    (void)rc_step;
    (void)rc_finalize;
    (void)rc_close;
    (void)rc_err;
    (void)glob_match;
    (void)pzTail;
    (void)serialized_size;
    (void)sql_all;
    (void)stmt;
    (void)db;

    // API sequence test completed successfully
    return 66;
}