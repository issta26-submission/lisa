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
//<ID> 1176
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
    char text_buf[] = "prefix foo123 suffix";
    const int text_len = (int)(sizeof(text_buf) - 1);
    char text_and_target_buf[] = "prefix foo123 suffix";
    char rewrite_buf[] = "[$1:$2]";
    const int rewrite_len = (int)(sizeof(rewrite_buf) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t matches[64];
    cre2_string_t text_and_target;
    cre2_string_t rewrite_str;
    int num_groups = 0;
    int nmatch = 0;
    int match_ret = 0;
    int replace_ret = 0;
    long computed_check = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, 1);
    cre2_opt_set_literal(opt, 0);

    // step 3: Compile
    re = cre2_new(pattern, pattern_len, opt);

    // step 4: Prepare match buffers and perform match
    memset(matches, 0, sizeof(matches));
    num_groups = cre2_num_capturing_groups(re);
    nmatch = num_groups + 1;
    if (nmatch <= 0) nmatch = 1; /* harmless direct adjustment without branching logic for flow safety */
    if (nmatch > (int)(sizeof(matches) / sizeof(matches[0]))) nmatch = (int)(sizeof(matches) / sizeof(matches[0]));
    match_ret = cre2_match(re, text_buf, text_len, 0, text_len, CRE2_UNANCHORED, matches, nmatch);

    // step 5: Prepare replace structures and perform replacement
    text_and_target.data = text_and_target_buf;
    text_and_target.length = (int)(sizeof(text_and_target_buf) - 1);
    rewrite_str.data = rewrite_buf;
    rewrite_str.length = rewrite_len;
    replace_ret = cre2_replace_re(re, &text_and_target, &rewrite_str);

    // step 6: Validate / Cleanup
    computed_check = (long)match_ret + (long)replace_ret + (long)num_groups + (long)text_and_target.length + (long)rewrite_str.length + (long)pattern_len;
    (void)computed_check;
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}