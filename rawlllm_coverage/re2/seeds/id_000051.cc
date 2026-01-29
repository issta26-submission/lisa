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
//<ID> 51
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    /* Create options and set some flags */
    cre2_options_t *opt = cre2_opt_new();
    if (!opt) return 1;

    /* Toggle literal mode and longest-match mode */
    cre2_opt_set_literal(opt, 1);
    cre2_opt_set_longest_match(opt, 0);

    /* Query encoding (use the value to avoid unused-variable warnings) */
    cre2_encoding_t enc = cre2_opt_encoding(opt);
    (void)enc;

    /* Create a pattern set and immediately delete it to exercise set lifecycle.
       Use a zero value cast for the anchor enum (safe as a placeholder). */
    cre2_set *set = cre2_set_new(opt, (cre2_anchor_t)0);
    if (set) {
        cre2_set_delete(set);
        set = nullptr;
    }

    /* Prepare the text/target and rewrite strings for a replace operation.
       Lengths are provided explicitly to avoid depending on external headers. */
    cre2_string_t text_and_target = { "foo bar foo", 11 };
    cre2_string_t rewrite = { "baz", 3 };

    /* Perform a single replace operation using a simple pattern */
    int replace_ret = cre2_replace("foo", &text_and_target, &rewrite);
    (void)replace_ret; /* ignore result for this test sequence */

    /* Clean up options */
    cre2_opt_delete(opt);

    return 66;
}