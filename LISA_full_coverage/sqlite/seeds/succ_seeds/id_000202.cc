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
//<ID> 202
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    // step 1: Declarations and open database
    sqlite3 *db = NULL;
    int rc_open = sqlite3_open(":memory:", &db);
    const char *sql_create = "CREATE TABLE t(id INTEGER PRIMARY KEY, val INTEGER);";
    const char *sql_insert = "INSERT INTO t(val) VALUES(42);";

    // step 2: Configure - create table and enable extended result codes
    int rc_exec_create = sqlite3_exec(db, sql_create, NULL, NULL, NULL);
    int rc_exec_insert = sqlite3_exec(db, sql_insert, NULL, NULL, NULL);
    int rc_ext_codes = sqlite3_extended_result_codes(db, 1);

    // step 3: Operate and validate - check total changes, release memory, probe error text
    sqlite3_int64 total_changes = sqlite3_total_changes64(db);
    int rc_release = sqlite3_db_release_memory(db);
    const char *errmsg = sqlite3_errmsg(db);

    // step 4: Cleanup
    int rc_close = sqlite3_close(db);

    (void)rc_open;
    (void)rc_exec_create;
    (void)rc_exec_insert;
    (void)rc_ext_codes;
    (void)total_changes;
    (void)rc_release;
    (void)errmsg;
    (void)rc_close;
    (void)db;

    // API sequence test completed successfully
    return 66;
}