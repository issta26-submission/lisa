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
//<ID> 649
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "foo";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_buf[] = "foo and foo";
    const int text_len = (int)(sizeof(text_buf) - 1);
    char rewrite_buf[] = "bar";
    const int rewrite_len = (int)(sizeof(rewrite_buf) - 1);
    char errmsg_buf[128];
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_named_groups_iter_t * iter = NULL;
    cre2_string_t text_and_target;
    cre2_string_t rewrite;
    cre2_string_t errmsg;
    cre2_encoding_t enc = CRE2_UNKNOWN;
    const char * pat_ptr = NULL;
    int rc_replace = 0;
    int rc_check = 0;
    int computed_check = 0;
    int first_char = 0;

    // step 2: Setup
    memset(&text_and_target, 0, sizeof(text_and_target));
    memset(&rewrite, 0, sizeof(rewrite));
    memset(&errmsg, 0, sizeof(errmsg));
    memset(errmsg_buf, 0, sizeof(errmsg_buf));
    text_and_target.data = text_buf;
    text_and_target.length = text_len;
    rewrite.data = rewrite_buf;
    rewrite.length = rewrite_len;
    errmsg.data = errmsg_buf;
    errmsg.length = 0;
    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, 1);

    // step 3: Configure
    re = cre2_new(pattern, pattern_len, opt);
    iter = cre2_named_groups_iter_new(re);
    pat_ptr = cre2_pattern(re);
    enc = cre2_opt_encoding(opt);

    // step 4: Operate
    rc_replace = cre2_global_replace(pattern, &text_and_target, &rewrite);
    rc_check = cre2_check_rewrite_string(re, &rewrite, &errmsg);

    // step 5: Validate
    first_char = (int)pat_ptr[0];
    computed_check = rc_replace + rc_check + text_and_target.length + rewrite.length + (int)enc + first_char;
    (void)computed_check;

    // step 6: Cleanup
    cre2_named_groups_iter_delete(iter);
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}