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
//<ID> 642
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern_buf[] = "([a-z]+)-([a-z]+)";
    const int pattern_len = (int)(sizeof(pattern_buf) - 1);
    char text_buf[] = "one-two three-four";
    const int text_len = (int)(sizeof(text_buf) - 1);
    const char rewrite_buf[] = "$2:$1";
    const int rewrite_len = (int)(sizeof(rewrite_buf) - 1);
    char errmsg_buf[128];
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_named_groups_iter_t * iter = NULL;
    cre2_string_t text_and_target;
    cre2_string_t rewrite;
    cre2_string_t errmsg;
    cre2_encoding_t enc = CRE2_UNKNOWN;
    const char * pattern_from_re = NULL;
    int rc_check = 0;
    int rc_replace = 0;
    int computed_check = 0;

    // step 2: Setup
    memset(errmsg_buf, 0, sizeof(errmsg_buf));
    text_and_target.data = text_buf;
    text_and_target.length = text_len;
    rewrite.data = rewrite_buf;
    rewrite.length = rewrite_len;
    errmsg.data = errmsg_buf;
    errmsg.length = (int)(sizeof(errmsg_buf) - 1);
    opt = cre2_opt_new();

    // step 3: Configure
    cre2_opt_set_case_sensitive(opt, 1);
    re = cre2_new(pattern_buf, pattern_len, opt);
    iter = cre2_named_groups_iter_new(re);
    enc = cre2_opt_encoding(opt);
    pattern_from_re = cre2_pattern(re);

    // step 4: Operate
    rc_check = cre2_check_rewrite_string(re, &rewrite, &errmsg);
    rc_replace = cre2_global_replace(pattern_from_re, &text_and_target, &rewrite);

    // step 5: Validate
    computed_check = (int)enc + rc_check + rc_replace + text_and_target.length + rewrite.length + (int)pattern_len;
    (void)computed_check;

    // step 6: Cleanup
    cre2_named_groups_iter_delete(iter);
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}