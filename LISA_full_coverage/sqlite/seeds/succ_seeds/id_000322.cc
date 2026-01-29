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
//<ID> 322
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    // step 1: Declarations & Initialize
    sqlite3 *db = NULL;
    sqlite3_snapshot snapshot;
    void *mem = NULL;
    char **table = NULL;
    const void *filename = (const void *)L":memory:";
    sqlite3_int64 lastrowid = 12345;
    int rc_open = 0;
    int rc_snapshot = 0;
    int rc_errcode = 0;

    rc_open = sqlite3_open16(filename, &db);

    // step 2: Configure
    sqlite3_extended_result_codes(db, 1);
    sqlite3_set_last_insert_rowid(db, lastrowid);
    mem = sqlite3_realloc(NULL, 128);

    // step 3: Operate & Validate
    mem = sqlite3_realloc(mem, 256);
    rc_snapshot = sqlite3_snapshot_open(db, "main", &snapshot);
    rc_errcode = sqlite3_errcode(db);
    sqlite3_free_table(table);

    // step 4: Cleanup
    mem = sqlite3_realloc(mem, 0);
    sqlite3_close(db);

    (void)rc_open;
    (void)rc_snapshot;
    (void)rc_errcode;
    (void)mem;
    (void)table;
    (void)filename;
    (void)lastrowid;
    (void)snapshot;
    // API sequence test completed successfully
    return 66;
}