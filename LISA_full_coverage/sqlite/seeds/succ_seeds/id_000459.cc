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
//<ID> 459
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
    sqlite3_int64 current_before = 0;
    sqlite3_int64 highwater_before = 0;
    sqlite3_int64 current_after = 0;
    sqlite3_int64 highwater_after = 0;
    int rc_open = 0;
    int rc_status_before = 0;
    int rc_status_after = 0;
    int was_interrupted = 0;
    char result_buf[] = "sqlite3_test_result";
    memset(&current_before, 0, sizeof(current_before));
    memset(&highwater_before, 0, sizeof(highwater_before));
    memset(&current_after, 0, sizeof(current_after));
    memset(&highwater_after, 0, sizeof(highwater_after));

    // step 2: Initialize / Configure
    rc_open = sqlite3_open16(filename, &db);
    rc_status_before = sqlite3_status64(0, &current_before, &highwater_before, 0);

    // step 3: Operate / Validate
    sqlite3_interrupt(db);
    was_interrupted = sqlite3_is_interrupted(db);
    rc_status_after = sqlite3_status64(1, &current_after, &highwater_after, 0);

    // step 4: Cleanup / Finalize
    sqlite3_result_text(ctx, result_buf, (int)(sizeof(result_buf) - 1), NULL);
    db = NULL;
    ctx = NULL;

    (void)rc_open;
    (void)rc_status_before;
    (void)rc_status_after;
    (void)current_before;
    (void)highwater_before;
    (void)current_after;
    (void)highwater_after;
    (void)was_interrupted;
    (void)result_buf;

    // API sequence test completed successfully
    return 66;
}