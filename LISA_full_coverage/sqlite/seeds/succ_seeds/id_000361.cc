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
//<ID> 361
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    // step 1: Declarations & Initialize
    sqlite3 *db = NULL;
    sqlite3_str *str = NULL;
    sqlite3_blob *blob = NULL;
    const void *errmsg16 = NULL;
    int rc_open = 0;
    int rc_auto = 0;
    int rc_blob_open = 0;
    int str_len = 0;
    sqlite3_int64 target_row = (sqlite3_int64)1;

    rc_open = sqlite3_open_v2(":memory:", &db, 0, NULL);

    // step 2: Configure (register an auto-extension and create a string builder)
    rc_auto = sqlite3_auto_extension((void(*)(void))sqlite3_initialize);
    str = sqlite3_str_new(db);
    sqlite3_str_appendall(str, "constructed-string"); /* use appendall to avoid negative length */
    str_len = sqlite3_str_length(str);

    // step 3: Operate (attempt to open a blob and capture any UTF-16 error message)
    rc_blob_open = sqlite3_blob_open(db, "main", "mytable", "data", target_row, 0, &blob);
    errmsg16 = sqlite3_errmsg16(db);

    // step 4: Validate & Cleanup (reset auto-extensions, free resources, close DB)
    sqlite3_reset_auto_extension();
    sqlite3_free((void *)str);
    sqlite3_close(db);

    (void)rc_open;
    (void)rc_auto;
    (void)rc_blob_open;
    (void)str_len;
    (void)blob;
    (void)errmsg16;
    (void)target_row;
    // API sequence test completed successfully
    return 66;
}