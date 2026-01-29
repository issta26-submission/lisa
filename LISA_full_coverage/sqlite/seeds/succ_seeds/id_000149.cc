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
//<ID> 149
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    // step 1: Declarations / Initialize
    sqlite3 *db = NULL;
    const char *last_errmsg = NULL;
    const char *tag = "sqlite_err";

    // step 2: Create/open an in-memory database so sqlite3_errmsg can be safely called
    int rc_open = sqlite3_open_v2(":memory:", &db, 0, NULL);

    // step 3: Configure - obtain an error message string from the DB handle
    last_errmsg = sqlite3_errmsg(db);

    // step 4: Operate / Validate - attach the error message pointer to the database client-data and log it
    sqlite3_set_clientdata(db, tag, (void *)last_errmsg, (void (*)(void *))NULL);
    sqlite3_log(0, last_errmsg);

    // touch another DB-related API to exercise the handle
    int ext_err = sqlite3_extended_errcode(db);
    sqlite3_db_config(db, 0);

    // step 5: Cleanup (no close API used from provided list; just silence unused vars)
    (void)rc_open;
    (void)ext_err;
    (void)db;
    (void)tag;
    (void)last_errmsg;

    // API sequence test completed successfully
    return 66;
}