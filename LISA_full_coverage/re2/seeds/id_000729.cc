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
//<ID> 729
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
    char text_and_target_buf[128];
    char rewrite_buf[] = "REPLACED";
    char target_buf[128];
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_set * set = NULL;
    cre2_string_t text;
    cre2_string_t text_and_target;
    cre2_string_t rewrite;
    cre2_string_t target;
    int rc_add = 0;
    int rc_compile = 0;
    int rc_extract = 0;
    int rc_replace = 0;
    cre2_encoding_t enc = CRE2_UNKNOWN;
    int computed_check = 0;

    // step 2: Setup
    memset(text_and_target_buf, 0, sizeof(text_and_target_buf));
    memset(target_buf, 0, sizeof(target_buf));
    opt = cre2_opt_new();

    // step 3: Configure
    re = cre2_new(pattern, pattern_len, opt);
    set = cre2_set_new(opt, (cre2_anchor_t)0);
    rc_add = cre2_set_add_simple(set, pattern);
    rc_compile = cre2_set_compile(set);

    // step 4: Operate
    memcpy(text_and_target_buf, text_buf, (size_t)text_len);
    text.data = text_buf;
    text.length = text_len;
    text_and_target.data = text_and_target_buf;
    text_and_target.length = (int)strlen(text_and_target_buf);
    rewrite.data = rewrite_buf;
    rewrite.length = (int)(sizeof(rewrite_buf) - 1);
    target.data = target_buf;
    target.length = 0;
    rc_extract = cre2_extract_re(re, &text, &rewrite, &target);
    rc_replace = cre2_global_replace_re(re, &text_and_target, &rewrite);
    enc = cre2_opt_encoding(opt);

    // step 5: Validate
    computed_check = rc_add + rc_compile + rc_extract + rc_replace + (int)enc + text.length + rewrite.length + (int)text_and_target.length + (int)target.length;
    (void)computed_check;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}