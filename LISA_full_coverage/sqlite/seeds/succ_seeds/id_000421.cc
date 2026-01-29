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
//<ID> 421
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
    int op = 1;
    int enable = 1;
    int rc_ext_enable = 0;
    int rc_dbconf = 0;
    int pages = 0;
    int rc_step = 0;
    int rc_backup = 0;
    int rc_ext_disable = 0;

    // step 2: Initialize / Configure
    rc_ext_enable = sqlite3_extended_result_codes(db, enable);
    rc_dbconf = sqlite3_db_config(db, op);
    pages = rc_ext_enable + rc_dbconf + 1;

    // step 3: Operate / Validate
    rc_step = sqlite3_step(stmt);
    rc_backup = sqlite3_backup_step(backup, pages);

    // step 4: Cleanup
    rc_ext_disable = sqlite3_extended_result_codes(db, 0);
    db = NULL;
    stmt = NULL;
    backup = NULL;

    (void)op;
    (void)enable;
    (void)rc_ext_enable;
    (void)rc_dbconf;
    (void)pages;
    (void)rc_step;
    (void)rc_backup;
    (void)rc_ext_disable;

    // API sequence test completed successfully
    return 66;
}