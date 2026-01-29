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
//<ID> 211
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    // step 1: Declarations and initialization
    sqlite3_mutex *mtx = sqlite3_mutex_alloc(0);
    sqlite3_stmt *stmt = NULL;
    sqlite3_int64 cur = 0;
    sqlite3_int64 hi = 0;
    const char *extracted_param = NULL;
    char *uri = NULL;
    char *report = NULL;
    int rc_explain = 0;
    void *tmp_free = NULL;

    // step 2: Configure - build a URI and extract a parameter
    uri = sqlite3_mprintf("file:%s?mode=%s&cache=%s", "test.db", "ro", "shared");
    extracted_param = sqlite3_uri_parameter((sqlite3_filename)uri, "mode");

    // step 3: Operate - query a status and attempt to explain a (NULL) stmt
    sqlite3_status64(0, &cur, &hi, 0);
    rc_explain = sqlite3_stmt_explain(stmt, 1);

    // step 4: Validate and cleanup - produce a small report and free allocated buffers
    report = sqlite3_mprintf("explain_rc=%d mode=%s cur=%lld hi=%lld", rc_explain, (extracted_param ? extracted_param : "NULL"), (long long)cur, (long long)hi);
    tmp_free = sqlite3_realloc(uri, 0);
    tmp_free = sqlite3_realloc(report, 0);

    (void)mtx;
    (void)stmt;
    (void)cur;
    (void)hi;
    (void)extracted_param;
    (void)rc_explain;
    (void)tmp_free;

    (void)"API sequence test completed successfully";
    return 66;
}