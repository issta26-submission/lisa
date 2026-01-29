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
//<ID> 513
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    // step 1: Declarations
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    const char *sql = "SELECT 1";
    char *expanded_sql = NULL;
    void *mem = NULL;
    int rc_open = 0;
    int rc_prep = 0;
    int rc_finalize = 0;
    int rc_close = 0;
    int errcode = 0;

    // step 2: Setup / Initialize
    rc_open = sqlite3_open_v2(":memory:", &db, 0, NULL);
    rc_prep = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);

    // step 3: Operate / Validate
    expanded_sql = sqlite3_expanded_sql(stmt);
    mem = sqlite3_malloc64((sqlite3_uint64)64);
    errcode = sqlite3_errcode(db);
    mem = sqlite3_realloc(mem, 0);
    expanded_sql = (char *)sqlite3_realloc(expanded_sql, 0);

    // step 4: Cleanup / Finalize
    rc_finalize = sqlite3_finalize(stmt);
    rc_close = sqlite3_close(db);

    (void)rc_open;
    (void)rc_prep;
    (void)rc_finalize;
    (void)rc_close;
    (void)errcode;
    (void)sql;
    (void)stmt;
    (void)expanded_sql;
    (void)mem;

    // API sequence test completed successfully
    return 66;
}