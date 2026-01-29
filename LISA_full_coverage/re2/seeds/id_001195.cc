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
//<ID> 1195
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
    char text_buf[] = "prefix foo123 suffix foo45 end";
    const int text_len = (int)(sizeof(text_buf) - 1);
    char rewrite_buf[] = "$1:$2";
    const int rewrite_len = (int)(sizeof(rewrite_buf) - 1);
    char target_buf[128];
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t text_str;
    cre2_string_t text_and_target;
    cre2_string_t rewrite_str;
    cre2_string_t match_str;
    int partial_ret = 0;
    int consume_ret = 0;
    int replace_ret = 0;
    int global_ret = 0;
    long computed_check = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, 1);
    cre2_opt_set_literal(opt, 0);
    memset(target_buf, 0, sizeof(target_buf));
    memcpy(target_buf, text_buf, (size_t)text_len);
    memset(&text_str, 0, sizeof(text_str));
    memset(&text_and_target, 0, sizeof(text_and_target));
    memset(&rewrite_str, 0, sizeof(rewrite_str));
    memset(&match_str, 0, sizeof(match_str));
    text_str.data = target_buf;
    text_str.length = text_len;
    text_and_target.data = target_buf;
    text_and_target.length = text_len;
    rewrite_str.data = rewrite_buf;
    rewrite_str.length = rewrite_len;

    // step 3: Compile
    re = cre2_new(pattern, pattern_len, opt);

    // step 4: Core operations
    partial_ret = cre2_partial_match(pattern, &text_str, &match_str, 2);
    consume_ret = cre2_consume(pattern, &text_and_target, &match_str, 2);
    replace_ret = cre2_replace_re(re, &text_and_target, &rewrite_str);
    global_ret = cre2_global_replace_re(re, &text_and_target, &rewrite_str);

    // step 5: Validate / compute lightweight checksum
    computed_check = (long)partial_ret + (long)consume_ret + (long)replace_ret + (long)global_ret
                     + (long)text_and_target.length + (long)match_str.length + (long)rewrite_str.length + (long)pattern_len;
    (void)computed_check;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}