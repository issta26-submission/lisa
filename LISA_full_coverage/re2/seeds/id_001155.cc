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
//<ID> 1155
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "a([0-9]+)b";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_buf[] = "a123b and a45b";
    const int text_len = (int)(sizeof(text_buf) - 1);
    char rewrite_buf[] = "Z";
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_set * set = NULL;
    cre2_string_t text_and_target;
    cre2_string_t rewrite;
    cre2_string_t matches[4];
    const int nmatch = (int)(sizeof(matches) / sizeof(matches[0]));
    int ret_match = 0;
    int ret_replace = 0;
    int ret_set_add = 0;
    int err_code = 0;
    long computed_check = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, 1);

    // step 3: Operate (compile, add to set, match, and global replace)
    re = cre2_new(pattern, pattern_len, opt);
    set = cre2_set_new(opt, CRE2_UNANCHORED);
    ret_set_add = cre2_set_add_simple(set, pattern);
    text_and_target.data = text_buf;
    text_and_target.length = text_len;
    rewrite.data = rewrite_buf;
    rewrite.length = (int)(sizeof(rewrite_buf) - 1);
    memset(matches, 0, sizeof(matches));
    ret_match = cre2_match(re, text_buf, text_len, 0, text_len, CRE2_UNANCHORED, matches, nmatch);
    ret_replace = cre2_global_replace_re(re, &text_and_target, &rewrite);

    // step 4: Validate (checksum-style usage of results)
    err_code = cre2_error_code(re);
    computed_check = (long)pattern_len + (long)ret_set_add + (long)ret_match + (long)ret_replace + (long)err_code + (long)text_and_target.length + (long)rewrite.length + (long)(unsigned char)text_buf[0];
    (void)computed_check;

    // step 5: Cleanup
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}