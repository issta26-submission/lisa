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
//<ID> 192
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    sqlite3 *db = NULL;
    sqlite3_mutex *mtx = sqlite3_mutex_alloc(0);
    const char *ver = sqlite3_libversion();
    sqlite3_int64 mem_used = sqlite3_memory_used();
    sqlite3_int64 mem_hw = sqlite3_memory_highwater(0);
    int cmp = sqlite3_stricmp("One", "one");
    int ncmp = sqlite3_strnicmp("Hello", "hellO", 3);
    int kw_count = sqlite3_keyword_count();
    sqlite3_log(0, "ver=%s cmp=%d ncmp=%d kw=%d", ver, cmp, ncmp, kw_count);
    int try_rc = sqlite3_mutex_try(mtx);
    int held_rc = sqlite3_mutex_held(mtx);
    int notheld_rc = sqlite3_mutex_notheld(mtx);
    sqlite3_mutex_leave(mtx);
    int slept = sqlite3_sleep(1);
    int cfg = sqlite3_config(0);
    sqlite3_release_memory(1024);
    sqlite3_free_table(NULL);
    sqlite3_free(NULL);
    sqlite3_shutdown();

    (void)db;
    (void)mtx;
    (void)ver;
    (void)mem_used;
    (void)mem_hw;
    (void)cmp;
    (void)ncmp;
    (void)kw_count;
    (void)try_rc;
    (void)held_rc;
    (void)notheld_rc;
    (void)slept;
    (void)cfg;

    return 66;
}