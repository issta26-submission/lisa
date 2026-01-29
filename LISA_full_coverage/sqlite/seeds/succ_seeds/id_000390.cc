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
//<ID> 390
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
    const unsigned char *coltext = NULL;
    unsigned char *serialized = NULL;
    sqlite3_int64 ser_size = 0;
    const char *sql_create = "CREATE TABLE t(id INTEGER PRIMARY KEY, txt TEXT);";
    const char *sql_insert = "INSERT INTO t(txt) VALUES('hello');";
    const char *sql_select = "SELECT txt FROM t WHERE id=1;";
    int rc_open = 0;
    int rc_prepare = 0;
    int rc_step = 0;
    int rc_finalize = 0;

    // step 2: Setup - open DB and create/insert a row
    rc_open = sqlite3_open(":memory:", &db);
    rc_prepare = sqlite3_prepare_v2(db, sql_create, -1, &stmt, NULL);
    rc_step = sqlite3_step(stmt);
    rc_finalize = sqlite3_finalize(stmt);
    stmt = NULL;
    rc_prepare = sqlite3_prepare_v2(db, sql_insert, -1, &stmt, NULL);
    rc_step = sqlite3_step(stmt);
    rc_finalize = sqlite3_finalize(stmt);
    stmt = NULL;

    // step 3: Core operations - select the row, read column text, create sqlite3_str and serialize using column text as schema name
    rc_prepare = sqlite3_prepare_v2(db, sql_select, -1, &stmt, NULL);
    rc_step = sqlite3_step(stmt);
    coltext = sqlite3_column_text(stmt, 0);
    pStr = sqlite3_str_new(db);
    char *strval = sqlite3_str_value(pStr);
    (void)strval;
    serialized = sqlite3_serialize(db, (const char *)(coltext ? coltext : (const unsigned char *)"main"), &ser_size, 0);

    // step 4: Cleanup - finalize statement and close DB
    rc_finalize = sqlite3_finalize(stmt);
    stmt = NULL;
    rc_open = sqlite3_close_v2(db);
    db = NULL;

    (void)rc_open;
    (void)rc_prepare;
    (void)rc_step;
    (void)rc_finalize;
    (void)pStr;
    (void)coltext;
    (void)serialized;
    (void)ser_size;

    // API sequence test completed successfully
    return 66;
}