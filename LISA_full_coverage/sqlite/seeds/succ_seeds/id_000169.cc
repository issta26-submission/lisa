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
//<ID> 169
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
    const char *sql = "CREATE TABLE IF NOT EXISTS t(id INTEGER); INSERT INTO t(id) VALUES(1);";
    const char *pzTail = NULL;
    const char *compile_opt0 = NULL;
    int rc_os_init = 0;
    int rc_open = 0;
    int rc_ext = 0;
    int rc_prepare = 0;
    int rc_close = 0;

    // step 2: Initialize sqlite3 OS layer and open an in-memory database
    rc_os_init = sqlite3_os_init();
    rc_open = sqlite3_open_v2(":memory:", &db, 0, NULL);

    // step 3: Configure extended result codes and prepare a statement
    rc_ext = sqlite3_extended_result_codes(db, 1);
    rc_prepare = sqlite3_prepare_v2(db, sql, -1, &stmt, &pzTail);

    // step 4: Validate via compile option inspection and cleanup handles
    compile_opt0 = sqlite3_compileoption_get(0);
    sqlite3_log(0, compile_opt0 ? compile_opt0 : "no-compile-opt");
    rc_close = sqlite3_close_v2(db);

    (void)rc_os_init;
    (void)rc_open;
    (void)rc_ext;
    (void)rc_prepare;
    (void)pzTail;
    (void)stmt;
    (void)compile_opt0;
    (void)rc_close;

    // API sequence test completed successfully
    return 66;
}