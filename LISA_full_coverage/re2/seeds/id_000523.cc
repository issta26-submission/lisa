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
//<ID> 523
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
    const char partial_buf[] = "xxfoobarbaz";
    const int partial_len = (int)(sizeof(partial_buf) - 1);
    const char replace_buf[] = "foo bar foo";
    const int replace_len = (int)(sizeof(replace_buf) - 1);
    const char rewrite_buf[] = "$1-REPL";
    const int rewrite_len = (int)(sizeof(rewrite_buf) - 1);

    cre2_options_t *opt = NULL;
    cre2_regexp_t *re = NULL;

    cre2_string_t partial_input;
    cre2_string_t pmatches[4];
    cre2_string_t errarg;
    cre2_string_t text_and_target;
    cre2_string_t rewrite;

    int partial_ret = 0;
    int replace_ret = 0;
    int computed_check = 0;

    // step 2: Setup
    memset(&partial_input, 0, sizeof(partial_input));
    memset(pmatches, 0, sizeof(pmatches));
    memset(&errarg, 0, sizeof(errarg));
    memset(&text_and_target, 0, sizeof(text_and_target));
    memset(&rewrite, 0, sizeof(rewrite));

    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, 1);

    // step 3: Initialize regex and inputs
    re = cre2_new(pattern, pattern_len, opt);

    partial_input.data = partial_buf;
    partial_input.length = partial_len;

    pmatches[0].data = NULL;
    pmatches[0].length = 0;
    pmatches[1].data = NULL;
    pmatches[1].length = 0;
    pmatches[2].data = NULL;
    pmatches[2].length = 0;
    pmatches[3].data = NULL;
    pmatches[3].length = 0;

    text_and_target.data = replace_buf;
    text_and_target.length = replace_len;

    rewrite.data = rewrite_buf;
    rewrite.length = rewrite_len;

    // step 4: Core operations
    partial_ret = cre2_partial_match_re(re, &partial_input, pmatches, 3);
    cre2_error_arg(re, &errarg);
    replace_ret = cre2_global_replace_re(re, &text_and_target, &rewrite);

    // step 5: Validate
    computed_check = partial_ret + replace_ret + pmatches[0].length + pmatches[1].length + errarg.length + text_and_target.length + rewrite.length;
    (void)computed_check;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}