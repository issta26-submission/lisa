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
//<ID> 168
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    // step 1: Initialize
    int rc_init = sqlite3_os_init();
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    const char *sql = "CREATE TABLE IF NOT EXISTS demo(id INTEGER PRIMARY KEY, val TEXT);";
    const char *tail = NULL;
    const char *compile_opt = NULL;

    // step 2: Configure and Operate
    int rc_open = sqlite3_open_v2(":memory:", &db, 0, NULL);
    int rc_ext_on = sqlite3_extended_result_codes(db, 1);
    int rc_prep = sqlite3_prepare_v2(db, sql, -1, &stmt, &tail);
    compile_opt = sqlite3_compileoption_get(0);

    // step 3: Validate
    sqlite3_log(0, compile_opt ? compile_opt : "no_compile_option");
    (void)tail;
    (void)stmt;

    // step 4: Cleanup
    int rc_close = sqlite3_close_v2(db);
    (void)rc_init;
    (void)rc_open;
    (void)rc_ext_on;
    (void)rc_prep;
    (void)compile_opt;
    (void)rc_close;

    // API sequence test completed successfully
    return 66;
}