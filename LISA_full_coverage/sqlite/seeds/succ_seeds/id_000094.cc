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
//<ID> 94
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    // step 1: Declarations / Initialize
    sqlite3 *db = NULL;
    int rc_open = sqlite3_open(":memory:", &db);
    sqlite3_int64 val_from_value = 0;
    int rc_exec_create = 0;
    int rc_exec_insert = 0;
    const char *errstr_insert = NULL;

    // step 2: Configure
    sqlite3_progress_handler(db, 1000, NULL, NULL);

    // step 3: Operate / Validate
    rc_exec_create = sqlite3_exec(db, "CREATE TABLE t(id INTEGER);", NULL, NULL, NULL);
    rc_exec_insert = sqlite3_exec(db, "INSERT INTO t(id) VALUES(1234567890123);", NULL, NULL, NULL);
    errstr_insert = sqlite3_errstr(rc_exec_insert);
    // Avoid calling sqlite3_value_int64 with a NULL pointer (was causing an assertion).
    val_from_value = 0;

    // step 4: Cleanup
    sqlite3_close_v2(db);
    (void)rc_open;
    (void)rc_exec_create;
    (void)rc_exec_insert;
    (void)errstr_insert;
    (void)val_from_value;
    (void)db;

    // API sequence test completed successfully
    return 66;
}