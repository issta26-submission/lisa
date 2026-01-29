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
//<ID> 162
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
    const char *sql = "CREATE TABLE t(id INTEGER PRIMARY KEY, val TEXT);";
    int rc_os = sqlite3_os_init();

    // step 2: Setup - open in-memory database and enable extended result codes
    int rc_open = sqlite3_open_v2(":memory:", &db, 0, NULL);
    int rc_ext = sqlite3_extended_result_codes(db, 1);

    // step 3: Operate / Validate - prepare a statement and inspect a compile option
    int rc_prepare = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    const char *opt0 = sqlite3_compileoption_get(0);

    // step 4: Cleanup
    int rc_close = sqlite3_close_v2(db);
    (void)rc_os;
    (void)rc_open;
    (void)rc_ext;
    (void)rc_prepare;
    (void)stmt;
    (void)opt0;
    (void)rc_close;

    // API sequence test completed successfully
    return 66;
}