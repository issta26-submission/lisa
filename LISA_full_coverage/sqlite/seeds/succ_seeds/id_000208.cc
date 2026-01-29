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
//<ID> 208
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    sqlite3 *db = NULL;
    const char *sql_create = "CREATE TABLE t(id INTEGER);";
    const char *sql_insert = "INSERT INTO t(id) VALUES(42);";
    int rc_open = sqlite3_open_v2(":memory:", &db, 0, NULL);
    int rc_ext_err = sqlite3_extended_errcode(db);
    int rc_exec_create = sqlite3_exec(db, sql_create, NULL, NULL, NULL);
    int rc_exec_insert = sqlite3_exec(db, sql_insert, NULL, NULL, NULL);
    sqlite3_int64 last_row = sqlite3_last_insert_rowid(db);
    sqlite3_int64 total_changes = sqlite3_total_changes64(db);
    const char *libver = sqlite3_libversion();
    int rc_close = sqlite3_close(db);

    (void)rc_open;
    (void)rc_ext_err;
    (void)rc_exec_create;
    (void)rc_exec_insert;
    (void)last_row;
    (void)total_changes;
    (void)libver;
    (void)rc_close;
    (void)db;
    (void)sql_create;
    (void)sql_insert;
    return 66;
}