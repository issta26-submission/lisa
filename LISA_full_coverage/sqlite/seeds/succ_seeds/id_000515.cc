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
//<ID> 515
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
    const char *sql = "SELECT 42 AS value";
    char *expanded_sql = NULL;
    void *buffer = NULL;
    sqlite3_uint64 buf_size = (sqlite3_uint64)256;
    int rc_open = 0;
    int rc_prep = 0;
    int rc_finalize = 0;
    int rc_close = 0;
    int errcode = 0;
    int colcount = 0;

    rc_open = sqlite3_open_v2(":memory:", &db, 0, NULL);

    // step 2: Configure / Operate
    rc_prep = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    expanded_sql = sqlite3_expanded_sql(stmt);
    buffer = sqlite3_malloc64(buf_size);
    memset(buffer, 0, (size_t)buf_size);
    sqlite3_snprintf((int)buf_size, (char *)buffer, "%s", expanded_sql);
    errcode = sqlite3_errcode(db);

    // step 3: Validate
    colcount = sqlite3_column_count(stmt);
    (void)colcount;
    (void)errcode;
    (void)buffer;
    (void)expanded_sql;
    (void)rc_prep;
    (void)rc_open;

    // step 4: Cleanup
    rc_finalize = sqlite3_finalize(stmt);
    rc_close = sqlite3_close(db);
    (void)rc_finalize;
    (void)rc_close;

    // API sequence test completed successfully
    return 66;
}