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
//<ID> 392
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
    char *final_str = NULL;
    char *numbuf = NULL;
    const unsigned char *coltext = NULL;
    unsigned char *serialized = NULL;
    sqlite3_int64 sz = 0;
    int rc = 0;
    const char *sql_create = "CREATE TABLE t(a TEXT);";
    const char *sql_insert = "INSERT INTO t(a) VALUES('hello');";
    const char *sql_select = "SELECT a FROM t LIMIT 1;";

    // step 2: Setup - open in-memory DB and create a table
    rc = sqlite3_open(":memory:", &db);
    rc = sqlite3_prepare_v2(db, sql_create, -1, &stmt, NULL);
    rc = sqlite3_step(stmt);
    rc = sqlite3_finalize(stmt);
    stmt = NULL;

    // step 3: Core operations - insert a row, select it, capture column text, serialize DB, build string
    rc = sqlite3_prepare_v2(db, sql_insert, -1, &stmt, NULL);
    rc = sqlite3_step(stmt);
    rc = sqlite3_finalize(stmt);
    stmt = NULL;

    rc = sqlite3_prepare_v2(db, sql_select, -1, &stmt, NULL);
    rc = sqlite3_step(stmt);
    coltext = sqlite3_column_text(stmt, 0);
    rc = sqlite3_finalize(stmt);
    stmt = NULL;

    serialized = sqlite3_serialize(db, "main", &sz, 0);

    pStr = sqlite3_str_new(db);
    sqlite3_str_appendall(pStr, (const char *)coltext);
    sqlite3_str_appendall(pStr, " ");
    numbuf = sqlite3_mprintf("%lld", (long long)sz);
    sqlite3_str_appendall(pStr, numbuf);
    final_str = sqlite3_str_value(pStr);

    // step 4: Cleanup
    rc = sqlite3_close_v2(db);
    db = NULL;

    (void)rc;
    (void)serialized;
    (void)final_str;
    (void)numbuf;
    (void)pStr;
    (void)coltext;
    (void)sz;

    // API sequence test completed successfully
    return 66;
}