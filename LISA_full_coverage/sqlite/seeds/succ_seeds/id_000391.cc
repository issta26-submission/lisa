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
//<ID> 391
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
    unsigned char *serialized = NULL;
    const unsigned char *coltext = NULL;
    char *strOut = NULL;
    sqlite3_int64 serialized_size = 0;
    const char *sql_create = "CREATE TABLE t(x TEXT);";
    const char *sql_insert = "INSERT INTO t(x) VALUES('hello');";
    const char *sql_select = "SELECT x FROM t;";
    int rc_open = 0;
    int rc_prepare = 0;
    int rc_step = 0;
    int rc_finalize = 0;
    int rc_close = 0;

    // step 2: Setup - open DB and create/insert data using prepared statements
    rc_open = sqlite3_open(":memory:", &db);
    rc_prepare = sqlite3_prepare_v2(db, sql_create, -1, &stmt, NULL);
    rc_step = sqlite3_step(stmt);
    rc_finalize = sqlite3_finalize(stmt);
    stmt = NULL;
    rc_prepare = sqlite3_prepare_v2(db, sql_insert, -1, &stmt, NULL);
    rc_step = sqlite3_step(stmt);
    rc_finalize = sqlite3_finalize(stmt);
    stmt = NULL;

    // step 3: Core operations and validation - select, inspect column text, serialize DB, inspect sqlite3_str
    rc_prepare = sqlite3_prepare_v2(db, sql_select, -1, &stmt, NULL);
    rc_step = sqlite3_step(stmt);
    coltext = sqlite3_column_text(stmt, 0);
    serialized = sqlite3_serialize(db, "main", &serialized_size, 0);
    pStr = sqlite3_str_new(db);
    strOut = sqlite3_str_value(pStr);
    rc_finalize = sqlite3_finalize(stmt);
    stmt = NULL;

    // step 4: Cleanup
    rc_close = sqlite3_close(db);
    db = NULL;

    (void)rc_open;
    (void)rc_prepare;
    (void)rc_step;
    (void)rc_finalize;
    (void)rc_close;
    (void)coltext;
    (void)serialized;
    (void)serialized_size;
    (void)pStr;
    (void)strOut;

    // API sequence test completed successfully
    return 66;
}