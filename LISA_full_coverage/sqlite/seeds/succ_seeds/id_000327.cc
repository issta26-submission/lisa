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
//<ID> 327
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    sqlite3 *db = NULL;
    sqlite3_snapshot *snapshot = NULL;
    char **table = NULL;
    const void *filename16 = (const void *)L":memory:";
    sqlite3_int64 lastid = 12345;
    int allocSize = (int)(2 * sizeof(char *));
    int rc_open = 0;
    int rc_snapshot = 0;
    int rc_err = 0;

    rc_open = sqlite3_open16(filename16, &db);
    sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS t(x); INSERT INTO t(x) VALUES(1);", 0, 0, 0);
    rc_snapshot = sqlite3_snapshot_get(db, "main", &snapshot);
    table = (char **)sqlite3_realloc(NULL, allocSize);
    table[0] = NULL;
    table[1] = NULL;
    rc_err = sqlite3_extended_errcode(db);
    sqlite3_free(table);
    sqlite3_close(db);

    (void)rc_open;
    (void)rc_snapshot;
    (void)rc_err;
    (void)lastid;
    (void)allocSize;
    (void)filename16;

    return 66;
}