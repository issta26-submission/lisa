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
//<ID> 285
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    // step 1: Declarations
    sqlite3 *db = NULL;
    sqlite3_str *str = NULL;
    sqlite3_context *ctx = NULL;
    sqlite3_mutex *mx = NULL;
    int rc_open = 0;
    const char *errmsg = NULL;
    void *aux = NULL;
    int str_err = 0;
    int mem_release_rc = 0;
    int mx_notheld = 0;

    // step 2: Setup - open an in-memory database and allocate a mutex
    rc_open = sqlite3_open(":memory:", &db);
    mx = sqlite3_mutex_alloc(0);

    // step 3: Operate and Validate - obtain error message, auxdata, and sqlite3_str error code, release DB memory
    errmsg = sqlite3_errmsg(db);
    aux = sqlite3_get_auxdata(ctx, 0);
    str_err = sqlite3_str_errcode(str);
    mem_release_rc = sqlite3_db_release_memory(db);

    // step 4: Cleanup - request shutdown and query mutex state
    sqlite3_shutdown();
    mx_notheld = sqlite3_mutex_notheld(mx);

    (void)rc_open;
    (void)errmsg;
    (void)aux;
    (void)str_err;
    (void)mem_release_rc;
    (void)mx_notheld;
    (void)db;
    (void)str;
    (void)ctx;
    (void)mx;

    // API sequence test completed successfully
    return 66;
}