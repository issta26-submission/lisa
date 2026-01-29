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
//<ID> 424
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
    sqlite3_backup *backup = NULL;
    int rc_ext = 0;
    int rc_dbconfig = 0;
    int rc_step = 0;
    int rc_backup = 0;
    int onoff = 1;
    int dbconfig_op = 1;

    // step 2: Initialize / Configure
    rc_ext = sqlite3_extended_result_codes(db, onoff);
    dbconfig_op = dbconfig_op + rc_ext;
    rc_dbconfig = sqlite3_db_config(db, dbconfig_op);

    // step 3: Operate / Validate
    rc_step = sqlite3_step(stmt);
    rc_backup = sqlite3_backup_step(backup, rc_step);

    // step 4: Cleanup
    sqlite3_close_v2(db);
    db = NULL;
    stmt = NULL;
    backup = NULL;

    (void)rc_ext;
    (void)rc_dbconfig;
    (void)rc_step;
    (void)rc_backup;
    (void)onoff;
    (void)dbconfig_op;

    // API sequence test completed successfully
    return 66;
}