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
//<ID> 189
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

int test_sqlite3_api_sequence() {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt_create = NULL;
    sqlite3_stmt *stmt_insert = NULL;
    sqlite3_stmt *stmt_select = NULL;
    const char *sql_create = "CREATE TABLE t(id INTEGER, name TEXT);";
    const char *sql_insert = "INSERT INTO t(id,name) VALUES(1,'alice');";
    const char *sql_select = "SELECT id, name FROM t;";
    int rc_complete_create = sqlite3_complete(sql_create);
    int rc_complete_insert = sqlite3_complete(sql_insert);
    int rc_open = sqlite3_open(":memory:", &db);
    int rc_prepare_create = sqlite3_prepare_v2(db, sql_create, -1, &stmt_create, NULL);
    int rc_prepare_insert = sqlite3_prepare_v2(db, sql_insert, -1, &stmt_insert, NULL);
    int rc_prepare_select = sqlite3_prepare_v2(db, sql_select, -1, &stmt_select, NULL);
    int colcount = sqlite3_column_count(stmt_select);
    const char *col0_name = sqlite3_column_name(stmt_select, 0);
    const unsigned char *col0_text = sqlite3_column_text(stmt_select, 0);
    const char *libver = sqlite3_libversion();
    sqlite3_int64 last_rowid = sqlite3_last_insert_rowid(db);
    sqlite3_int64 total_changes = sqlite3_changes64(db);
    const char *errmsg = sqlite3_errmsg(db);

    (void)rc_complete_create;
    (void)rc_complete_insert;
    (void)rc_open;
    (void)rc_prepare_create;
    (void)rc_prepare_insert;
    (void)rc_prepare_select;
    (void)colcount;
    (void)col0_name;
    (void)col0_text;
    (void)libver;
    (void)last_rowid;
    (void)total_changes;
    (void)errmsg;
    (void)db;
    (void)stmt_create;
    (void)stmt_insert;
    (void)stmt_select;

    return 66;
}