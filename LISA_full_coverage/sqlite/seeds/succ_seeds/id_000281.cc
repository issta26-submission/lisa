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
//<ID> 281
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
    const char *err_msg = NULL;
    void *auxdata = NULL;
    int rc_open = 0;
    int rc_close = 0;
    int rc_errcode = 0;
    int rc_str_err = 0;
    int mx_notheld = 0;

    // step 2: Setup - open an in-memory database and allocate a mutex
    rc_open = sqlite3_open(":memory:", &db);
    mx = sqlite3_mutex_alloc(0);

    // step 3: Operate and query state - get error message, DB error code, str error code, and auxdata
    err_msg = sqlite3_errmsg(db);
    rc_errcode = sqlite3_errcode(db);
    rc_str_err = sqlite3_str_errcode(str);
    auxdata = sqlite3_get_auxdata(ctx, 0);
    mx_notheld = sqlite3_mutex_notheld(mx);

    // step 4: Cleanup - close database and tidy local references
    rc_close = sqlite3_close_v2(db);
    db = NULL;
    str = NULL;
    ctx = NULL;
    mx = NULL;

    (void)rc_open;
    (void)rc_close;
    (void)err_msg;
    (void)rc_errcode;
    (void)rc_str_err;
    (void)auxdata;
    (void)mx_notheld;

    // API sequence test completed successfully
    return 66;
}