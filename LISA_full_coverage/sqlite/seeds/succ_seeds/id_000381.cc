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
//<ID> 381
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    // step 1: Declarations / Initialize
    sqlite3 *db = (sqlite3 *)0;
    sqlite3_stmt *stmt = (sqlite3_stmt *)0;
    sqlite3_str *pStr = (sqlite3_str *)0;
    const char *create_sql = "CREATE TABLE IF NOT EXISTS t(a INTEGER, b TEXT);";
    const char *insert_sql = "INSERT INTO t(a,b) VALUES(42,'hello');";
    const char *select_sql = "SELECT a, b FROM t WHERE a=42;";
    int rc_init = 0;
    int rc = 0;
    int colval = 0;
    int scan_status_out = 0;
    const unsigned char *coltext = (const unsigned char *)0;
    char *final_str = (char *)0;

    rc_init = sqlite3_initialize();

    // step 2: Configure - open DB, create table, register a no-op collation
    rc = sqlite3_open(":memory:", &db);
    rc = sqlite3_prepare_v2(db, create_sql, -1, &stmt, (const char **)0);
    rc = sqlite3_step(stmt);
    rc = sqlite3_finalize(stmt);
    stmt = (sqlite3_stmt *)0;
    rc = sqlite3_create_collation_v2(db, "my_collation", 1, (void *)0,
        (int (*)(void *, int, const void *, int, const void *))0,
        (void (*)(void *))0);

    // step 3: Operate - insert a row, select it, inspect columns and scan status, build string
    rc = sqlite3_prepare_v2(db, insert_sql, -1, &stmt, (const char **)0);
    rc = sqlite3_step(stmt);
    rc = sqlite3_finalize(stmt);
    stmt = (sqlite3_stmt *)0;
    rc = sqlite3_prepare_v2(db, select_sql, -1, &stmt, (const char **)0);
    rc = sqlite3_step(stmt);
    colval = sqlite3_column_int(stmt, 0);
    coltext = sqlite3_column_text(stmt, 1);
    pStr = sqlite3_str_new(db);
    sqlite3_str_appendall(pStr, (const char *)(coltext ? coltext : (const unsigned char *)""));
    rc = sqlite3_stmt_scanstatus(stmt, 0, 0, &scan_status_out);
    rc = sqlite3_finalize(stmt);
    stmt = (sqlite3_stmt *)0;

    // step 4: Validate / Cleanup - finish string and close DB
    final_str = sqlite3_str_finish(pStr);
    pStr = (sqlite3_str *)0;
    rc = sqlite3_close(db);
    db = (sqlite3 *)0;

    (void)rc_init;
    (void)rc;
    (void)colval;
    (void)scan_status_out;
    (void)coltext;
    (void)final_str;

    // API sequence test completed successfully
    return 66;
}