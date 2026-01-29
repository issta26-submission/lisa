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
//<ID> 282
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    // step 1: Declarations
    sqlite3 *db = NULL;
    sqlite3_mutex *mx = NULL;
    sqlite3_str *str = NULL;
    sqlite3_context *ctx = NULL;
    const char *errmsg = NULL;
    void *aux = NULL;
    int rc_open = 0;
    int rc_close = 0;
    int str_err = 0;

    // step 2: Setup - open an in-memory database
    rc_open = sqlite3_open(":memory:", &db);

    // step 3: Configure & Operate - allocate a mutex, query error message, check string error code, get auxdata
    mx = sqlite3_mutex_alloc(0);
    errmsg = sqlite3_errmsg(db);
    str_err = sqlite3_str_errcode(str);
    aux = sqlite3_get_auxdata(ctx, 1);

    // step 4: Validate & Cleanup - close database and silence unused-variable warnings
    rc_close = sqlite3_close_v2(db);
    db = NULL;
    (void)mx;
    (void)str;
    (void)ctx;
    (void)errmsg;
    (void)aux;
    (void)rc_open;
    (void)rc_close;
    (void)str_err;

    // API sequence test completed successfully
    return 66;
}