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
//<ID> 369
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    int rc_os_init = 0;
    int threadsafe = 0;
    int libver = 0;
    int rc_sleep = 0;
    void *mem = (void *)0;
    void *mem2 = (void *)0;
    int status_cur = 0;
    int status_high = 0;
    sqlite3_int64 highwater = 0;

    rc_os_init = sqlite3_os_init();
    threadsafe = sqlite3_threadsafe();
    libver = sqlite3_libversion_number();
    rc_sleep = sqlite3_sleep(0);
    mem = sqlite3_malloc(16);
    mem2 = sqlite3_realloc(mem, 32);
    sqlite3_status(0, &status_cur, &status_high, 0);
    highwater = sqlite3_memory_highwater(0);
    sqlite3_free_table((char **)0);
    sqlite3_os_end();
    sqlite3_shutdown();

    (void)rc_os_init;
    (void)threadsafe;
    (void)libver;
    (void)rc_sleep;
    (void)mem;
    (void)mem2;
    (void)status_cur;
    (void)status_high;
    (void)highwater;

    return 66;
}