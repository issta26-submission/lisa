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
    /* Create options and set the one-line flag */
    cre2_options_t *opt = cre2_opt_new();
    if (opt) {
        cre2_opt_set_one_line(opt, 1);
    }

    /* Create a set with the above options (anchor unanchored) */
    cre2_set *set = nullptr;
    if (opt) {
        set = cre2_set_new(opt, CRE2_UNANCHORED);
    }

    /* Create a regexp using the same options */
    const char *pattern = "ab";
    int pattern_len = 2;
    cre2_regexp_t *re = nullptr;
    if (opt) {
        re = cre2_new(pattern, pattern_len, opt);
    }

    /* Prepare input text and rewrite strings */
    cre2_string_t text_and_target;
    text_and_target.data = "ab ab";
    text_and_target.length = 5; /* length of "ab ab" */

    cre2_string_t rewrite;
    rewrite.data = "X";
    rewrite.length = 1;

    /* Perform a global replace using the compiled regexp if available */
    if (re) {
        (void)cre2_global_replace_re(re, &text_and_target, &rewrite);
    }

    /* Clean up created objects */
    if (re) {
        cre2_delete(re);
    }
    if (set) {
        cre2_set_delete(set);
    }
    if (opt) {
        cre2_opt_delete(opt);
    }

    return 66;
}