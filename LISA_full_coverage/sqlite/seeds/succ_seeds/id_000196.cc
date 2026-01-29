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
//<ID> 196
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    sqlite3 *db = NULL;
    const char *libver = sqlite3_libversion();
    int rc_open = sqlite3_open(":memory:", &db);
    const char *create_sql = "CREATE TABLE t(id INTEGER PRIMARY KEY, data TEXT);";
    int rc_create = sqlite3_exec(db, create_sql, NULL, NULL, NULL);
    sqlite3_str *builder = sqlite3_str_new(db);
    sqlite3_str_appendf(builder, "INSERT INTO t(id, data) VALUES(1, '%s');", libver);
    char *insert_sql = sqlite3_str_finish(builder);
    int rc_insert = sqlite3_exec(db, insert_sql, NULL, NULL, NULL);
    sqlite3_int64 lastid = sqlite3_last_insert_rowid(db);
    sqlite3_int64 total_changes = sqlite3_total_changes64(db);
    sqlite3_uint64 mem_used = (sqlite3_uint64)sqlite3_memory_used();
    int rc_close = sqlite3_close(db);

    (void)rc_open;
    (void)rc_create;
    (void)rc_insert;
    (void)lastid;
    (void)total_changes;
    (void)mem_used;
    (void)rc_close;
    (void)insert_sql;
    (void)builder;
    (void)libver;
    (void)create_sql;

    return 66;
}