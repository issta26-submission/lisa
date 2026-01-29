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
//<ID> 485
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    int vlib = sqlite3_libversion_number();
    const char *src = sqlite3_sourceid();
    const char *comp = sqlite3_compileoption_get(0);
    char buf[64];
    char *pbuf = sqlite3_snprintf(sizeof(buf), buf, "ver=%d", vlib);
    sqlite3_log(0, "fuzz-log");
    int slept = sqlite3_sleep(0);
    sqlite3_thread_cleanup();
    sqlite3_int64 cur = 0;
    sqlite3_int64 high = 0;
    int status_rc = sqlite3_status64(0, &cur, &high, 0);
    sqlite3_memory_highwater(0);
    sqlite3_randomness(0, NULL);
    void *wal_hook_ret = sqlite3_wal_hook(NULL, NULL, NULL);
    void *mem = sqlite3_malloc(32);
    sqlite3_free(mem);
    (void)src;
    (void)comp;
    (void)pbuf;
    (void)slept;
    (void)status_rc;
    (void)cur;
    (void)high;
    (void)wal_hook_ret;
    return 66;
}