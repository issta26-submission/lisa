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
//<ID> 209
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    sqlite3_int64 hw;
    sqlite3_str *s;
    const char *src;
    int ts;
    int cfg;
    int serr;

    sqlite3_initialize();
    ts = sqlite3_threadsafe();
    src = sqlite3_sourceid();
    sqlite3_sleep(0);
    hw = sqlite3_memory_highwater(0);
    cfg = sqlite3_config(0);

    s = sqlite3_str_new(NULL);
    sqlite3_str_append(s, "abc", 3);
    sqlite3_str_appendchar(s, 2, '!');
    sqlite3_str_appendf(s, " done");
    serr = sqlite3_str_errcode(s);

    (void)hw;
    (void)s;
    (void)src;
    (void)ts;
    (void)cfg;
    (void)serr;

    return 66;
}