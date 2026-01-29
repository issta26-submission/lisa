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
//<ID> 399
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
    unsigned char *serialized = NULL;
    const unsigned char *coltext = NULL;
    sqlite3_int64 ser_size = 0;
    const char *sql_create = "CREATE TABLE t(id INTEGER PRIMARY KEY, txt TEXT);";
    const char *sql_insert = "INSERT INTO t(txt) VALUES('hello');";
    const char *sql_select = "SELECT txt FROM t WHERE id=1;";
    int rc_open = 0;
    int rc_exec1 = 0;
    int rc_exec2 = 0;
    int rc_prepare = 0;
    int rc_step = 0;
    int rc_reset = 0;
    int rc_close = 0;

    // step 2: Setup - open in-memory DB and create/insert a row
    rc_open = sqlite3_open(":memory:", &db);
    rc_exec1 = sqlite3_exec(db, sql_create, NULL, NULL, NULL);
    rc_exec2 = sqlite3_exec(db, sql_insert, NULL, NULL, NULL);

    // step 3: Core operations - prepare/select/inspect/serialize/build string
    rc_prepare = sqlite3_prepare_v2(db, sql_select, -1, &stmt, NULL);
    rc_step = sqlite3_step(stmt);
    coltext = sqlite3_column_text(stmt, 0);
    pStr = sqlite3_str_new(db);
    strOut = sqlite3_str_value(pStr);
    serialized = sqlite3_serialize(db, "main", &ser_size, 0);

    // step 4: Cleanup - reset statement and close DB, tidy up local references
    rc_reset = sqlite3_reset(stmt);
    rc_close = sqlite3_close_v2(db);
    db = NULL;
    stmt = NULL;
    pStr = NULL;

    (void)rc_open;
    (void)rc_exec1;
    (void)rc_exec2;
    (void)rc_prepare;
    (void)rc_step;
    (void)rc_reset;
    (void)rc_close;
    (void)coltext;
    (void)serialized;
    (void)ser_size;
    (void)strOut;

    // API sequence test completed successfully
    return 66;
}