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
//<ID> 495
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    // step 1: Declarations
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    const char *sql = "SELECT 1";
    const char *pzTail = NULL;
    int rc_open = 0;
    int rc_prepare = 0;
    int rc_exec = 0;
    int rc_db_status = 0;
    int rc_close = 0;
    int cur = 0;
    int hi = 0;

    // step 2: Initialize / Configure
    rc_open = sqlite3_open(":memory:", &db);
    rc_prepare = sqlite3_prepare_v2(db, sql, -1, &stmt, &pzTail);
    rc_exec = sqlite3_exec(db, "CREATE TABLE t(x); INSERT INTO t(x) VALUES(1);", NULL, NULL, NULL);

    // step 3: Operate / Validate
    rc_db_status = sqlite3_db_status(db, 0, &cur, &hi, 0);

    // step 4: Cleanup / Finalize
    rc_close = sqlite3_close(db);

    (void)rc_open;
    (void)rc_prepare;
    (void)rc_exec;
    (void)rc_db_status;
    (void)rc_close;
    (void)pzTail;
    (void)sql;
    (void)stmt;
    (void)cur;
    (void)hi;

    // API sequence test completed successfully
    return 66;
}