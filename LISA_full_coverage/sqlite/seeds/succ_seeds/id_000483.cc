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
//<ID> 483
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    // step 1: Declarations
    sqlite3 *db_dest = NULL;
    sqlite3 *db_src = NULL;
    sqlite3_backup *backup = NULL;
    sqlite3_blob *blob = NULL;
    sqlite3_value *val = NULL;
    sqlite3_snapshot snap1;
    sqlite3_snapshot snap2;
    const char *ver = NULL;
    const char *srcid = NULL;
    int vernum = 0;
    int threadsafe = 0;
    const char *errstr = NULL;

    memset(&snap1, 0, sizeof(snap1));
    memset(&snap2, 0, sizeof(snap2));

    // step 2: Initialize / Configure (safe calls that don't require non-NULL opaque pointers)
    sqlite3_reset_auto_extension();

    // step 3: Operate / Validate (use library-wide queries that are safe with no DB handle)
    ver = sqlite3_libversion();
    srcid = sqlite3_sourceid();
    vernum = sqlite3_libversion_number();
    threadsafe = sqlite3_threadsafe();
    errstr = sqlite3_errstr(0);

    // step 4: Cleanup / Finalize (clear local handles; nothing to free)
    backup = NULL;
    blob = NULL;
    val = NULL;
    db_dest = NULL;
    db_src = NULL;

    (void)ver;
    (void)srcid;
    (void)vernum;
    (void)threadsafe;
    (void)errstr;

    // API sequence test completed successfully
    return 66;
}