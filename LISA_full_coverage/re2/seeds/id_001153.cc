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
//<ID> 1153
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
    /* allocate larger buffers to avoid writes past end during replace operations */
    char text_buf[128] = "prefix foo123bar suffix";
    const int text_len = (int)strlen(text_buf);
    char rewrite_buf[128] = "X$1Y";
    cre2_regexp_t * re = NULL;
    struct cre2_set * set = NULL;
    cre2_options_t * opt = NULL;
    cre2_string_t text_and_target;
    cre2_string_t rewrite;
    cre2_string_t matches[4];
    const int nmatch = (int)(sizeof(matches) / sizeof(matches[0]));
    int rc_set_add = 0;
    int rc_match = 0;
    int rc_replace = 0;
    int rc_error = 0;
    long computed_check = 0;

    // step 2: Setup options, set and regexp
    opt = cre2_opt_new();
    if (opt) {
        /* enable logging of errors to be safe */
        cre2_opt_set_log_errors(opt, 1);
    }

    set = cre2_set_new(opt, CRE2_UNANCHORED);
    if (set) {
        rc_set_add = cre2_set_add_simple(set, pattern);
        /* attempt to compile the set; ignore result here but keep it for computed_check */
        (void)cre2_set_compile(set);
    }

    re = cre2_new(pattern, pattern_len, opt);

    // step 3: Operate
    /* Provide buffers with their full capacities so replace can write safely */
    text_and_target.data = text_buf;
    text_and_target.length = (int)(sizeof(text_buf) - 1);
    rewrite.data = rewrite_buf;
    rewrite.length = (int)(sizeof(rewrite_buf) - 1);

    memset(matches, 0, sizeof(matches));
    if (re) {
        rc_match = cre2_match(re, text_buf, text_len, 0, text_len, CRE2_UNANCHORED, matches, nmatch);
        /* Attempt a global replace using the compiled regexp */
        rc_replace = cre2_global_replace_re(re, &text_and_target, &rewrite);
    }

    // step 4: Validate / collect status
    if (re) {
        rc_error = cre2_error_code(re);
    } else {
        rc_error = -1;
    }

    computed_check = (long)rc_set_add + (long)rc_match + (long)rc_replace + (long)rc_error
                     + (long)matches[0].length + (long)text_and_target.length + (long)rewrite.length;
    (void)computed_check; /* suppress unused variable warning */

    // step 5: Cleanup
    if (re) cre2_delete(re);
    if (set) cre2_set_delete(set);
    if (opt) cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}