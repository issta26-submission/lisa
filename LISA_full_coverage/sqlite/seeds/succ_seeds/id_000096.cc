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
//<ID> 96
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
    int rc_exec = 0;
    const char *sql_create = "CREATE TABLE IF NOT EXISTS t(x INTEGER); INSERT INTO t(x) VALUES(42);";
    char *errmsg = NULL;
    const char *errstr = NULL;
    sqlite3_int64 val_int64 = 0;

    rc_open = sqlite3_open(":memory:", &db);

    // step 2: Configure
    sqlite3_progress_handler(db, 1000, NULL, NULL);

    // step 3: Operate / Validate
    rc_exec = sqlite3_exec(db, sql_create, NULL, NULL, &errmsg);
    errstr = sqlite3_errstr(rc_exec);
    val_int64 = sqlite3_last_insert_rowid(db);

    // step 4: Cleanup
    sqlite3_interrupt(db);
    sqlite3_db_release_memory(db);

    (void)rc_open;
    (void)rc_exec;
    (void)errmsg;
    (void)errstr;
    (void)val_int64;
    (void)sql_create;
    (void)db;

    // API sequence test completed successfully
    return 66;
}