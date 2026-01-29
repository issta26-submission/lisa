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
//<ID> 365
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    // step 1: Declarations
    sqlite3 *db = NULL;
    sqlite3_blob *blob = NULL;
    sqlite3_str *s = NULL;
    const void *errmsg16 = NULL;
    int rc_init = 0;
    int rc_open = 0;
    int rc_auto_ext = 0;
    int rc_blob_open = 0;
    int rc_close = 0;
    const char *db_uri = ":memory:";
    const char *table = "t";
    const char *col = "data";
    sqlite3_int64 rowid = 1;
    int flags = 0;

    // step 2: Initialize and configure
    rc_init = sqlite3_initialize();
    rc_auto_ext = sqlite3_auto_extension((void (*)())NULL);
    rc_open = sqlite3_open_v2(db_uri, &db, flags, NULL);
    s = sqlite3_str_new(db);
    sqlite3_str_append(s, "init", 4);

    // step 3: Operate and validate
    rc_blob_open = sqlite3_blob_open(db, "main", table, col, rowid, 0, &blob);
    errmsg16 = sqlite3_errmsg16(db);
    (void)sqlite3_str_length(s);

    // step 4: Cleanup
    rc_close = sqlite3_close(db);

    (void)rc_init;
    (void)rc_open;
    (void)rc_auto_ext;
    (void)rc_blob_open;
    (void)rc_close;
    (void)blob;
    (void)s;
    (void)errmsg16;
    (void)db_uri;
    (void)table;
    (void)col;
    (void)rowid;
    (void)flags;
    // API sequence test completed successfully
    return 66;
}