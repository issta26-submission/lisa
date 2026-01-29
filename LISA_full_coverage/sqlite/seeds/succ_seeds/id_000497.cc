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
//<ID> 497
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    sqlite3 *db = NULL;
    const char *sql = "CREATE TABLE t(x); INSERT INTO t(x) VALUES(42); SELECT x FROM t;";
    int rc_open = sqlite3_open(":memory:", &db);
    char *errmsg = NULL;
    int rc_exec = sqlite3_exec(db, sql, 0, 0, &errmsg);
    const char *emsg = sqlite3_errmsg(db);
    void *buf = sqlite3_malloc(128);
    sqlite3_snprintf(128, (char *)buf, "open=%d exec=%d", rc_open, rc_exec);
    sqlite3_free(errmsg);
    sqlite3_free(buf);
    (void)emsg;
    (void)rc_open;
    (void)rc_exec;
    (void)sql;
    (void)db;
    return 66;
}