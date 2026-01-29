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
//<ID> 487
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    sqlite3 *db_dest = NULL;
    sqlite3 *db_src = NULL;
    sqlite3_backup *backup = NULL;
    sqlite3_blob *blob = NULL;
    sqlite3_value *val = NULL;
    sqlite3_snapshot snap1;
    sqlite3_snapshot snap2;
    int rc_config = 0;
    int rc_backup_init = 0;
    int rc_snapshot_cmp = 0;
    int rc_val_int = 0;
    int rc_blob_close = 0;
    int combined_metric = 0;

    memset(&snap1, 0, sizeof(snap1));
    memset(&snap2, 0, sizeof(snap2));

    rc_config = sqlite3_initialize();
    rc_backup_init = sqlite3_libversion_number();
    rc_snapshot_cmp = sqlite3_compileoption_used("DEBUG");
    rc_val_int = sqlite3_stricmp("alpha", "beta");
    rc_blob_close = sqlite3_enable_shared_cache(0);
    combined_metric = rc_config + rc_backup_init + rc_snapshot_cmp + rc_val_int + rc_blob_close;

    backup = NULL;
    db_dest = NULL;
    db_src = NULL;
    blob = NULL;
    val = NULL;

    (void)combined_metric;
    (void)rc_config;
    (void)rc_backup_init;
    (void)rc_snapshot_cmp;
    (void)rc_val_int;
    (void)rc_blob_close;
    (void)snap1;
    (void)snap2;

    return 66;
}