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
//<ID> 19
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    // step 1: Declarations / initialize
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    char *sqlBuf = NULL;
    char *expanded = NULL;
    int rc_open = 0;
    int rc_prepare = 0;
    int rc_reset = 0;
    int rc_finalize = 0;
    int ver = 0;

    // step 2: allocate buffer and populate SQL text (straight-line assignments, no loops)
    sqlBuf = (char *)sqlite3_malloc(10);
    sqlBuf[0] = 'S';
    sqlBuf[1] = 'E';
    sqlBuf[2] = 'L';
    sqlBuf[3] = 'E';
    sqlBuf[4] = 'C';
    sqlBuf[5] = 'T';
    sqlBuf[6] = ' ';
    sqlBuf[7] = '1';
    sqlBuf[8] = ';';
    sqlBuf[9] = '\0';

    // step 3: open in-memory database and prepare the SQL (use prepare to validate completeness)
    rc_open = sqlite3_open(":memory:", &db);
    rc_prepare = sqlite3_prepare_v2(db, sqlBuf, -1, &stmt, NULL);

    // step 4: obtain expanded SQL text (optional introspection), query a simple library property
    expanded = sqlite3_expanded_sql(stmt);
    ver = sqlite3_libversion_number();

    // step 5: reset and finalize the prepared statement, then free allocations
    rc_reset = sqlite3_reset(stmt);
    rc_finalize = sqlite3_finalize(stmt);
    sqlite3_free(expanded);
    sqlite3_free(sqlBuf);

    // silence unused-var warnings while preserving the straight-line sequence requirement
    (void)rc_open;
    (void)rc_prepare;
    (void)rc_reset;
    (void)rc_finalize;
    (void)ver;
    (void)db;

    // API sequence test completed successfully
    return 66;
}