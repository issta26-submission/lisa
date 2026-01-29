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
//<ID> 396
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
    sqlite3_str *pStr = NULL;
    const unsigned char *col_text = NULL;
    unsigned char *serialized = NULL;
    char *strVal = NULL;
    sqlite3_int64 ser_size = 0;
    const char *sql_create = "CREATE TABLE t(a TEXT);";
    const char *sql_insert = "INSERT INTO t(a) VALUES('hello');";
    const char *sql_select = "SELECT a FROM t LIMIT 1;";
    int rc_open = 0;
    int rc_exec = 0;
    int rc_prepare = 0;
    int rc_step = 0;
    int rc_finalize = 0;
    int rc_close = 0;

    // step 2: Setup - open database and create table
    rc_open = sqlite3_open(":memory:", &db);
    rc_exec = sqlite3_exec(db, sql_create, 0, 0, 0);

    // step 3: Core operations - insert a row, prepare/select, inspect column, serialize DB, build/get str
    rc_exec = sqlite3_exec(db, sql_insert, 0, 0, 0);
    rc_prepare = sqlite3_prepare_v2(db, sql_select, -1, &stmt, NULL);
    rc_step = sqlite3_step(stmt);
    col_text = sqlite3_column_text(stmt, 0);
    serialized = sqlite3_serialize(db, "main", &ser_size, 0);
    pStr = sqlite3_str_new(db);
    strVal = sqlite3_str_value(pStr);

    // step 4: Cleanup - finalize statement, free serialized buffer if present, close DB
    rc_finalize = sqlite3_finalize(stmt);
    stmt = NULL;
    if (serialized) { free((void *)serialized); }
    rc_close = sqlite3_close(db);
    db = NULL;

    (void)rc_open;
    (void)rc_exec;
    (void)rc_prepare;
    (void)rc_step;
    (void)rc_finalize;
    (void)rc_close;
    (void)col_text;
    (void)strVal;
    (void)pStr;
    (void)ser_size;

    // API sequence test completed successfully
    return 66;
}