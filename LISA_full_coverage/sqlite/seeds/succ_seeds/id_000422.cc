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
//<ID> 422
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
    const char *pzTail = NULL;
    const char *sql_select = "SELECT 42;";
    int rc_open = 0;
    int rc_prepare = 0;
    int rc_step = 0;
    int rc_db_config = 0;
    int rc_extcodes = 0;
    int rc_backup_step = 0;
    int rc_backup_finish = 0;
    int rc_finalize = 0;
    int rc_close = 0;

    // step 2: Setup - open an in-memory database and enable extended result codes and a db config option
    rc_open = sqlite3_open(":memory:", &db);
    rc_extcodes = sqlite3_extended_result_codes(db, 1);
    rc_db_config = sqlite3_db_config(db, 1);

    // step 3: Operate - prepare and step a simple statement, then perform a backup step and finish it
    rc_prepare = sqlite3_prepare_v2(db, sql_select, -1, &stmt, &pzTail);
    rc_step = sqlite3_step(stmt);
    backup = sqlite3_backup_init(db, "main", db, "main");
    rc_backup_step = sqlite3_backup_step(backup, 1);
    rc_backup_finish = sqlite3_backup_finish(backup);
    backup = NULL;

    // step 4: Cleanup - finalize statement and close database
    rc_finalize = sqlite3_finalize(stmt);
    stmt = NULL;
    rc_close = sqlite3_close_v2(db);
    db = NULL;

    (void)rc_open;
    (void)rc_prepare;
    (void)rc_step;
    (void)rc_db_config;
    (void)rc_extcodes;
    (void)rc_backup_step;
    (void)rc_backup_finish;
    (void)rc_finalize;
    (void)rc_close;
    (void)pzTail;
    (void)sql_select;

    // API sequence test completed successfully
    return 66;
}