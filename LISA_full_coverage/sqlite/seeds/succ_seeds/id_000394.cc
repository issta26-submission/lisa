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
//<ID> 394
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    // Step 1: Declarations
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    sqlite3_str *pStr = NULL;
    unsigned char *serialized = NULL;
    sqlite3_int64 sz = 0;
    const unsigned char *coltext = NULL;
    char *sizebuf = NULL;
    char *strOut = NULL;
    char *tmpValue = NULL;
    int rc = 0;

    // Step 2: Setup - open in-memory DB and create a table, insert a row
    rc = sqlite3_open(":memory:", &db);
    rc = sqlite3_prepare_v2(db, "CREATE TABLE t(a TEXT);", -1, &stmt, NULL);
    rc = sqlite3_step(stmt);
    rc = sqlite3_finalize(stmt);
    stmt = NULL;
    rc = sqlite3_prepare_v2(db, "INSERT INTO t(a) VALUES('hello');", -1, &stmt, NULL);
    rc = sqlite3_step(stmt);
    rc = sqlite3_finalize(stmt);
    stmt = NULL;

    // Step 3: Operate - serialize DB and select the inserted value
    serialized = sqlite3_serialize(db, "main", &sz, 0);
    rc = sqlite3_prepare_v2(db, "SELECT a FROM t LIMIT 1;", -1, &stmt, NULL);
    rc = sqlite3_step(stmt);
    coltext = sqlite3_column_text(stmt, 0);

    // Step 4: Validate and Cleanup - build a string reporting serialized size and column text, finalize and close
    pStr = sqlite3_str_new(db);
    sizebuf = sqlite3_mprintf("%lld", (long long)sz);
    sqlite3_str_appendall(pStr, "size=");
    sqlite3_str_appendall(pStr, sizebuf);
    sqlite3_str_appendall(pStr, ";value=");
    sqlite3_str_appendall(pStr, (const char *)coltext);
    tmpValue = sqlite3_str_value(pStr);
    strOut = sqlite3_str_finish(pStr);
    pStr = NULL;
    rc = sqlite3_finalize(stmt);
    stmt = NULL;
    sqlite3_free(sizebuf);
    sqlite3_free(serialized);
    rc = sqlite3_close(db);
    db = NULL;

    (void)rc;
    (void)tmpValue;
    (void)strOut;
    (void)coltext;

    // API sequence test completed successfully
    return 66;
}