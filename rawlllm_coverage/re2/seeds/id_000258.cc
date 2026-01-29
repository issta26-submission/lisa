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
//<ID> 258
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

    /* Prepare an original pattern and a container for the quoted pattern */
    cre2_string_t original = { "a+b", 3 }; /* + is meta, we'll quote it */
    cre2_string_t quoted = { NULL, 0 };

    /* Quote the meta-characters from original into quoted */
    (void)cre2_quote_meta(&quoted, &original);

    /* Build a regexp from the quoted pattern (fall back to original if quoting failed) */
    const char *pattern_data = quoted.data ? quoted.data : original.data;
    int pattern_len = quoted.length ? quoted.length : original.length;
    cre2_regexp_t *re = cre2_new(pattern_data, pattern_len, opt);
    if (!re) {
        cre2_opt_delete(opt);
        return 2;
    }

    /* Prepare text and a match array for find_and_consume_re */
    cre2_string_t text = { "a+b and following", 17 };
    cre2_string_t matches[4];
    for (int i = 0; i < 4; ++i) {
        matches[i].data = NULL;
        matches[i].length = 0;
    }

    /* Try to find and consume using the compiled regexp */
    (void)cre2_find_and_consume_re(re, &text, matches, 2);

    /* Demonstrate cre2_set_match: create a set, add a simple pattern, compile and match */
    cre2_set *set = cre2_set_new(opt, (cre2_anchor_t)0);
    if (set) {
        (void)cre2_set_add_simple(set, "a+b"); /* add same pattern (simple API) */
        (void)cre2_set_compile(set);

        int match_indexes[8] = {0};
        const char *set_text = "prefix a+b suffix";
        size_t matched = cre2_set_match(set, set_text, strlen(set_text), match_indexes, 8);
        (void)matched; /* ignore result for this test */

        cre2_set_delete(set);
    }

    /* Clean up regexp and options */
    cre2_delete(re);
    cre2_opt_delete(opt);

    return 66;
}