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
//<ID> 324
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    sqlite3 *db = NULL;
    sqlite3_snapshot *snap = NULL;
    void *buffer = NULL;
    char **result = NULL;
    int rc_init = 0;
    int rc_open = 0;
    int rc_snap = 0;
    sqlite3_int64 changes = 0;
    sqlite3_int64 total_changes = 0;

    rc_init = sqlite3_initialize();
    rc_open = sqlite3_open(":memory:", &db);
    buffer = malloc(64);
    ((char *)buffer)[0] = '\0';
    rc_snap = sqlite3_snapshot_get(db, "main", &snap);
    changes = sqlite3_changes64(db);
    total_changes = sqlite3_total_changes64(db);
    sqlite3_free_table(result);
    free(buffer);
    sqlite3_interrupt(db);
    sqlite3_os_end();

    (void)rc_init;
    (void)rc_open;
    (void)rc_snap;
    (void)changes;
    (void)total_changes;
    (void)result;

    return 66;
}