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
//<ID> 98
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    // step 1: Declarations / Initialize
    sqlite3 *db = NULL;
    int rc_open = 0;
    int rc_exec_create = 0;
    int rc_exec_insert = 0;
    int rc_close = 0;
    sqlite3_int64 val64 = 0;
    const char *errstr = NULL;

    // step 2: Open in-memory database
    rc_open = sqlite3_open(":memory:", &db);

    // step 3: Operate / Validate (use APIs that won't dereference NULL sqlite3_value)
    rc_exec_create = sqlite3_exec(db, "CREATE TABLE t(a INTEGER);", NULL, NULL, NULL);
    rc_exec_insert = sqlite3_exec(db, "INSERT INTO t VALUES(42);", NULL, NULL, NULL);
    val64 = sqlite3_total_changes64(db);
    errstr = sqlite3_errstr(rc_exec_insert);

    // step 4: Cleanup
    rc_close = sqlite3_close_v2(db);
    (void)rc_open;
    (void)rc_exec_create;
    (void)rc_exec_insert;
    (void)val64;
    (void)errstr;
    (void)rc_close;

    // API sequence test completed successfully
    return 66;
}