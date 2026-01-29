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
//<ID> 409
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    // step 1: Declarations
    sqlite3 *db = NULL;
    void *buf = NULL;
    void *buf2 = NULL;
    const void *filename = (const void *)L":memory:";
    sqlite3_int64 lastid = (sqlite3_int64)987654321LL;
    int rc_open = 0;
    int txn_state = 0;
    int relmem_rc = 0;
    int size1 = 128;
    int size2 = 256;

    // step 2: Initialize
    rc_open = sqlite3_open16(filename, &db);
    sqlite3_set_last_insert_rowid(db, lastid);

    // step 3: Configure / Operate / Validate
    txn_state = sqlite3_txn_state(db, "main");
    buf = sqlite3_realloc(NULL, size1);
    memset(buf, 0, size1);
    buf2 = sqlite3_realloc(buf, size2);
    relmem_rc = sqlite3_release_memory(1024);
    sqlite3_set_last_insert_rowid(db, lastid + 1);

    // step 4: Cleanup
    buf2 = sqlite3_realloc(buf2, 0);
    sqlite3_close_v2(db);
    db = NULL;

    (void)rc_open;
    (void)txn_state;
    (void)relmem_rc;
    (void)buf;
    (void)buf2;
    (void)lastid;
    (void)size1;
    (void)size2;

    // API sequence test completed successfully
    return 66;
}