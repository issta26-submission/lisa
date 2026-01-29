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
//<ID> 127
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    /* Create options and configure them */
    cre2_options_t *opt = cre2_opt_new();
    if (!opt) return 1;

    /* Set literal mode and never_nl as required by the task */
    cre2_opt_set_literal(opt, 1);
    cre2_opt_set_never_nl(opt, 1);

    /* Query case-sensitivity (demonstrate use of cre2_opt_case_sensitive) */
    int case_sensitive = cre2_opt_case_sensitive(opt);
    (void)case_sensitive; /* silence unused-variable warnings if any */

    /* Create a set with the options. Use anchor value 0 (cast to cre2_anchor_t). */
    cre2_set *set = cre2_set_new(opt, (cre2_anchor_t)0);
    if (!set) {
        cre2_opt_delete(opt);
        return 2;
    }

    /* Add a simple pattern to the set */
    const char set_pattern[] = "hello";
    /* cre2_set_add_simple returns an int; treat negative as error if it occurs */
    int add_ret = cre2_set_add_simple(set, set_pattern);
    (void)add_ret; /* we don't rely on a specific return code here */

    /* Prepare a simple pattern/text pair and a match output */
    const char pattern[] = "hello";
    const char text[] = "hello world";
    int pattern_len = (int)(sizeof(pattern) - 1);
    int text_len = (int)(sizeof(text) - 1);

    cre2_string_t match;
    match.data = NULL;
    match.length = 0;

    /* Use cre2_easy_match to attempt a match */
    int em_ret = cre2_easy_match(pattern, pattern_len, text, text_len, &match, 1);
    /* em_ret: typically 1 for match, 0 for no match, negative for error.
       We won't treat a "no match" as failure for this test; only negative as error. */
    if (em_ret < 0) {
        cre2_set_delete(set);
        cre2_opt_delete(opt);
        return 3;
    }

    /* Clean up */
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    /* Success */
    return 66;
}