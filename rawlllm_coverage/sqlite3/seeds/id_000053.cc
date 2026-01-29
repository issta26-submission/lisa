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
//<ID> 53
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    const char *kw = "SELECT";
    int kwlen = 6; // length of "SELECT"
    int is_keyword = sqlite3_keyword_check(kw, kwlen);

    int cmp = sqlite3_strnicmp("Hello", "hello", 5);

    sqlite3_stmt *stmt = nullptr;
    int expired = sqlite3_expired(stmt);

    sqlite3_str *s = nullptr;
    char *sval = sqlite3_str_value(s);

    // Combine results in a harmless way so values are used.
    volatile int sink = 0;
    sink += is_keyword;
    sink += (cmp == 0);
    sink += expired;
    sink += (sval != nullptr);

    (void)sink;
    return 66;
}