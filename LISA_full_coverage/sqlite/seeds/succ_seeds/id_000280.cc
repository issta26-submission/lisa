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
//<ID> 280
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    // step 1: Declarations
    sqlite3 *db = NULL;
    sqlite3_mutex *mut = NULL;
    sqlite3_str *s = NULL;
    sqlite3_context *ctx = NULL;
    const char *errmsg = NULL;
    void *aux = NULL;
    sqlite3_int64 last_id = 0;
    int changes = 0;
    int rc_open = 0;
    int rc_exec = 0;
    int rc_close = 0;
    int str_err = 0;
    int mutex_notheld = 0;

    // step 2: Setup - open an in-memory database and create a table with one row
    rc_open = sqlite3_open(":memory:", &db);
    rc_exec = sqlite3_exec(db, "CREATE TABLE t(x TEXT); INSERT INTO t(x) VALUES('fuzz');", NULL, NULL, NULL);

    // step 3: Operate and validate - inspect error message, get last row id and changes, allocate mutex, query aux/str error
    errmsg = sqlite3_errmsg(db);
    last_id = sqlite3_last_insert_rowid(db);
    changes = sqlite3_changes(db);
    mut = sqlite3_mutex_alloc(0);
    mutex_notheld = sqlite3_mutex_notheld(mut);
    aux = sqlite3_get_auxdata(ctx, 0);
    str_err = sqlite3_str_errcode(s);

    // step 4: Cleanup - flush caches, release DB and tidy up
    sqlite3_db_cacheflush(db);
    rc_close = sqlite3_close_v2(db);
    db = NULL;

    (void)rc_open;
    (void)rc_exec;
    (void)errmsg;
    (void)last_id;
    (void)changes;
    (void)mut;
    (void)mutex_notheld;
    (void)aux;
    (void)str_err;
    (void)rc_close;

    // API sequence test completed successfully
    return 66;
}