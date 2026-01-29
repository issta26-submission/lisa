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
//<ID> 167
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    // step 1: Initialize
    int rc_os = sqlite3_os_init();
    sqlite3_stmt *stmt = NULL;
    const char *tail = NULL;

    // step 2: Configure
    const char *opt0 = sqlite3_compileoption_get(0);
    const char *opt1 = sqlite3_compileoption_get(1);

    // step 3: Operate / Validate
    int rc_prep = sqlite3_prepare_v2(NULL, opt0, -1, &stmt, &tail);
    int rc_ext_on = sqlite3_extended_result_codes(NULL, 1);
    const char *opt_runtime = sqlite3_compileoption_get(rc_prep & 0x7);
    int rc_ext_off = sqlite3_extended_result_codes(NULL, 0);

    // step 4: Cleanup / Final checks
    int aggregate = rc_os ^ rc_prep ^ rc_ext_on ^ rc_ext_off;
    const char *opt_check = sqlite3_compileoption_get(aggregate & 0x3);

    (void)stmt;
    (void)tail;
    (void)opt0;
    (void)opt1;
    (void)opt_runtime;
    (void)opt_check;
    (void)aggregate;

    // API sequence test completed successfully
    return 66;
}