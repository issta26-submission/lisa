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
//<ID> 458
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    // step 1: Declarations / Initialize
    sqlite3 *db = NULL;
    sqlite3_context *ctx = NULL;
    const void *filename = (const void *)L":memory:";
    const char *result_text = "ok";
    sqlite3_int64 current = 0;
    sqlite3_int64 highwater = 0;
    int rc_open = 0;
    int rc_status = 0;
    int interrupted = 0;
    int rc_close = 0;

    // step 2: Setup / Open and query status
    rc_open = sqlite3_open16(filename, &db);
    rc_status = sqlite3_status64(0, &current, &highwater, 0);

    // step 3: Operate - check interruption and invoke result API (as a simulated callback use)
    interrupted = sqlite3_is_interrupted(db);
    sqlite3_result_text(ctx, result_text, -1, (void (*)(void *))NULL);

    // step 4: Cleanup / Interrupt and close
    sqlite3_interrupt(db);
    rc_close = sqlite3_close(db);
    db = NULL;
    ctx = NULL;

    (void)rc_open;
    (void)rc_status;
    (void)current;
    (void)highwater;
    (void)interrupted;
    (void)rc_close;
    (void)filename;
    (void)result_text;

    // API sequence test completed successfully
    return 66;
}