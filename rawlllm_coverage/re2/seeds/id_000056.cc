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
//<ID> 56
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    cre2_options_t *opt = cre2_opt_new();
    if (!opt) return 1;

    /* Query the encoding from options to exercise cre2_opt_encoding. */
    cre2_encoding_t enc = cre2_opt_encoding(opt);
    (void)enc; /* use variable to avoid unused-warning */

    /* Create a set (anchor value supplied as a casted 0). */
    cre2_set *set = cre2_set_new(opt, (cre2_anchor_t)0);

    /* Create a simple regexp using the options. */
    const char *pattern = "a";
    cre2_regexp_t *re = cre2_new(pattern, 1, opt);
    if (!re) {
        if (set) cre2_set_delete(set);
        cre2_opt_delete(opt);
        return 2;
    }

    /* Prepare text and rewrite strings for replace operation. */
    const char *text = "aba a ba"; /* length 8 */
    cre2_string_t text_and_target = { text, 8 };
    const char *rewrite_s = "X";
    cre2_string_t rewrite = { rewrite_s, 1 };

    /* Perform a single replace using the compiled regexp. */
    int replace_ret = cre2_replace_re(re, &text_and_target, &rewrite);

    /* Clean up objects created. */
    cre2_delete(re);
    if (set) cre2_set_delete(set);
    cre2_opt_delete(opt);

    /* Treat non-negative return as success for this test. */
    if (replace_ret >= 0) return 66;
    return 3;
}