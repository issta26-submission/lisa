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
//<ID> 81
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
    int rc_open = sqlite3_open(":memory:", &db);
    int opt_used = sqlite3_compileoption_used("ENABLE_FTS5");

    // step 2: Configure
    const char *sql = "CREATE TABLE t(a); INSERT INTO t VALUES(x'414243'); SELECT a FROM t;";
    int rc_prep1 = sqlite3_prepare_v2(db, sql, -1, &stmt, &pzTail);

    // step 3: Operate / Validate
    int rc_step1 = sqlite3_step(stmt);
    int rc_prep2 = sqlite3_prepare_v2(db, pzTail, -1, &stmt, &pzTail);
    int rc_step2 = sqlite3_step(stmt);
    int rc_prep3 = sqlite3_prepare_v2(db, pzTail, -1, &stmt, &pzTail);
    int rc_step3 = sqlite3_step(stmt);
    expanded = sqlite3_expanded_sql(stmt);
    int rc_blob_open = sqlite3_blob_open(db, "main", "t", "a", 1, 0, &blob);
    int rc_blob_close = sqlite3_blob_close(blob);

    // step 4: Cleanup
    int rc_reset = sqlite3_reset(stmt);
    int rc_close = sqlite3_close_v2(db);
    (void)rc_open;
    (void)opt_used;
    (void)rc_prep1;
    (void)rc_step1;
    (void)rc_prep2;
    (void)rc_step2;
    (void)rc_prep3;
    (void)rc_step3;
    (void)expanded;
    (void)rc_blob_open;
    (void)rc_blob_close;
    (void)rc_reset;
    (void)rc_close;
    // API sequence test completed successfully
    return 66;
}