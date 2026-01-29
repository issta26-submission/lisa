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
//<ID> 165
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    // step 1: Initialize environment and declarations
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    const char *sql = "SELECT sqlite_version();";
    const char *opt0 = NULL;
    int rc_os_init = sqlite3_os_init();
    int rc_open = sqlite3_open_v2(":memory:", &db, 0, NULL);

    // step 2: Configure database handle
    int rc_ext = sqlite3_extended_result_codes(db, 1);

    // step 3: Operate - prepare a statement and read a compile option
    int rc_prepare = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    opt0 = sqlite3_compileoption_get(0);

    // step 4: Validate / Cleanup - close DB and silence unused variables
    int rc_close = sqlite3_close_v2(db);
    (void)stmt;
    (void)opt0;
    (void)rc_os_init;
    (void)rc_open;
    (void)rc_ext;
    (void)rc_prepare;
    (void)rc_close;

    // API sequence test completed successfully
    return 66;
}