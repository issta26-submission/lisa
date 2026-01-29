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
//<ID> 99
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

int test_sqlite3_api_sequence() {
    // step 1: Declarations / Initialize
    sqlite3 *db = NULL;
    char *errmsg = NULL;
    const char *create_sql = "CREATE TABLE t(id INTEGER PRIMARY KEY, val INTEGER);";
    const char *insert_sql = "INSERT INTO t(val) VALUES(123);";
    const char *select_sql = "SELECT val FROM t LIMIT 1;";
    int rc_open = sqlite3_open(":memory:", &db);

    // step 2: Configure / Execute statements
    int rc_exec_create = sqlite3_exec(db, create_sql, NULL, NULL, &errmsg);
    int rc_exec_insert = sqlite3_exec(db, insert_sql, NULL, NULL, &errmsg);
    const char *errstr_after_insert = sqlite3_errmsg(db);

    // step 3: Operate / Validate
    int rc_exec_select = sqlite3_exec(db, select_sql, NULL, NULL, &errmsg);
    sqlite3_int64 extracted_from_value_api = sqlite3_last_insert_rowid(db);
    sqlite3_int64 combined_check = extracted_from_value_api + (sqlite3_int64)rc_exec_select;
    (void)errstr_after_insert;
    (void)combined_check;

    // step 4: Cleanup
    int rc_exec_drop = sqlite3_exec(db, "DROP TABLE IF EXISTS t;", NULL, NULL, &errmsg);
    int rc_close = sqlite3_close(db);
    (void)rc_open;
    (void)rc_exec_create;
    (void)rc_exec_insert;
    (void)rc_exec_select;
    (void)rc_exec_drop;
    (void)rc_close;
    (void)errmsg;
    (void)db;

    // API sequence test completed successfully
    return 66;
}