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
//<ID> 160
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    // step 1: Declarations / Initialize
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    const char *sql = "CREATE TABLE IF NOT EXISTS t(id INTEGER PRIMARY KEY, val TEXT);";
    const char *compile_opt = NULL;
    const char *last_errmsg = NULL;
    int rc_os = sqlite3_os_init();
    int rc_open = sqlite3_open_v2(":memory:", &db, 0, NULL);

    // step 2: Configure
    int rc_ext = sqlite3_extended_result_codes(db, 1);

    // step 3: Operate / Validate
    int rc_prepare = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    compile_opt = sqlite3_compileoption_get(0);
    last_errmsg = sqlite3_errmsg(db);

    // step 4: Cleanup
    int rc_close = sqlite3_close_v2(db);
    (void)rc_os;
    (void)rc_open;
    (void)rc_ext;
    (void)rc_prepare;
    (void)stmt;
    (void)compile_opt;
    (void)last_errmsg;
    (void)rc_close;

    // API sequence test completed successfully
    return 66;
}