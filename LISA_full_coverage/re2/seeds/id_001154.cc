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
//<ID> 1154
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(world) (\\d+)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_buf[] = "hello world 42 and world 99!";
    const int text_len = (int)(sizeof(text_buf) - 1);
    char repl_buf[] = "NUMBER";
    const int repl_len = (int)(sizeof(repl_buf) - 1);
    cre2_options_t * opt = NULL;
    cre2_set * set = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t text_and_target;
    cre2_string_t rewrite;
    cre2_string_t match;
    cre2_string_t matches[2];
    int match_ret = 0;
    int replace_ret = 0;
    int err_code = 0;
    long computed_check = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, 1);
    cre2_opt_set_perl_classes(opt, 1);
    set = cre2_set_new(opt, CRE2_UNANCHORED);
    cre2_set_add_simple(set, "world");

    // step 3: Compile and prepare buffers
    re = cre2_new(pattern, pattern_len, opt);
    text_and_target.data = text_buf;
    text_and_target.length = text_len;
    rewrite.data = repl_buf;
    rewrite.length = repl_len;
    memset(matches, 0, sizeof(matches));
    match.data = NULL;
    match.length = 0;

    // step 4: Operate (match and global replace)
    match_ret = cre2_match(re, text_buf, text_len, 0, text_len, CRE2_UNANCHORED, &match, 2);
    replace_ret = cre2_global_replace_re(re, &text_and_target, &rewrite);
    err_code = cre2_error_code(re);

    // step 5: Validate / Cleanup
    computed_check = (long)pattern_len + (long)text_and_target.length + (long)rewrite.length + (long)match_ret + (long)replace_ret + (long)err_code + (long)(matches[0].length) + (long)(matches[1].length);
    (void)computed_check;
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}