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
//<ID> 298
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
    sqlite3_str *strb = NULL;
    sqlite3_blob *blob = NULL;
    const char *sql_text = NULL;
    const char *compile_opt = "ENABLE_FTS5";
    const char *str_value = NULL;
    void *realloc_ptr = NULL;
    int rc_open = 0;
    int rc_prepare = 0;
    int rc_finalize = 0;
    int rc_close = 0;
    int rc_blob_close = 0;
    int opt_used = 0;
    int str_err = 0;
    int alloc_size = 32;

    // step 2: Setup - open in-memory DB and prepare a simple statement
    rc_open = sqlite3_open(":memory:", &db);
    rc_prepare = sqlite3_prepare16(db, (const void*)L"SELECT 1", -1, &stmt, NULL);

    // step 3: Operate & Validate - inspect prepared SQL, query compile option, build a string and inspect its error code
    sql_text = sqlite3_sql(stmt);
    opt_used = sqlite3_compileoption_used(compile_opt);
    strb = sqlite3_str_new(db);
    sqlite3_str_append(strb, "compiled_option=", (int)strlen("compiled_option="));
    sqlite3_str_appendf(strb, "option"); /* appendf variant called with a simple format token */
    str_err = sqlite3_str_errcode(strb);
    str_value = sqlite3_str_value(strb);

    /* allocate a small buffer via sqlite3_realloc to simulate using the allocator */
    realloc_ptr = sqlite3_realloc(NULL, alloc_size);

    // step 4: Cleanup - finalize statement, close blob (NULL allowed as a safe no-op), free allocations and close DB
    rc_finalize = sqlite3_finalize(stmt);
    rc_blob_close = sqlite3_blob_close(blob);
    rc_close = sqlite3_close(db);
    sqlite3_free(realloc_ptr);
    sqlite3_free((void *)strb);

    (void)rc_open;
    (void)rc_prepare;
    (void)sql_text;
    (void)opt_used;
    (void)str_err;
    (void)str_value;
    (void)alloc_size;
    (void)rc_finalize;
    (void)rc_blob_close;
    (void)rc_close;

    // API sequence test completed successfully
    return 66;
}