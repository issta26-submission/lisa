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
//<ID> 226
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    /* Step 1: Declarations and initialize (open database) */
    sqlite3 *db = NULL;
    int rc_open = sqlite3_open(":memory:", &db);

    /* Step 2: Configure (prepare a simple CREATE TABLE statement) */
    sqlite3_stmt *stmt = NULL;
    const char *pzTail = NULL;
    int rc_prep = sqlite3_prepare_v2(db, "CREATE TABLE IF NOT EXISTS t(a INTEGER);", -1, &stmt, &pzTail);

    /* Step 3: Operate and validate (exercise APIs safely on the prepared statement and DB) */
    int readonly = sqlite3_stmt_readonly(stmt);
    int data_count = sqlite3_data_count(stmt);
    const char *sql_text = sqlite3_sql(stmt);
    const char *param_name = sqlite3_bind_parameter_name(stmt, 1);
    sqlite3_int64 last_rowid = sqlite3_last_insert_rowid(db);
    int total_changes = sqlite3_total_changes(db);
    const char *err_msg = sqlite3_errmsg(db);
    int mem_high = sqlite3_memory_highwater(0);
    int released = sqlite3_db_release_memory(db);

    /* Step 4: Cleanup */
    sqlite3_finalize(stmt);

    /* Use results in no-op casts to avoid unused-variable warnings */
    (void)rc_open;
    (void)rc_prep;
    (void)readonly;
    (void)data_count;
    (void)sql_text;
    (void)param_name;
    (void)last_rowid;
    (void)total_changes;
    (void)err_msg;
    (void)mem_high;
    (void)released;

    return 66;
}