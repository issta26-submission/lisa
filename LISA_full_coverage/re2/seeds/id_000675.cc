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
//<ID> 675
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "a(b)c";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    struct cre2_string_t rewrite;
    struct cre2_string_t errmsg;
    char rewrite_buf[] = "$1Z";
    int rc_check = 0;
    const char * pat_from_re = NULL;
    int computed_check = 0;

    // step 2: Setup
    memset(&rewrite, 0, sizeof(rewrite));
    memset(&errmsg, 0, sizeof(errmsg));
    opt = cre2_opt_new();
    cre2_opt_set_literal(opt, 1);
    cre2_opt_set_dot_nl(opt, 1);

    // step 3: Configure
    rewrite.data = rewrite_buf;
    rewrite.length = (int)(sizeof(rewrite_buf) - 1);
    re = cre2_new(pattern, pattern_len, opt);

    // step 4: Operate
    rc_check = cre2_check_rewrite_string(re, &rewrite, &errmsg);
    pat_from_re = cre2_pattern(re);

    // step 5: Validate
    computed_check = rc_check + rewrite.length + errmsg.length + (int)pat_from_re[0];
    (void)computed_check;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}