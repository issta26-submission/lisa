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
//<ID> 1158
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "foo([0-9]+)bar";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_buf[] = "prefix foo123bar middle foo456bar suffix";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_set * set = NULL;
    cre2_string_t input;
    cre2_string_t rewrite;
    cre2_string_t matches[3];
    const int nmatch = (int)(sizeof(matches) / sizeof(matches[0]));
    int match_ret = 0;
    int global_replace_ret = 0;
    int add_ret = 0;
    int err_code = 0;
    long computed_check = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, 1);
    set = cre2_set_new(opt, CRE2_UNANCHORED);

    // step 3: Compile pattern into a regexp object
    re = cre2_new(pattern, pattern_len, opt);

    // step 4: Inspect compilation outcome
    err_code = cre2_error_code(re);

    // step 5: Operate - perform a match, then a global replace using the compiled regexp, and add pattern to set
    input.data = text_buf;
    input.length = text_len;
    memset(matches, 0, sizeof(matches));
    match_ret = cre2_match(re, text_buf, text_len, 0, text_len, CRE2_UNANCHORED, matches, nmatch);
    rewrite.data = "X";
    rewrite.length = 1;
    global_replace_ret = cre2_global_replace_re(re, &input, &rewrite);
    add_ret = cre2_set_add_simple(set, pattern);

    // step 6: Validate / Cleanup
    computed_check = (long)err_code + (long)match_ret + (long)global_replace_ret + (long)add_ret + (long)matches[0].length + (long)input.length;
    (void)computed_check;
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}