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
//<ID> 428
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
    const char *sql = "SELECT 1;";
    int rc_prep = 0;
    int rc_step = 0;
    int rc_dbconf = 0;
    int rc_backup = 0;
    int rc_ext = 0;
    int rc_bfinish = 0;
    int rc_stmt_reset = 0;

    // step 2: Initialize / Configure
    rc_ext = sqlite3_extended_result_codes(db, 1);
    rc_prep = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);

    // step 3: Operate / Validate
    rc_step = sqlite3_step(stmt);
    rc_backup = sqlite3_backup_step(backup, 5);
    rc_dbconf = sqlite3_db_config(db, 0);

    // step 4: Cleanup
    rc_stmt_reset = (stmt ? (int)( (void)(sqlite3_stmt_scanstatus_reset(stmt)), 0 ) : 0);
    rc_bfinish = sqlite3_backup_finish(backup);
    sqlite3_close_v2(db);
    db = NULL;
    stmt = NULL;
    backup = NULL;

    (void)rc_prep;
    (void)rc_step;
    (void)rc_dbconf;
    (void)rc_backup;
    (void)rc_ext;
    (void)rc_bfinish;
    (void)rc_stmt_reset;
    (void)sql;

    // API sequence test completed successfully
    return 66;
}