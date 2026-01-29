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
//<ID> 216
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

int test_sqlite3_api_sequence() {
    // step 1: Declarations and initial status query
    sqlite3_mutex *mtx = NULL;
    sqlite3_stmt *stmt = NULL;
    sqlite3_int64 cur = 0;
    sqlite3_int64 hi = 0;
    int resetFlag = 0;
    int status_rc = 0;
    char *uri_buf = NULL;
    const char *uri_param = NULL;
    int stmt_explain_rc = 0;
    int mtx_try_rc = 0;
    int cmp_rc = 0;

    status_rc = sqlite3_status64(0, &cur, &hi, resetFlag);

    // step 2: Build a URI string using sqlite3_malloc
    const char *literal = "file:demo.db?mode=memory&cache=shared";
    uri_buf = (char*)sqlite3_malloc((int)(strlen(literal) + 1));
    memcpy(uri_buf, literal, strlen(literal) + 1);

    // step 3: Allocate a mutex and check its held/notheld status
    mtx = sqlite3_mutex_alloc(0);
    mtx_try_rc = sqlite3_mutex_held(mtx);
    cmp_rc = sqlite3_mutex_notheld(mtx);

    // step 4: Inspect URI parameter (basic pointer into string) and run stmt explain, then cleanup
    uri_param = uri_buf + 5; /* points past "file:" */
    cmp_rc = sqlite3_strnicmp(uri_param, "demo.db", 7);
    stmt_explain_rc = sqlite3_stmt_isexplain(stmt);

    /* free the allocated URI buffer using sqlite3_realloc64(ptr,0) */
    sqlite3_realloc64(uri_buf, 0);

    (void)status_rc;
    (void)cur;
    (void)hi;
    (void)resetFlag;
    (void)uri_param;
    (void)stmt_explain_rc;
    (void)mtx_try_rc;
    (void)cmp_rc;
    (void)mtx;
    (void)stmt;
    // API sequence test completed successfully

    return 66;
}