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
//<ID> 274
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    cre2_options_t *opt = cre2_opt_new();
    if (!opt) {
        return 66;
    }

    /* Configure options: allow dot to match newline, do not set never_nl */
    cre2_opt_set_dot_nl(opt, 1);
    cre2_opt_set_never_nl(opt, 0);

    /* Create a simple pattern with two capturing groups: "(a)(b)" (length 6) */
    const char *pattern = "(a)(b)";
    cre2_regexp_t *re = cre2_new(pattern, 6, opt);

    if (re) {
        /* Query number of capturing groups */
        int groups = cre2_num_capturing_groups(re);
        (void)groups; /* suppress unused warning in this test */

        /* Retrieve error string (should be empty for success, but we call it to exercise API) */
        const char *err = cre2_error_string(re);
        (void)err;
    }

    /* Prepare text and rewrite strings for cre2_replace */
    cre2_string_t text_and_target;
    text_and_target.data = "abab";
    text_and_target.length = 4;

    cre2_string_t rewrite;
    rewrite.data = "$1-$2";
    rewrite.length = 5;

    /* Perform a replace using a pattern string (non-compiled API) */
    int replace_result = cre2_replace(pattern, &text_and_target, &rewrite);
    (void)replace_result;

    /* Cleanup */
    if (re) cre2_delete(re);
    cre2_opt_delete(opt);

    return 66;
}