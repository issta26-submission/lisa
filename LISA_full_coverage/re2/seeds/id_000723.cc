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
//<ID> 723
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(ab)(cd)?";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_buf[] = "xx abcd yy ab zz";
    const int text_len = (int)(sizeof(text_buf) - 1);
    char rewrite_buf[] = "$1X$2";
    const int rewrite_len = (int)(sizeof(rewrite_buf) - 1);
    char extract_rewrite_buf[] = "$2$1";
    const int extract_rewrite_len = (int)(sizeof(extract_rewrite_buf) - 1);
    char target_buf[64];
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_set * set = NULL;
    cre2_string_t text_and_target;
    cre2_string_t rewrite;
    cre2_string_t extract_text;
    cre2_string_t extract_rewrite;
    cre2_string_t extract_target;
    int rc_add = 0;
    int rc_compile = 0;
    int rc_replace = 0;
    int rc_extract = 0;
    cre2_encoding_t enc = CRE2_UNKNOWN;
    int computed_check = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, 1);
    cre2_opt_set_encoding(opt, CRE2_UTF8);
    enc = cre2_opt_encoding(opt);

    // step 3: Configure
    re = cre2_new(pattern, pattern_len, opt);
    set = cre2_set_new(opt, (cre2_anchor_t)0);
    rc_add = cre2_set_add_simple(set, pattern);
    rc_compile = cre2_set_compile(set);

    // step 4: Operate
    text_and_target.data = text_buf;
    text_and_target.length = text_len;
    rewrite.data = rewrite_buf;
    rewrite.length = rewrite_len;
    rc_replace = cre2_global_replace_re(re, &text_and_target, &rewrite);

    extract_text.data = text_buf;
    extract_text.length = text_len;
    extract_rewrite.data = extract_rewrite_buf;
    extract_rewrite.length = extract_rewrite_len;
    memset(target_buf, 0, sizeof(target_buf));
    extract_target.data = target_buf;
    extract_target.length = 0;
    rc_extract = cre2_extract_re(re, &extract_text, &extract_rewrite, &extract_target);

    // step 5: Validate
    computed_check = rc_add + rc_compile + rc_replace + rc_extract + (int)enc + text_and_target.length + rewrite.length + extract_target.length;
    (void)computed_check;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}