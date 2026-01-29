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
//<ID> 454
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    // step 1: Declarations
    sqlite3 *db = NULL;
    sqlite3_context *ctx = NULL;
    const void *filename = (const void *)L":memory:";
    const char *text_result = "sqlite3_test";
    int rc_open = 0;
    int rc_status = 0;
    int rc_is_interrupted = 0;
    int rc_close = 0;
    sqlite3_int64 status_current = 0;
    sqlite3_int64 status_highwater = 0;

    // step 2: Initialize / Configure
    rc_open = sqlite3_open16(filename, &db);
    rc_status = sqlite3_status64(0, &status_current, &status_highwater, 0);

    // step 3: Operate
    rc_is_interrupted = sqlite3_is_interrupted(db);
    sqlite3_result_text(ctx, text_result, -1, (void (*)(void *))NULL);

    // step 4: Cleanup / Validate
    rc_close = sqlite3_close(db);
    db = NULL;
    ctx = NULL;

    (void)rc_open;
    (void)rc_status;
    (void)rc_is_interrupted;
    (void)rc_close;
    (void)status_current;
    (void)status_highwater;
    (void)text_result;
    (void)filename;

    // API sequence test completed successfully
    return 66;
}