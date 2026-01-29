#include <cre2.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstring>
#include <fcntl.h>
//<ID> 1502
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "^(foo|bar)(baz)?$";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    struct cre2_set * set = NULL;
    const char * re_pat = NULL;
    int add_ret = 0;
    long checksum = 0;

    // step 2: Setup
    memset(&checksum, 0, sizeof(checksum));
    opt = cre2_opt_new();
    set = cre2_set_new(opt, (cre2_anchor_t)0);

    // step 3: Configure / Operate
    add_ret = cre2_set_add_simple(set, pattern);
    re = cre2_new(pattern, pattern_len, opt);

    // step 4: Retrieve pattern and validate
    re_pat = cre2_pattern(re);
    checksum = (long)pattern_len + (long)add_ret + (long)(set != NULL) + (long)(re != NULL) + (long)(re_pat ? (int)strlen(re_pat) : 0);
    (void)checksum;

    // step 5: Cleanup
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // step 6:
    // API sequence test completed successfully
    return 66;
}