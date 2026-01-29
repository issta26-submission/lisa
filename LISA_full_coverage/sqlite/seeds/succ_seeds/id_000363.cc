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
//<ID> 363
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
    int rc_open = 0;
    int rc_auto = 0;
    int rc_blob_open = 0;
    int str_len = 0;

    // step 2: Setup - open an in-memory database and register (noop) auto-extension
    rc_open = sqlite3_open_v2(":memory:", &db, 0, NULL);
    rc_auto = sqlite3_auto_extension(NULL);

    // step 3: Core operations - create a string context, append text, measure it, attempt a blob open, and capture any UTF-16 error message
    str = sqlite3_str_new(db);
    const char *text = "example table/data for blob_open";
    str_len = (int)(sizeof("example table/data for blob_open") - 1);
    sqlite3_str_append(str, text, str_len);
    rc_blob_open = sqlite3_blob_open(db, "main", "t", "data", (sqlite3_int64)1, 0, &blob);
    errmsg16 = sqlite3_errmsg16(db);

    // step 4: Cleanup - free allocated string structure and close database (blob handle is not closed because blob_open was attempted without creating the table/row)
    sqlite3_free((void *)str);
    sqlite3_close(db);

    (void)blob;
    (void)errmsg16;
    (void)rc_open;
    (void)rc_auto;
    (void)rc_blob_open;
    (void)str_len;
    // API sequence test completed successfully
    return 66;
}