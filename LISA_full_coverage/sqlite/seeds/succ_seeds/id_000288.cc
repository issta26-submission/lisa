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
//<ID> 288
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    // step 1: Declarations
    sqlite3 *db = NULL;
    sqlite3_mutex *mtx = NULL;
    sqlite3_str *str = NULL;
    sqlite3_context *ctx = NULL;
    const char *errmsg = NULL;
    void *auxdata = NULL;
    char *formatted = NULL;
    int rc_open = 0;
    int str_errcode = 0;

    // step 2: Setup - open an in-memory database and allocate a mutex
    rc_open = sqlite3_open(":memory:", &db);
    mtx = sqlite3_mutex_alloc(0);

    // step 3: Operate - retrieve error message, query str error code and auxdata, format a string
    errmsg = sqlite3_errmsg(db);
    str_errcode = sqlite3_str_errcode(str);
    auxdata = sqlite3_get_auxdata(ctx, 0);
    formatted = sqlite3_mprintf("err:%s code:%d aux:%p", errmsg ? errmsg : "(null)", str_errcode, auxdata);

    // step 4: Cleanup - free formatted string and release DB memory
    sqlite3_free(formatted);
    sqlite3_db_release_memory(db);

    (void)rc_open;
    (void)mtx;
    (void)str;
    (void)ctx;
    (void)errmsg;
    (void)auxdata;
    (void)str_errcode;

    // API sequence test completed successfully
    return 66;
}