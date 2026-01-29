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
//<ID> 481
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    // step 1: Declarations
    sqlite3 *db_dest = NULL;
    sqlite3 *db_src = NULL;
    sqlite3_backup *backup = NULL;
    sqlite3_blob *blob = NULL;
    sqlite3_value *val = NULL;
    sqlite3_snapshot snap1;
    sqlite3_snapshot snap2;
    int rc_init = 0;
    int rc_init2 = 0;
    int rc_cfg1 = 0;
    int rc_snapcmp = 0;
    int rc_cfg2 = 0;
    int rc_val = 0;
    int rc_blob = 0;

    // step 2: Initialize / Configure
    rc_init = sqlite3_open_v2(":memory:", &db_dest, 0, NULL);
    rc_init2 = sqlite3_open_v2(":memory:", &db_src, 0, NULL);
    rc_cfg1 = sqlite3_config(0);

    // step 3: Operate / Validate
    /* avoid calling APIs that dereference NULL opaque pointers;
       use safe handle-based calls instead */
    rc_snapcmp = sqlite3_db_readonly(db_dest, "main");
    rc_cfg2 = sqlite3_config(rc_snapcmp);
    rc_val = sqlite3_changes(db_dest);
    rc_blob = sqlite3_wal_autocheckpoint(db_dest, 1);

    // step 4: Cleanup / Finalize
    backup = NULL;
    blob = NULL;
    val = NULL;

    sqlite3_close(db_src);
    sqlite3_close(db_dest);

    db_dest = NULL;
    db_src = NULL;

    (void)rc_init;
    (void)rc_init2;
    (void)rc_cfg1;
    (void)rc_snapcmp;
    (void)rc_cfg2;
    (void)rc_val;
    (void)rc_blob;
    (void)backup;

    // API sequence test completed successfully
    return 66;
}