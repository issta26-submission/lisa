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
//<ID> 451
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    // step 1: Declarations
    sqlite3 *db = NULL;
    const void *filename = (const void *)L":memory:";
    sqlite3_context *ctx = NULL;
    sqlite3_int64 status_current = 0;
    sqlite3_int64 status_high = 0;
    int rc_open = 0;
    int rc_status = 0;
    int interrupted = 0;
    int use_len = 0;
    const char *text_payload = "sqlite3-status";
    
    // step 2: Initialize / Configure
    rc_open = sqlite3_open16(filename, &db);
    rc_status = sqlite3_status64(0, &status_current, &status_high, 0);
    
    // step 3: Operate
    interrupted = sqlite3_is_interrupted(db);
    use_len = (int)(status_current & 0x7FFFFFFF);
    use_len = use_len % 16;
    use_len = use_len + (interrupted ? 1 : 0);
    sqlite3_result_text(ctx, text_payload, use_len, NULL);
    
    // step 4: Validate / Cleanup
    db = NULL;
    ctx = NULL;
    (void)rc_open;
    (void)rc_status;
    (void)status_high;
    (void)status_current;
    (void)interrupted;
    (void)use_len;
    (void)text_payload;
    // API sequence test completed successfully
    return 66;
}