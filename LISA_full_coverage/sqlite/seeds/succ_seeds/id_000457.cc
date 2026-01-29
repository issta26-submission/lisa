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
//<ID> 457
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
    sqlite3_int64 current_usage = 0;
    sqlite3_int64 highwater_usage = 0;
    const char *message = "sqlite3 status and interrupt test";
    int rc_open = 0;
    int rc_status_before = 0;
    int rc_status_after = 0;
    int interrupted = 0;
    void (*destructor)(void *) = NULL;

    // step 2: Initialize / Configure
    rc_open = sqlite3_open16(filename, &db);
    rc_status_before = sqlite3_status64(0, &current_usage, &highwater_usage, 0);

    // step 3: Operate
    interrupted = sqlite3_is_interrupted(db);
    sqlite3_result_text(ctx, message, -1, destructor);

    // step 4: Validate / Cleanup
    sqlite3_interrupt(db);
    rc_status_after = sqlite3_status64(0, &current_usage, &highwater_usage, 1);
    db = NULL;
    ctx = NULL;

    (void)rc_open;
    (void)rc_status_before;
    (void)rc_status_after;
    (void)interrupted;
    (void)current_usage;
    (void)highwater_usage;
    (void)message;
    (void)destructor;
    (void)filename;

    // API sequence test completed successfully
    return 66;
}