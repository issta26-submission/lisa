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
//<ID> 12
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    // Allocate opaque objects using sqlite3_malloc64. Use fixed sizes since
    // the actual structures are incomplete here.
    sqlite3 *db = static_cast<sqlite3*>(sqlite3_malloc64((sqlite3_uint64)128));
    sqlite3_value *val = static_cast<sqlite3_value*>(sqlite3_malloc64((sqlite3_uint64)64));
    sqlite3_index_info *idx = static_cast<sqlite3_index_info*>(sqlite3_malloc64((sqlite3_uint64)128));

    // Use sqlite3_value_double on the allocated sqlite3_value pointer.
    // If val is NULL the call is skipped.
    double d = 0.0;
    if (val) {
        d = sqlite3_value_double(val);
    }

    // Call sqlite3_wal_checkpoint with the allocated (opaque) sqlite3 pointer.
    // Use "main" as the database name string.
    int rc = sqlite3_wal_checkpoint(db, "main");

    // Retrieve a collation name from the (opaque) sqlite3_index_info pointer.
    const char *coll = sqlite3_vtab_collation(idx, 0);

    // Use the returned values in trivial ways to avoid unused-variable warnings.
    (void)d;
    (void)rc;
    if (coll) {
        (void)coll[0];
    }

    return 66;
}