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
//<ID> 1157
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "hello([0-9]+)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_buf[] = "xx hello42 yy";
    const int text_len = (int)(sizeof(text_buf) - 1);
    char rewrite_buf[] = "greeting$1";
    const int rewrite_len = (int)(sizeof(rewrite_buf) - 1);
    cre2_options_t * opt = NULL;
    cre2_set * set = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t input;
    cre2_string_t rewrite;
    cre2_string_t matches[4];
    const int nmatch = (int)(sizeof(matches) / sizeof(matches[0]));
    int match_ret = 0;
    int replace_ret = 0;
    int errcode = 0;
    long computed_check = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, 1);
    set = cre2_set_new(opt, CRE2_UNANCHORED);
    cre2_set_add_simple(set, pattern);
    cre2_set_compile(set);

    // step 3: Compile regex
    re = cre2_new(pattern, pattern_len, opt);

    // step 4: Prepare inputs and operate
    input.data = text_buf;
    input.length = text_len;
    rewrite.data = rewrite_buf;
    rewrite.length = rewrite_len;
    memset(matches, 0, sizeof(matches));
    match_ret = cre2_match(re, text_buf, text_len, 0, text_len, CRE2_UNANCHORED, matches, nmatch);
    replace_ret = cre2_global_replace_re(re, &input, &rewrite);
    errcode = cre2_error_code(re);

    // step 5: Validate / compute light checksum
    computed_check = (long)match_ret + (long)replace_ret + (long)errcode + (long)matches[0].length + (long)input.length + (long)rewrite.length + (long)pattern_len;
    (void)computed_check;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}