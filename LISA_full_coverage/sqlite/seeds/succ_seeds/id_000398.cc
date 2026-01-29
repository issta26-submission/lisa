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
//<ID> 398
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
    char *strval = NULL;
    const unsigned char *col_text = NULL;
    unsigned char *serialized = NULL;
    sqlite3_int64 serialized_size = 0;
    int rc_open = 0;
    int rc_prepare = 0;
    int rc_step = 0;
    const char *sql_create = "CREATE TABLE t(id INTEGER PRIMARY KEY, txt TEXT);";
    const char *sql_insert = "INSERT INTO t(txt) VALUES('hello world');";
    const char *sql_select = "SELECT txt FROM t WHERE id=1;";

    // step 2: Setup - open database and create table, insert a row
    rc_open = sqlite3_open(":memory:", &db);
    rc_prepare = sqlite3_prepare_v2(db, sql_create, -1, &stmt, NULL);
    rc_step = sqlite3_step(stmt);
    rc_step = sqlite3_reset(stmt);
    rc_prepare = sqlite3_prepare_v2(db, sql_insert, -1, &stmt, NULL);
    rc_step = sqlite3_step(stmt);
    rc_step = sqlite3_reset(stmt);

    // step 3: Operate - select the inserted text, serialize the DB, and obtain a sqlite3_str value
    rc_prepare = sqlite3_prepare_v2(db, sql_select, -1, &stmt, NULL);
    rc_step = sqlite3_step(stmt);
    col_text = sqlite3_column_text(stmt, 0);
    serialized = sqlite3_serialize(db, "main", &serialized_size, 0);
    pStr = sqlite3_str_new(db);
    strval = sqlite3_str_value(pStr);

    // step 4: Cleanup - reset statement and close database
    rc_step = sqlite3_reset(stmt);
    rc_open = sqlite3_close_v2(db);
    db = NULL;

    (void)rc_open;
    (void)rc_prepare;
    (void)rc_step;
    (void)col_text;
    (void)serialized;
    (void)serialized_size;
    (void)pStr;
    (void)strval;

    // API sequence test completed successfully
    return 66;
}