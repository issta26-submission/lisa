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
//<ID> 164
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
    const char *sql_setup = "CREATE TABLE t(id INTEGER PRIMARY KEY); INSERT INTO t(id) VALUES(1);";
    const char *opt0 = NULL;
    int rc_os = 0;
    int rc_open = 0;
    int rc_ext = 0;
    int rc_prep = 0;
    int rc_close = 0;

    // step 2: Initialize SQLite OS layer and open an in-memory database
    rc_os = sqlite3_os_init();
    rc_open = sqlite3_open_v2(":memory:", &db, 0, NULL);

    // step 3: Configure database handle and prepare a statement
    rc_ext = sqlite3_extended_result_codes(db, 1);
    rc_prep = sqlite3_prepare_v2(db, sql_setup, -1, &stmt, NULL);
    opt0 = sqlite3_compileoption_get(0);
    sqlite3_log(0, opt0 ? opt0 : "compileoption-none");

    // step 4: Cleanup
    rc_close = sqlite3_close_v2(db);

    (void)stmt;
    (void)rc_os;
    (void)rc_open;
    (void)rc_ext;
    (void)rc_prep;
    (void)rc_close;
    (void)opt0;

    // API sequence test completed successfully
    return 66;
}