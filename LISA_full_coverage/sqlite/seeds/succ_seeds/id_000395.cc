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
//<ID> 395
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
    const unsigned char *coltxt = NULL;
    unsigned char *serialized = NULL;
    sqlite3_int64 ser_size = 0;
    char *strval = NULL;
    int rc_open = 0;
    int rc_prep = 0;
    int rc_step = 0;
    int rc_fin = 0;

    // step 2: Setup - open in-memory DB and create a table
    rc_open = sqlite3_open(":memory:", &db);
    rc_prep = sqlite3_prepare_v2(db, "CREATE TABLE t(x TEXT);", -1, &stmt, NULL);
    rc_step = sqlite3_step(stmt);
    rc_fin = sqlite3_finalize(stmt);
    stmt = NULL;

    // step 3: Operate - insert a row, select it, read column text, serialize DB, create sqlite3_str and read its value
    rc_prep = sqlite3_prepare_v2(db, "INSERT INTO t(x) VALUES('hello');", -1, &stmt, NULL);
    rc_step = sqlite3_step(stmt);
    rc_fin = sqlite3_finalize(stmt);
    stmt = NULL;
    rc_prep = sqlite3_prepare_v2(db, "SELECT x FROM t LIMIT 1;", -1, &stmt, NULL);
    rc_step = sqlite3_step(stmt);
    coltxt = sqlite3_column_text(stmt, 0);
    serialized = sqlite3_serialize(db, "main", &ser_size, 0);
    pStr = sqlite3_str_new(db);
    strval = sqlite3_str_value(pStr);

    // step 4: Cleanup - finalize/select statement, close DB, and tidy local variables
    rc_fin = sqlite3_finalize(stmt);
    stmt = NULL;
    rc_open = sqlite3_close(db);
    db = NULL;

    (void)rc_open;
    (void)rc_prep;
    (void)rc_step;
    (void)rc_fin;
    (void)coltxt;
    (void)serialized;
    (void)ser_size;
    (void)pStr;
    (void)strval;

    return 66;
    // API sequence test completed successfully
}