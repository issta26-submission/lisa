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
//<ID> 85
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
    const char original_text[] = "prefix foobar suffix";
    const int original_text_len = (int)(sizeof(original_text) - 1);
    const char rewrite_template[] = "$1-REWRITTEN";
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t text_and_target;
    cre2_string_t rewrite_str;
    cre2_encoding_t enc = CRE2_UNKNOWN;
    const char * pattern_from_re = NULL;
    int replace_res = 0;

    // step 2: Setup
    memset(&text_and_target, 0, sizeof(text_and_target));
    memset(&rewrite_str, 0, sizeof(rewrite_str));
    text_and_target.data = original_text;
    text_and_target.length = original_text_len;
    rewrite_str.data = rewrite_template;
    rewrite_str.length = (int)(sizeof(rewrite_template) - 1);
    opt = cre2_opt_new();

    // step 3: Configure
    cre2_opt_set_case_sensitive(opt, 1);
    cre2_opt_set_posix_syntax(opt, 0);
    enc = cre2_opt_encoding(opt);

    // step 4: Operate
    re = cre2_new(pattern, pattern_len, opt);
    pattern_from_re = cre2_pattern(re);
    replace_res = cre2_global_replace(pattern_from_re, &text_and_target, &rewrite_str);

    // step 5: Validate
    (void)enc;
    (void)replace_res;
    (void)text_and_target;
    (void)rewrite_str;
    (void)pattern_from_re;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}