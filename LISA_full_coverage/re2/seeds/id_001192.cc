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
//<ID> 1192
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
    char text_buf[] = "foo123 and foo45 tail";
    const int text_len = (int)(sizeof(text_buf) - 1);
    char rewrite_buf[] = "$1:$2";
    const int rewrite_len = (int)(sizeof(rewrite_buf) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t input;
    cre2_string_t text_and_target;
    cre2_string_t rewrite_str;
    cre2_string_t matches[4];
    cre2_string_t consumed;
    int pm_ret = 0;
    int cons_ret = 0;
    int replace_ret = 0;
    int greplace_ret = 0;
    long computed_check = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, 1);
    cre2_opt_set_literal(opt, 0);
    input.data = text_buf;
    input.length = text_len;
    text_and_target.data = text_buf;
    text_and_target.length = text_len;
    rewrite_str.data = rewrite_buf;
    rewrite_str.length = rewrite_len;
    memset(matches, 0, sizeof(matches));
    memset(&consumed, 0, sizeof(consumed));

    // step 3: Core operations (partial match then consume)
    pm_ret = cre2_partial_match(pattern, &input, matches, 3);
    cons_ret = cre2_consume(pattern, &input, &consumed, 1);

    // step 4: Replacement operations (compile regex then do single and global replaces)
    re = cre2_new(pattern, pattern_len, opt);
    replace_ret = cre2_replace_re(re, &text_and_target, &rewrite_str);
    greplace_ret = cre2_global_replace_re(re, &text_and_target, &rewrite_str);

    // step 5: Validate / compute lightweight checksum
    computed_check = (long)pm_ret + (long)cons_ret + (long)replace_ret + (long)greplace_ret
                     + (long)matches[0].length + (long)consumed.length + (long)text_and_target.length
                     + (long)rewrite_str.length + (long)pattern_len + (long)(re != NULL);
    (void)computed_check;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}