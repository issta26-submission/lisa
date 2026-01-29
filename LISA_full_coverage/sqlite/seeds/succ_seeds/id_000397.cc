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
//<ID> 397
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
    char *strOut = NULL;
    const unsigned char *coltext = NULL;
    unsigned char *serialized = NULL;
    sqlite3_int64 serialized_size = 0;
    const char *sql_create = "CREATE TABLE t(a TEXT);";
    const char *sql_insert = "INSERT INTO t(a) VALUES('hello');";
    const char *sql_select = "SELECT a FROM t LIMIT 1;";
    int rc_open = 0;
    int rc_prepare = 0;
    int rc_step = 0;
    int rc_finalize = 0;

    // step 2: Setup - open DB and create & populate table
    rc_open = sqlite3_open(":memory:", &db);
    rc_prepare = sqlite3_prepare_v2(db, sql_create, -1, &stmt, NULL);
    rc_step = sqlite3_step(stmt);
    rc_finalize = sqlite3_finalize(stmt);
    stmt = NULL;
    rc_prepare = sqlite3_prepare_v2(db, sql_insert, -1, &stmt, NULL);
    rc_step = sqlite3_step(stmt);
    rc_finalize = sqlite3_finalize(stmt);
    stmt = NULL;

    // step 3: Core operations - select, inspect column text, build string, serialize DB
    rc_prepare = sqlite3_prepare_v2(db, sql_select, -1, &stmt, NULL);
    rc_step = sqlite3_step(stmt);
    coltext = sqlite3_column_text(stmt, 0);
    pStr = sqlite3_str_new(db);
    sqlite3_str_appendall(pStr, "value=");
    sqlite3_str_appendall(pStr, (const char *)coltext);
    (void)sqlite3_str_value(pStr); /* use required API */
    strOut = sqlite3_str_finish(pStr);
    pStr = NULL;
    serialized = sqlite3_serialize(db, NULL, &serialized_size, 0);
    sqlite3_free(serialized);
    serialized = NULL;
    rc_finalize = sqlite3_finalize(stmt);
    stmt = NULL;

    // step 4: Cleanup
    sqlite3_free(strOut);
    strOut = NULL;
    sqlite3_close(db);
    db = NULL;

    (void)rc_open;
    (void)rc_prepare;
    (void)rc_step;
    (void)rc_finalize;
    (void)coltext;
    (void)serialized_size;

    // API sequence test completed successfully
    return 66;
}