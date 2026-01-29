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
//<ID> 385
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
    sqlite3_str *builder = (sqlite3_str *)0;
    const char *create_sql = "CREATE TABLE t(a INTEGER,b TEXT);";
    const char *insert_sql = "INSERT INTO t(a,b) VALUES(42,'hello');";
    const char *select_sql = "SELECT a,b FROM t;";
    const char *colname_b = (const char *)0;
    char *final_str = (char *)0;
    int rc_open = 0;
    int rc_prepare = 0;
    int rc_step = 0;
    int rc_finalize = 0;
    int rc_close = 0;
    int rc_coll = 0;
    int value_a = 0;
    int scanstatus_out = 0;
    int scanstatus_rc = 0;

    // step 2: Setup - open DB and register a collation (configuration)
    rc_open = sqlite3_open(":memory:", &db);
    rc_coll = sqlite3_create_collation_v2(db, "mycoll", 1, (void *)0, (int (*)(void *, int, const void *, int, const void *))0, (void (*)(void *))0);

    // step 3: Operate - create table, insert row, select and inspect using required APIs
    rc_prepare = sqlite3_prepare_v2(db, create_sql, -1, &stmt, (const char **)0);
    rc_step = sqlite3_step(stmt);
    rc_finalize = sqlite3_finalize(stmt);
    stmt = (sqlite3_stmt *)0;

    rc_prepare = sqlite3_prepare_v2(db, insert_sql, -1, &stmt, (const char **)0);
    rc_step = sqlite3_step(stmt);
    rc_finalize = sqlite3_finalize(stmt);
    stmt = (sqlite3_stmt *)0;

    rc_prepare = sqlite3_prepare_v2(db, select_sql, -1, &stmt, (const char **)0);
    rc_step = sqlite3_step(stmt);
    value_a = sqlite3_column_int(stmt, 0);
    colname_b = sqlite3_column_name(stmt, 1);

    builder = sqlite3_str_new(db);
    sqlite3_str_appendall(builder, colname_b);
    scanstatus_rc = sqlite3_stmt_scanstatus(stmt, 0, 0, (void *)&scanstatus_out);

    rc_finalize = sqlite3_finalize(stmt);
    stmt = (sqlite3_stmt *)0;

    final_str = sqlite3_str_finish(builder);
    builder = (sqlite3_str *)0;

    // step 4: Validate-ish (store/use results) and Cleanup
    rc_close = sqlite3_close(db);
    db = (sqlite3 *)0;

    (void)rc_open;
    (void)rc_prepare;
    (void)rc_step;
    (void)rc_finalize;
    (void)rc_close;
    (void)rc_coll;
    (void)value_a;
    (void)colname_b;
    (void)final_str;
    (void)scanstatus_out;
    (void)scanstatus_rc;

    // API sequence test completed successfully
    return 66;
}