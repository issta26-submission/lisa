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
//<ID> 161
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    // step 1: Declarations / Initialize
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt1 = NULL;
    sqlite3_stmt *stmt2 = NULL;
    const char *sql = "CREATE TABLE t(a INTEGER); INSERT INTO t(a) VALUES(42); SELECT a FROM t;";
    const char *pzTail = NULL;
    int rc_os_init = sqlite3_os_init();
    int rc_open = sqlite3_open_v2(":memory:", &db, 0, NULL);

    // step 2: Configure - enable extended result codes on the opened DB
    int rc_ext_on = sqlite3_extended_result_codes(db, 1);

    // step 3: Operate / Validate - prepare the first statement and then prepare the next using the tail;
    // also query a compile option and log it to tie data flow between calls
    int rc_p1 = sqlite3_prepare_v2(db, sql, -1, &stmt1, &pzTail);
    int rc_p2 = sqlite3_prepare_v2(db, pzTail, -1, &stmt2, &pzTail);
    const char *opt0 = sqlite3_compileoption_get(0);
    sqlite3_log(0, opt0 ? opt0 : "no_compile_option");

    // step 4: Cleanup - close the DB handle (statements left non-finalized on purpose for this sequence)
    int rc_close = sqlite3_close_v2(db);

    // silence unused-variable warnings while preserving meaningful data flow above
    (void)stmt1;
    (void)stmt2;
    (void)pzTail;
    (void)rc_os_init;
    (void)rc_open;
    (void)rc_ext_on;
    (void)rc_p1;
    (void)rc_p2;
    (void)opt0;
    (void)rc_close;

    // API sequence test completed successfully
    return 66;
}