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
//<ID> 119
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // Create options and set word-boundary option
    cre2_options_t *opt = cre2_opt_new();
    if (!opt) return 1;
    cre2_opt_set_word_boundary(opt, 1);

    // Prepare a pattern and create a regexp object using the options
    const char *pattern = "ab";
    int pattern_len = (int)(sizeof("ab") - 1);
    cre2_regexp_t *re = cre2_new(pattern, pattern_len, opt);
    if (!re) {
        cre2_opt_delete(opt);
        return 2;
    }

    // Create a set (use anchor value 0) and compile it
    cre2_set *set = cre2_set_new(opt, (cre2_anchor_t)0);
    if (!set) {
        cre2_delete(re);
        cre2_opt_delete(opt);
        return 3;
    }
    int compile_ret = cre2_set_compile(set);
    (void)compile_ret; // ignore specific compile result for this test sequence

    // Prepare text and rewrite strings for global replace
    cre2_string_t text_and_target;
    text_and_target.data = "ab ab";
    text_and_target.length = (int)(sizeof("ab ab") - 1);

    cre2_string_t rewrite;
    rewrite.data = "X";
    rewrite.length = (int)(sizeof("X") - 1);

    // Perform a global replace using the plain-pattern API
    int replace_ret = cre2_global_replace(pattern, &text_and_target, &rewrite);
    (void)replace_ret; // accept whatever result as part of this sequence

    // Cleanup
    cre2_set_delete(set);
    cre2_delete(re);
    cre2_opt_delete(opt);

    return 66;
}