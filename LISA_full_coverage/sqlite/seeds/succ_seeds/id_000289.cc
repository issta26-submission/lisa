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
//<ID> 289
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    // step 1: Declarations
    sqlite3 *db = NULL;
    sqlite3_mutex *alloc_mutex = NULL;
    sqlite3_mutex *db_mutex = NULL;
    sqlite3_str *str_builder = NULL;
    sqlite3_context *ctx = NULL;
    const char *errmsg = NULL;
    char *str_value = NULL;
    void *auxdata = NULL;
    int rc_open = 0;
    int mutex_notheld_flag = 0;
    int str_errcode = 0;
    int rc_release = 0;

    // step 2: Setup - open an in-memory database and obtain mutexes
    rc_open = sqlite3_open(":memory:", &db);
    db_mutex = sqlite3_db_mutex(db);
    alloc_mutex = sqlite3_mutex_alloc(0);

    // step 3: Operate and Validate - query error message, auxdata, and string-builder error code/state
    mutex_notheld_flag = sqlite3_mutex_notheld(alloc_mutex);
    errmsg = sqlite3_errmsg(db);
    auxdata = sqlite3_get_auxdata(ctx, 1);
    str_errcode = sqlite3_str_errcode(str_builder);
    str_value = sqlite3_str_value(str_builder);

    // step 4: Cleanup - release memory associated with the DB and coerce unused values
    rc_release = sqlite3_db_release_memory(db);

    (void)rc_open;
    (void)db;
    (void)db_mutex;
    (void)alloc_mutex;
    (void)mutex_notheld_flag;
    (void)errmsg;
    (void)auxdata;
    (void)str_errcode;
    (void)str_value;
    (void)rc_release;

    // API sequence test completed successfully
    return 66;
}