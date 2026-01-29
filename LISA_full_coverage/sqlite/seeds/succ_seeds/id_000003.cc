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
//<ID> 3
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

int test_sqlite3_api_sequence() {
    // Declarations (straight-line sequence; no branches or loops)
    int ts = 0;
    int kw = 0;
    char buf[128];
    int cmp1 = 0;
    int cmp2 = 0;
    int complete = 0;
    int glob = 0;
    unsigned long long msize = 0;
    int status_current = 0;
    int status_high = 0;
    int status_rc = 0;

    ts = sqlite3_threadsafe();
    kw = sqlite3_keyword_count();
    sqlite3_randomness((int)sizeof(buf), buf);
    sqlite3_snprintf((int)sizeof(buf), buf, "threadsafe=%d kw=%d", ts, kw);
    cmp1 = sqlite3_stricmp("Foo", "foo");
    cmp2 = sqlite3_strnicmp("Abc", "abc", 2);
    complete = sqlite3_complete16(u"SELECT 1");
    glob = sqlite3_strglob("pat*", "pattern");
    sqlite3_log(1, "info: %s", buf);
    msize = (unsigned long long)sqlite3_msize(NULL);
    sqlite3_release_memory(0);
    status_rc = sqlite3_status(0, &status_current, &status_high, 0);
    sqlite3_free(NULL);

    (void)ts; (void)kw; (void)cmp1; (void)cmp2; (void)complete; (void)glob;
    (void)msize; (void)status_current; (void)status_high; (void)status_rc;
    (void)buf;
    return 66;
}