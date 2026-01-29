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
//<ID> 367
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    // step 1: Declarations
    sqlite3 *db = NULL;
    sqlite3_str *str = NULL;
    sqlite3_blob *blob = NULL;
    const void *errmsg16 = NULL;
    int rc_initialize = 0;
    int rc_open = 0;
    int rc_auto = 0;
    int rc_blob = 0;
    int rc_cancel = 0;
    int str_len = 0;

    // step 2: Initialize
    rc_initialize = sqlite3_initialize();
    rc_open = sqlite3_open_v2(":memory:", &db, 0, NULL);

    // step 3: Configure
    rc_auto = sqlite3_auto_extension((void(*)())sqlite3_shutdown);
    str = sqlite3_str_new(db);
    sqlite3_str_appendall(str, "CREATE TABLE demo(id INTEGER PRIMARY KEY, data BLOB);");
    str_len = sqlite3_str_length(str);

    // step 4: Operate, validate and cleanup
    rc_blob = sqlite3_blob_open(db, "main", "demo", "data", (sqlite3_int64)1, 0, &blob);
    errmsg16 = sqlite3_errmsg16(db);
    rc_cancel = sqlite3_cancel_auto_extension((void(*)())sqlite3_shutdown);
    sqlite3_close(db);
    sqlite3_shutdown();

    (void)rc_initialize;
    (void)rc_open;
    (void)rc_auto;
    (void)str_len;
    (void)rc_blob;
    (void)errmsg16;
    (void)rc_cancel;
    (void)blob;
    (void)str;
    // API sequence test completed successfully
    return 66;
}