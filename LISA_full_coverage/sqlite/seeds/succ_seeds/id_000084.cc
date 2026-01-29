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
//<ID> 84
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
    char *expanded = NULL;
    int rc_open = 0;
    int rc_prep = 0;
    int rc_step = 0;
    int rc_bind = 0;
    int rc_blob_open = 0;
    int rc_blob_write = 0;
    int rc_blob_close = 0;
    int rc_close = 0;
    int opt_used = 0;

    // step 2: Configure
    rc_open = sqlite3_open(":memory:", &db);

    // step 3: Operate / Validate
    rc_prep = sqlite3_prepare_v2(db, "CREATE TABLE t(a BLOB);", -1, &stmt, &pzTail);
    rc_step = sqlite3_step(stmt);
    rc_prep = sqlite3_prepare_v2(db, "INSERT INTO t(a) VALUES(?);", -1, &stmt, &pzTail);
    rc_bind = sqlite3_bind_blob(stmt, 1, "X", 1, NULL);
    rc_step = sqlite3_step(stmt);
    rc_blob_open = sqlite3_blob_open(db, "main", "t", "a", 1, 1, &blob);
    rc_blob_write = sqlite3_blob_write(blob, "Y", 1, 0);
    rc_blob_close = sqlite3_blob_close(blob);
    rc_prep = sqlite3_prepare_v2(db, "SELECT a FROM t WHERE rowid=1;", -1, &stmt, &pzTail);
    expanded = sqlite3_expanded_sql(stmt);
    opt_used = sqlite3_compileoption_used("ENABLE_FTS5");

    // step 4: Cleanup
    rc_close = sqlite3_close_v2(db);
    (void)rc_open;
    (void)rc_prep;
    (void)rc_step;
    (void)rc_bind;
    (void)rc_blob_open;
    (void)rc_blob_write;
    (void)rc_blob_close;
    (void)rc_close;
    (void)expanded;
    (void)opt_used;
    (void)stmt;
    (void)pzTail;
    (void)blob;
    // API sequence test completed successfully
    return 66;
}