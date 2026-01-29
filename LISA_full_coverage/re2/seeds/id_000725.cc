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
//<ID> 725
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(foo)(bar)?";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_buf[] = "xx foobar yy foo zz";
    const int text_len = (int)(sizeof(text_buf) - 1);
    char rewrite_buf[] = "[$1-$2]";
    const int rewrite_len = (int)(sizeof(rewrite_buf) - 1);
    char target_buf[128];
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_set * set = NULL;
    cre2_string_t text_str;
    cre2_string_t rewrite_str;
    cre2_string_t target_str;
    cre2_string_t text_and_target;
    int rc_add_simple = 0;
    int rc_set_compile = 0;
    int rc_extract = 0;
    int rc_replace = 0;
    cre2_encoding_t enc = CRE2_UNKNOWN;
    int computed_check = 0;
    int zeros[16];
    memset(zeros, 0, sizeof(zeros));
    memset(target_buf, 0, sizeof(target_buf));

    // step 2: Setup
    opt = cre2_opt_new();
    set = cre2_set_new(opt, (cre2_anchor_t)0);
    rc_add_simple = cre2_set_add_simple(set, pattern);

    // step 3: Configure
    re = cre2_new(pattern, pattern_len, opt);

    // step 4: Operate
    rc_set_compile = cre2_set_compile(set);
    text_str.data = text_buf;
    text_str.length = text_len;
    rewrite_str.data = rewrite_buf;
    rewrite_str.length = rewrite_len;
    target_str.data = target_buf;
    target_str.length = 0;
    rc_extract = cre2_extract_re(re, &text_str, &rewrite_str, &target_str);
    text_and_target.data = text_buf;
    text_and_target.length = text_len;
    rc_replace = cre2_global_replace_re(re, &text_and_target, &rewrite_str);
    enc = cre2_opt_encoding(opt);

    // step 5: Validate
    computed_check = rc_add_simple + rc_set_compile + rc_extract + rc_replace + text_and_target.length + target_str.length + (int)enc + zeros[0];
    (void)computed_check;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}