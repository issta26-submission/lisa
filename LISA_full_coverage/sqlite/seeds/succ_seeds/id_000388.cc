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
//<ID> 388
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    // step 1: Declarations & Initialize
    sqlite3 *db = (sqlite3 *)0;
    sqlite3_stmt *stmt = (sqlite3_stmt *)0;
    sqlite3_str *str = (sqlite3_str *)0;
    int rc_init = 0;
    int rc = 0;
    int col_int = 0;
    int scan_out = 0;
    char numbuf[16];
    char *final_str = (char *)0;

    rc_init = sqlite3_initialize();

    // step 2: Configure - open in-memory DB and register (or attempt) a collation
    rc = sqlite3_open(":memory:", &db);
    rc = sqlite3_create_collation_v2(db, "my_collation", 0, (void *)0, (int (*)(void *, int, const void *, int, const void *))0, (void (*)(void *))0);

    // step 3: Operate & Validate - create table, insert a value, select it, inspect with required APIs
    rc = sqlite3_prepare_v2(db, "CREATE TABLE IF NOT EXISTS t(i INTEGER);", -1, &stmt, (const char **)0);
    rc = sqlite3_step(stmt);
    rc = sqlite3_finalize(stmt);
    stmt = (sqlite3_stmt *)0;

    rc = sqlite3_prepare_v2(db, "INSERT INTO t(i) VALUES(42);", -1, &stmt, (const char **)0);
    rc = sqlite3_step(stmt);
    rc = sqlite3_finalize(stmt);
    stmt = (sqlite3_stmt *)0;

    rc = sqlite3_prepare_v2(db, "SELECT i FROM t;", -1, &stmt, (const char **)0);
    rc = sqlite3_step(stmt);
    col_int = sqlite3_column_int(stmt, 0);
    rc = sqlite3_stmt_scanstatus(stmt, 0, 0, (void *)&scan_out);

    str = sqlite3_str_new(db);
    sqlite3_str_appendall(str, "value:");
    memset(numbuf, 0, sizeof(numbuf));
    int tens = col_int / 10;
    int ones = col_int % 10;
    numbuf[0] = (char)('0' + tens);
    numbuf[1] = (char)('0' + ones);
    numbuf[2] = '\0';
    sqlite3_str_appendall(str, numbuf);
    final_str = sqlite3_str_finish(str);
    str = (sqlite3_str *)0;

    rc = sqlite3_finalize(stmt);
    stmt = (sqlite3_stmt *)0;

    // step 4: Cleanup
    rc = sqlite3_close(db);
    db = (sqlite3 *)0;

    (void)rc_init;
    (void)rc;
    (void)col_int;
    (void)scan_out;
    (void)final_str;
    (void)tens;
    (void)ones;

    // API sequence test completed successfully
    return 66;
}