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
//<ID> 455
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
    sqlite3_int64 current = 0;
    sqlite3_int64 highwater = 0;
    const char *message = "sqlite3_status_and_interrupt_demo";
    int rc_open = 0;
    int rc_status = 0;
    int interrupted = 0;
    int textlen = 0;

    // step 2: Initialize / Configure
    rc_open = sqlite3_open16(filename, &db);

    // step 3: Operate
    rc_status = sqlite3_status64(0, &current, &highwater, 0);
    textlen = (int)(current & 0x7fffffff);
    interrupted = sqlite3_is_interrupted(db);
    sqlite3_result_text(ctx, message, textlen, NULL);

    // step 4: Validate / Cleanup
    sqlite3_interrupt(db);
    db = NULL;
    (void)rc_open;
    (void)rc_status;
    (void)interrupted;
    (void)current;
    (void)highwater;
    (void)textlen;
    (void)message;
    (void)ctx;

    // API sequence test completed successfully
    return 66;
}