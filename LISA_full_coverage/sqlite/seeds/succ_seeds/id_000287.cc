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
//<ID> 287
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    // step 1: Declarations & Initialize
    sqlite3 *db = NULL;
    sqlite3_mutex *alloc_mutex = NULL;
    sqlite3_mutex *db_mutex = NULL;
    sqlite3_str *sstr = NULL;
    sqlite3_context *ctx = NULL;
    const char *errmsg = NULL;
    int str_err = 0;
    void *aux = NULL;
    int rc_open = 0;
    int rc_limit = 0;
    int rc_changes = 0;
    int rc_close = 0;
    int notheld_alloc = 0;
    int notheld_db = 0;
    const char *source = NULL;

    // step 2: Setup / Configure
    rc_open = sqlite3_open(":memory:", &db);
    alloc_mutex = sqlite3_mutex_alloc(0);
    db_mutex = sqlite3_db_mutex(db);
    errmsg = sqlite3_errmsg(db);
    str_err = sqlite3_str_errcode(sstr);
    aux = sqlite3_get_auxdata(ctx, 0);
    notheld_alloc = sqlite3_mutex_notheld(alloc_mutex);
    notheld_db = sqlite3_mutex_notheld(db_mutex);

    // step 3: Operate / Validate
    rc_limit = sqlite3_limit(db, 1, str_err);
    rc_changes = sqlite3_changes(db);
    source = sqlite3_sourceid();

    // step 4: Cleanup
    rc_close = sqlite3_close(db);
    (void)errmsg;
    (void)aux;
    (void)notheld_alloc;
    (void)notheld_db;
    (void)rc_open;
    (void)rc_limit;
    (void)rc_changes;
    (void)rc_close;
    (void)sstr;
    (void)ctx;
    (void)alloc_mutex;
    (void)db_mutex;
    (void)source;

    // API sequence test completed successfully
    return 66;
}