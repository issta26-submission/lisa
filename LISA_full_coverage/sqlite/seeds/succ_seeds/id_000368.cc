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
//<ID> 368
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
    sqlite3_str *str = NULL;
    const void *errmsg16 = NULL;
    int rc_open = 0;
    int rc_auto = 0;
    int rc_blob = 0;
    int rc_close = 0;

    // step 2: Setup - open an in-memory database and register a (NULL) auto-extension entry point
    rc_open = sqlite3_open_v2(":memory:", &db, 0, NULL);
    rc_auto = sqlite3_auto_extension((void (*)())NULL);

    // step 3: Core operations - allocate a sqlite3_str tied to the DB, attempt to open a blob on a non-existent table to produce an error, and fetch the UTF-16 error message
    str = sqlite3_str_new(db);
    rc_blob = sqlite3_blob_open(db, "main", "no_table", "data", (sqlite3_int64)1, 0, &blob);
    errmsg16 = sqlite3_errmsg16(db);

    // step 4: Cleanup - free the string buffer and close the database
    sqlite3_free(str);
    rc_close = sqlite3_close(db);

    (void)blob;
    (void)errmsg16;
    (void)rc_open;
    (void)rc_auto;
    (void)rc_blob;
    (void)rc_close;
    // API sequence test completed successfully
    return 66;
}