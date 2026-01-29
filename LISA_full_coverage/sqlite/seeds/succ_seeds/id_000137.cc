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
//<ID> 137
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    sqlite3 *db = NULL;
    int rc_open = sqlite3_open(":memory:", &db);
    int cur = 0;
    int hi = 0;
    int rc_status = sqlite3_status(0, &cur, &hi, 1);
    int rc_db_status = sqlite3_db_status(db, 0, &cur, &hi, 1);
    int autocommit = sqlite3_get_autocommit(db);
    sqlite3_int64 mem_used = sqlite3_memory_used();
    int rc_ext_codes = sqlite3_extended_result_codes(db, 1);
    int changes = sqlite3_changes(db);
    int rc_wal_autocp = sqlite3_wal_autocheckpoint(db, 100);
    int rc_wal_ckpt = sqlite3_wal_checkpoint(db, "main");
    int rc_cacheflush = sqlite3_db_cacheflush(db);
    int rc_release = sqlite3_db_release_memory(db);
    int rc_close = sqlite3_close_v2(db);
    (void)rc_open;
    (void)rc_status;
    (void)rc_db_status;
    (void)autocommit;
    (void)mem_used;
    (void)rc_ext_codes;
    (void)changes;
    (void)rc_wal_autocp;
    (void)rc_wal_ckpt;
    (void)rc_cacheflush;
    (void)rc_release;
    (void)rc_close;
    return 66;
}