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
//<ID> 144
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    /* Create and configure options */
    cre2_options_t *opt = cre2_opt_new();
    if (!opt) return 1;
    cre2_opt_set_case_sensitive(opt, 1);
    cre2_opt_set_one_line(opt, 1);

    /* Pattern with two capturing groups */
    const char pattern[] = "(a)(b)c";
    int pattern_len = (int)(sizeof(pattern) - 1);

    /* Compile the regexp with the options */
    cre2_regexp_t *re = cre2_new(pattern, pattern_len, opt);
    if (!re) {
        cre2_opt_delete(opt);
        return 1;
    }

    /* Query number of capturing groups */
    int num_groups = cre2_num_capturing_groups(re);
    if (num_groups < 0) {
        cre2_delete(re);
        cre2_opt_delete(opt);
        return 1;
    }

    /* Prepare input text and match slots (whole match + capturing groups) */
    const char text[] = "abc";
    int text_len = (int)(sizeof(text) - 1);
    int nmatch = num_groups + 1;
    cre2_string_t *matches = new cre2_string_t[nmatch];
    for (int i = 0; i < nmatch; ++i) {
        matches[i].data = nullptr;
        matches[i].length = 0;
    }

    /* Attempt a match */
    int matched = cre2_match(re, text, text_len, 0, text_len, CRE2_UNANCHORED, matches, nmatch);

    /* Clean up */
    delete [] matches;
    cre2_delete(re);
    cre2_opt_delete(opt);

    /* Consider non-zero return from cre2_match as success */
    if (matched) return 66;
    return 2;
}