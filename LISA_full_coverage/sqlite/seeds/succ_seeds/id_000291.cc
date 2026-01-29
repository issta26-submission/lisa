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
//<ID> 291
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
    sqlite3_str *str = NULL;
    void *mem = NULL;
    const char *sql_text = NULL;
    int rc_open = 0;
    int rc_prep = 0;
    int rc_finalize = 0;
    int rc_close = 0;
    int compile_opt_used = 0;
    int blob_close_rc = 0;
    int str_err = 0;

    // step 2: Setup - open an in-memory database and prepare a statement (UTF-16)
    rc_open = sqlite3_open(":memory:", &db);
    rc_prep = sqlite3_prepare16(db, (const void *)u"SELECT 1", -1, &stmt, NULL);

    // step 3: Operate - allocate/reallocate memory, query compile option usage, string-builder error, close blob, inspect prepared SQL
    mem = sqlite3_realloc(NULL, 32);           // allocate 32 bytes
    mem = sqlite3_realloc(mem, 64);            // expand to 64 bytes
    compile_opt_used = sqlite3_compileoption_used("ENABLE_JSON1");
    str_err = sqlite3_str_errcode(str);        // str intentionally NULL to exercise API safely
    blob_close_rc = sqlite3_blob_close((sqlite3_blob *)NULL); // close NULL blob handle safely
    sql_text = sqlite3_sql(stmt);              // obtain SQL text from prepared statement

    // step 4: Cleanup - finalize statement, free allocated memory, and close database
    rc_finalize = sqlite3_finalize(stmt);
    sqlite3_free(mem);
    rc_close = sqlite3_close_v2(db);
    db = NULL;

    (void)rc_open;
    (void)rc_prep;
    (void)rc_finalize;
    (void)rc_close;
    (void)compile_opt_used;
    (void)blob_close_rc;
    (void)str_err;
    (void)sql_text;
    (void)str;

    // API sequence test completed successfully
    return 66;
}