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
//<ID> 59
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    // step 1: Declarations / initialize
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    sqlite3_str *pStr = NULL;
    const char *sql_create = "CREATE TABLE t(a); INSERT INTO t VALUES(1);";
    const char *sql_select = "SELECT a FROM t;";
    const char *tblname = NULL;
    const char *stmt_sql = NULL;
    int rc_open = 0;
    int rc_exec = 0;
    int rc_prep = 0;
    int status = 0;
    int expired = 0;

    // step 2: Setup - open in-memory database and create a table with a row
    rc_open = sqlite3_open_v2(":memory:", &db, 6, NULL); // flags 6 = READWRITE|CREATE
    rc_exec = sqlite3_exec(db, sql_create, 0, 0, 0);

    // step 3: Operate / Validate - prepare a SELECT, inspect statement SQL and column origin, query stmt status/expired, use sqlite3_str helper
    rc_prep = sqlite3_prepare(db, sql_select, -1, &stmt, NULL);
    stmt_sql = sqlite3_sql(stmt);
    tblname = sqlite3_column_table_name(stmt, 0);
    status = sqlite3_stmt_status(stmt, 0, 1);
    expired = sqlite3_expired(stmt);
    pStr = sqlite3_str_new(db);

    // step 4: Cleanup - (no explicit finalize/close calls used from provided API list)
    (void)rc_open;
    (void)rc_exec;
    (void)rc_prep;
    (void)stmt_sql;
    (void)tblname;
    (void)status;
    (void)expired;
    (void)pStr;
    (void)db;
    (void)stmt;

    // API sequence test completed successfully
    return 66;
}