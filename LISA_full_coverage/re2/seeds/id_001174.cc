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
//<ID> 1174
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(foo)([0-9]+)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_buf[] = "prefix foo123 middle foo45 suffix";
    const int text_len = (int)(sizeof(text_buf) - 1);
    char rewrite_buf[] = "[$2:$1]";
    const int rewrite_len = (int)(sizeof(rewrite_buf) - 1);
    char replace_buf[128];
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t text_and_target;
    cre2_string_t rewrite_str;
    cre2_string_t matches[3];
    const int nmatch = (int)(sizeof(matches) / sizeof(matches[0]));
    int match_ret = 0;
    int replace_ret = 0;
    int num_groups = 0;
    int err_code = 0;
    long checksum = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, 1);
    cre2_opt_set_literal(opt, 0);

    // step 3: Compile and prepare buffers
    re = cre2_new(pattern, pattern_len, opt);
    memset(matches, 0, sizeof(matches));
    memset(replace_buf, 0, sizeof(replace_buf));
    text_and_target.data = text_buf;
    text_and_target.length = text_len;
    rewrite_str.data = rewrite_buf;
    rewrite_str.length = rewrite_len;

    // step 4: Operate (count groups, perform a match, then a replace using the compiled regexp)
    num_groups = cre2_num_capturing_groups(re);
    match_ret = cre2_match(re, text_buf, text_len, 0, text_len, CRE2_UNANCHORED, matches, nmatch);
    replace_ret = cre2_replace_re(re, &text_and_target, &rewrite_str);

    // step 5: Validate / compute lightweight checksum
    err_code = cre2_error_code(re);
    checksum = (long)num_groups + (long)match_ret + (long)replace_ret + (long)err_code + (long)matches[0].length + (long)text_and_target.length + (long)pattern_len;
    (void)checksum;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}