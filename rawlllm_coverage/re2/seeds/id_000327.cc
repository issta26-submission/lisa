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
//<ID> 327
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    /* Create options and set UTF-8 encoding */
    cre2_options_t *opt = cre2_opt_new();
    if (!opt) return 0;
    cre2_opt_set_encoding(opt, CRE2_UTF8);
    cre2_encoding_t enc = cre2_opt_encoding(opt);

    /* Compile a simple regex with one capturing group */
    const char pattern[] = "([a-z]+)";
    int pat_len = (int)(sizeof(pattern) - 1);
    cre2_regexp_t *re = cre2_new(pattern, pat_len, opt);
    if (!re) {
        cre2_opt_delete(opt);
        return 0;
    }

    /* Query number of capturing groups */
    int num_groups = cre2_num_capturing_groups(re);

    /* Ask for possible match range (min/max strings) */
    cre2_string_t min_ = { nullptr, 0 };
    cre2_string_t max_ = { nullptr, 0 };
    int possible = cre2_possible_match_range(re, &min_, &max_, 100);

    /* Prepare a target text and a rewrite string, then perform global replace */
    char text_buf[] = "apple banana apple";
    cre2_string_t text_and_target = { text_buf, (int)(sizeof(text_buf) - 1) };
    char rewrite_buf[] = "X";
    cre2_string_t rewrite = { rewrite_buf, (int)(sizeof(rewrite_buf) - 1) };
    int replacements = cre2_global_replace_re(re, &text_and_target, &rewrite);

    /* Cleanup */
    cre2_delete(re);
    cre2_opt_delete(opt);

    /* Basic validation of results to ensure sequence executed */
    if (enc != CRE2_UTF8) return 0;
    if (num_groups < 0) return 0;
    if (possible < 0) return 0;
    if (replacements < 0) return 0;

    return 66;
}