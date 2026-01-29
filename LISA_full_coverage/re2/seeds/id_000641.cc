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
//<ID> 641
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(foo)(bar)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_buf[] = "foo and foobar";
    const int text_len = (int)(sizeof(text_buf) - 1);
    char rewrite_buf[] = "X";
    const int rewrite_len = (int)(sizeof(rewrite_buf) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_named_groups_iter_t * iter = NULL;
    cre2_string_t text_and_target;
    cre2_string_t rewrite;
    cre2_string_t errmsg;
    cre2_encoding_t enc = CRE2_UNKNOWN;
    const char * got_pattern = NULL;
    int rc_replace = 0;
    int rc_check = 0;
    int computed_check = 0;

    // step 2: Setup
    memset(&text_and_target, 0, sizeof(text_and_target));
    memset(&rewrite, 0, sizeof(rewrite));
    memset(&errmsg, 0, sizeof(errmsg));
    text_and_target.data = text_buf;
    text_and_target.length = text_len;
    rewrite.data = rewrite_buf;
    rewrite.length = rewrite_len;
    errmsg.data = NULL;
    errmsg.length = 0;
    opt = cre2_opt_new();
    cre2_opt_set_encoding(opt, CRE2_UTF8);
    enc = cre2_opt_encoding(opt);

    // step 3: Configure
    re = cre2_new(pattern, pattern_len, opt);

    // step 4: Operate
    got_pattern = cre2_pattern(re);
    rc_replace = cre2_global_replace(got_pattern, &text_and_target, &rewrite);
    rc_check = cre2_check_rewrite_string(re, &rewrite, &errmsg);
    iter = cre2_named_groups_iter_new(re);

    // step 5: Validate
    computed_check = rc_replace + rc_check + (int)enc + text_and_target.length + rewrite.length + pattern_len;
    (void)computed_check;

    // step 6: Cleanup
    cre2_named_groups_iter_delete(iter);
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}