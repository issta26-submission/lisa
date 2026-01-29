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
//<ID> 166
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    // step 1: Initialize
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    int rc_os_init = sqlite3_os_init();
    int rc_open = sqlite3_open_v2(":memory:", &db, 0, NULL);

    // step 2: Configure
    int rc_ext_codes = sqlite3_extended_result_codes(db, 1);
    const char *compile_opt0 = sqlite3_compileoption_get(0);

    // step 3: Operate / Validate
    const char *sql = "CREATE TABLE IF NOT EXISTS t(id INTEGER PRIMARY KEY, val TEXT); SELECT 1;";
    int rc_prepare = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);

    // step 4: Cleanup
    int rc_close = sqlite3_close_v2(db);
    (void)rc_os_init;
    (void)rc_open;
    (void)rc_ext_codes;
    (void)compile_opt0;
    (void)rc_prepare;
    (void)stmt;
    (void)rc_close;

    // API sequence test completed successfully
    return 66;
}