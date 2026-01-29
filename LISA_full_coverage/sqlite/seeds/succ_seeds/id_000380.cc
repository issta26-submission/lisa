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
//<ID> 380
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    // step 1: Declarations
    sqlite3 *db = (sqlite3 *)0;
    sqlite3_stmt *stmt = (sqlite3_stmt *)0;
    sqlite3_str *builder = (sqlite3_str *)0;
    char *sql = (char *)0;
    int rc_open = 0;
    int rc_prepare = 0;
    int scan_out = 0;
    int col_count = 0;
    int data_count = 0;
    double col_double = 0.0;

    // step 2: Build CREATE TABLE SQL and prepare it
    builder = sqlite3_str_new((sqlite3 *)0);
    sqlite3_str_appendall(builder, "CREATE TABLE t(id INTEGER PRIMARY KEY, v REAL);");
    sql = sqlite3_str_value(builder);
    rc_open = sqlite3_open(":memory:", &db);
    rc_prepare = sqlite3_prepare_v2(db, sql, -1, &stmt, (const char **)0);

    // Inspect prepared statement (no stepping/finalizing used to avoid mismatched frees)
    rc_prepare = sqlite3_stmt_scanstatus(stmt, 0, 0, (void *)&scan_out);
    col_count = sqlite3_column_count(stmt);
    data_count = sqlite3_data_count(stmt);
    col_double = sqlite3_column_double(stmt, 0);

    // Move to next prepared statement slot (if any)
    stmt = sqlite3_next_stmt(db, stmt);

    // step 3: Build INSERT SQL and prepare it
    sqlite3_str_appendall(builder, "INSERT INTO t(v) VALUES(42);");
    sql = sqlite3_str_value(builder);
    rc_prepare = sqlite3_prepare_v2(db, sql, -1, &stmt, (const char **)0);

    // Inspect second prepared statement similarly
    rc_prepare = sqlite3_stmt_scanstatus(stmt, 0, 0, (void *)&scan_out);
    col_count = sqlite3_column_count(stmt);
    data_count = sqlite3_data_count(stmt);
    col_double = sqlite3_column_double(stmt, 0);

    // Advance statement pointer
    stmt = sqlite3_next_stmt(db, stmt);

    // step 4: Cleanup
    rc_open = sqlite3_close(db);
    db = (sqlite3 *)0;

    (void)rc_open;
    (void)rc_prepare;
    (void)scan_out;
    (void)col_count;
    (void)data_count;
    (void)col_double;
    (void)sql;
    (void)builder;
    (void)stmt;

    // API sequence test completed successfully
    return 66;
}