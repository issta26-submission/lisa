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
//<ID> 574
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
    const char *sql_create = "CREATE TABLE t1(id INTEGER PRIMARY KEY, val TEXT);";
    const char *sql_insert = "INSERT INTO t1(val) VALUES('abc');";
    const char *sql_select = "SELECT val FROM t1;";
    const char *pzTail = NULL;
    sqlite3_int64 serialized_size = 0;
    unsigned char *serialized_db = NULL;
    const void *col_text = NULL;
    int rc_open = 0;
    int rc_prep = 0;
    int rc_step = 0;
    int rc_finalize = 0;
    int rc_errcode = 0;
    int glob_match = 0;
    int rc_close = 0;

    // step 2: Setup / Configure (open DB and create table)
    rc_open = sqlite3_open_v2(":memory:", &db, 0, NULL);
    rc_prep = sqlite3_prepare_v3(db, sql_create, -1, 0u, &stmt, &pzTail);
    rc_step = sqlite3_step(stmt);
    rc_finalize = sqlite3_finalize(stmt);
    stmt = NULL;

    // step 3: Operate / Validate (insert a row, select it, use glob and serialize)
    rc_prep = sqlite3_prepare_v3(db, sql_insert, -1, 0u, &stmt, &pzTail);
    rc_step = sqlite3_step(stmt);
    rc_finalize = sqlite3_finalize(stmt);
    stmt = NULL;
    rc_prep = sqlite3_prepare_v3(db, sql_select, -1, 0u, &stmt, &pzTail);
    rc_step = sqlite3_step(stmt);
    col_text = sqlite3_column_text16(stmt, 0);
    glob_match = sqlite3_strglob("a*", col_text ? (const char *)col_text : "");
    rc_finalize = sqlite3_finalize(stmt);
    stmt = NULL;
    serialized_db = sqlite3_serialize(db, "main", &serialized_size, 0u);
    rc_errcode = sqlite3_errcode(db);

    // step 4: Cleanup
    rc_close = sqlite3_close_v2(db);

    (void)serialized_db;
    (void)serialized_size;
    (void)rc_open;
    (void)rc_prep;
    (void)rc_step;
    (void)rc_finalize;
    (void)rc_errcode;
    (void)glob_match;
    (void)rc_close;
    (void)col_text;
    (void)pzTail;

    // API sequence test completed successfully
    return 66;
}