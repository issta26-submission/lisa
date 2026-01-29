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
//<ID> 562
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    // step 1: Declarations / Initialize
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    sqlite3_blob *blob = NULL;
    sqlite3_str *builder = NULL;
    char *str_val = NULL;
    const char *text = "hello";
    sqlite3_uint64 text_len = 5;
    int rc_os_init = 0;
    int rc_threadsafe = 0;
    int rc_libver = 0;
    int rc_status = 0;
    void *raw_stmt = NULL;
    void *raw_blob = NULL;

    // step 2: Setup / Configure (use only APIs that do not require valid opaque objects)
    rc_os_init = sqlite3_os_init();
    str_val = sqlite3_mprintf("%s", text);
    sqlite3_free(str_val);
    rc_threadsafe = sqlite3_threadsafe();
    rc_libver = sqlite3_libversion_number();
    const char *srcid = sqlite3_sourceid();
    sqlite3_soft_heap_limit(1024);
    int cur = 0;
    int hi = 0;
    rc_status = sqlite3_status(0, &cur, &hi, 0);

    // allocate dummy opaque objects but do not call APIs that require them to be valid
    raw_stmt = malloc(1);
    memset(raw_stmt, 0, 1);
    stmt = (sqlite3_stmt *)raw_stmt;
    raw_blob = malloc(1);
    memset(raw_blob, 0, 1);
    blob = (sqlite3_blob *)raw_blob;

    // step 3: Cleanup
    free(raw_blob);
    free(raw_stmt);

    (void)db;
    (void)stmt;
    (void)blob;
    (void)builder;
    (void)text;
    (void)text_len;
    (void)rc_os_init;
    (void)rc_threadsafe;
    (void)rc_libver;
    (void)rc_status;
    (void)srcid;
    (void)cur;
    (void)hi;

    // API sequence test completed successfully
    return 66;
}