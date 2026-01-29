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
//<ID> 144
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    sqlite3 *db = NULL;
    char **results = NULL;
    int nrow = 0;
    int ncol = 0;
    char *exec_errmsg = NULL;
    const char *last_errmsg = NULL;
    int rc_open = sqlite3_open(":memory:", &db);
    const char *sql_setup = "CREATE TABLE t(id INTEGER PRIMARY KEY, val TEXT); INSERT INTO t(val) VALUES('hello'); SELECT * FROM t;";
    int rc_get = sqlite3_get_table(db, sql_setup, &results, &nrow, &ncol, &exec_errmsg);
    last_errmsg = sqlite3_errmsg(db);
    sqlite3_int64 prev_limit = sqlite3_soft_heap_limit64(0);
    sqlite3_int64 mem_used = sqlite3_memory_used();
    sqlite3_free_table(results);
    int rc_close = sqlite3_close(db);
    (void)rc_open;
    (void)rc_get;
    (void)exec_errmsg;
    (void)last_errmsg;
    (void)prev_limit;
    (void)mem_used;
    (void)rc_close;
    (void)results;
    (void)nrow;
    (void)ncol;
    return 66;
}