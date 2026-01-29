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
//<ID> 321
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    sqlite3 *db = NULL;
    sqlite3_snapshot *snap = NULL;
    void *mem = NULL;
    char **table = NULL;
    const void *filename16 = (const void *)L":memory:";
    int rc_open = 0;
    int rc_exec = 0;
    int rc_snapshot = 0;
    char *errmsg = NULL;
    sqlite3_int64 lastid = (sqlite3_int64)12345;
    char sqlbuf[128];

    /* Build SQL that sets the last insert rowid by inserting a known value. */
    snprintf(sqlbuf, sizeof(sqlbuf),
             "CREATE TABLE t(id INTEGER PRIMARY KEY); INSERT INTO t(id) VALUES(%lld);",
             (long long)lastid);

    rc_open = sqlite3_open16(filename16, &db);
    rc_exec = sqlite3_exec(db, sqlbuf, 0, 0, &errmsg);

    mem = sqlite3_realloc(NULL, (int)sizeof(char *));
    table = (char **)mem;
    table[0] = NULL;

    rc_snapshot = sqlite3_snapshot_get(db, "main", &snap);

    sqlite3_free(errmsg);
    sqlite3_free(mem);

    sqlite3_close(db);

    (void)rc_open;
    (void)rc_exec;
    (void)rc_snapshot;
    (void)filename16;
    (void)lastid;
    (void)snap;

    return 66;
}