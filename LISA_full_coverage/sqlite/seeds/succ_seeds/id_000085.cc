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
//<ID> 85
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
    const char *pzTail = NULL;
    char *expanded_sql = NULL;
    int rc_open = 0;
    int rc_prep = 0;
    int rc_step = 0;
    int rc_blob = 0;
    int opt_used = 0;

    // step 2: Configure
    rc_open = sqlite3_open(":memory:", &db);
    opt_used = sqlite3_compileoption_used("THREADSAFE");

    // step 3: Operate / Validate
    rc_prep = sqlite3_prepare_v2(db, "CREATE TABLE t(a BLOB);", -1, &stmt, &pzTail);
    rc_step = sqlite3_step(stmt);
    sqlite3_reset(stmt);

    rc_prep = sqlite3_prepare_v2(db, "INSERT INTO t(a) VALUES(x'010203');", -1, &stmt, &pzTail);
    rc_step = sqlite3_step(stmt);
    sqlite3_reset(stmt);

    rc_prep = sqlite3_prepare_v2(db, "SELECT a FROM t WHERE rowid=1;", -1, &stmt, &pzTail);
    expanded_sql = sqlite3_expanded_sql(stmt);

    rc_blob = sqlite3_blob_open(db, "main", "t", "a", 1, 0, &blob);
    rc_blob = sqlite3_blob_close(blob);

    sqlite3_realloc(expanded_sql, 0);

    // step 4: Cleanup
    sqlite3_close_v2(db);

    (void)rc_open;
    (void)rc_prep;
    (void)rc_step;
    (void)rc_blob;
    (void)opt_used;
    (void)pzTail;
    (void)stmt;
    (void)blob;

    // API sequence test completed successfully
    return 66;
}