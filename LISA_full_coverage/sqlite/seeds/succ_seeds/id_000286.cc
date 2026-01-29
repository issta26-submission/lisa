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
//<ID> 286
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    const char *source_id = NULL;
    const char *compile_opt = NULL;
    int slept_ms = 0;
    int tc_rc = 0;
    unsigned char rnd_buf[16];
    sqlite3_int64 heap_limit = 0;
    sqlite3_int64 uri_val = 0;

    source_id = sqlite3_sourceid();
    compile_opt = sqlite3_compileoption_get(0);
    sqlite3_randomness((int)sizeof(rnd_buf), rnd_buf);
    slept_ms = sqlite3_sleep(0);
    tc_rc = sqlite3_test_control(0);
    heap_limit = sqlite3_soft_heap_limit64((sqlite3_int64)1024);
    uri_val = sqlite3_uri_int64((sqlite3_filename)NULL, "param", (sqlite3_int64)12345);

    (void)source_id;
    (void)compile_opt;
    (void)slept_ms;
    (void)tc_rc;
    (void)rnd_buf;
    (void)heap_limit;
    (void)uri_val;

    return 66;
}