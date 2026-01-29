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
//<ID> 265
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    cre2_options_t *opt = cre2_opt_new();
    if (!opt) return 1;

    /* Set never-capture mode on the options and query the literal flag (as required). */
    cre2_opt_set_never_capture(opt, 1);
    int literal_flag = cre2_opt_literal(opt);
    (void)literal_flag; /* silence unused-warning if any */

    /* Create a pattern set with the options. */
    cre2_set *set = cre2_set_new(opt, CRE2_UNANCHORED);
    if (!set) {
        cre2_opt_delete(opt);
        return 2;
    }

    /* Prepare a pattern and compile a regexp object with the same options. */
    const char pattern[] = "foo(\\d+)";
    int pattern_len = (int)(sizeof(pattern) - 1);
    cre2_regexp_t *re = cre2_new(pattern, pattern_len, opt);
    if (!re) {
        cre2_set_delete(set);
        cre2_opt_delete(opt);
        return 3;
    }

    /* Prepare text and rewrite strings for a global replace using the pattern string API. */
    const char text_buf[] = "one foo123 two foo456";
    cre2_string_t text_and_target = { text_buf, (int)(sizeof(text_buf) - 1) };

    const char rewrite_buf[] = "X";
    cre2_string_t rewrite = { rewrite_buf, (int)(sizeof(rewrite_buf) - 1) };

    /* Perform a global replace using the pattern (uses cre2_global_replace as required). */
    /* The return value is ignored for this test; just exercise the API. */
    (void)cre2_global_replace(pattern, &text_and_target, &rewrite);

    /* Prepare subject and result containers for find-and-consume using the compiled regexp. */
    const char subject_buf[] = "foo123rest";
    cre2_string_t subject = { subject_buf, (int)(sizeof(subject_buf) - 1) };
    cre2_string_t match = { NULL, 0 };

    /* Use the compiled regexp to find and consume from the subject (required API). */
    (void)cre2_find_and_consume_re(re, &subject, &match, 0);

    /* Clean up all allocated objects. */
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    return 66;
}