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
//<ID> 71
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // Create options and enable word-boundary behavior.
    cre2_options_t *opt = cre2_opt_new();
    if (!opt) return 1;
    cre2_opt_set_word_boundary(opt, 1);

    // Pattern using word boundaries.
    const char *pattern = "\\babc\\b";
    // Compute pattern length without relying on headers.
    int pat_len = 0;
    while (pattern[pat_len]) ++pat_len;

    // Create a compiled regexp with the options.
    cre2_regexp_t *re = cre2_new(pattern, pat_len, opt);

    // Create a set (using a default anchor value) and add the same simple pattern.
    cre2_set *set = cre2_set_new(opt, (cre2_anchor_t)0);
    if (set) {
        (void)cre2_set_add_simple(set, pattern);
        cre2_set_delete(set);
    }

    // Prepare input text and a single-slot match array for full match.
    cre2_string_t text;
    text.data = "abc";
    text.length = 3;

    cre2_string_t match;
    match.data = NULL;
    match.length = 0;

    // Attempt a full match using the pattern (pattern form of API).
    (void)cre2_full_match(pattern, &text, &match, 1);

    // Clean up compiled regexp and options.
    if (re) cre2_delete(re);
    cre2_opt_delete(opt);

    return 66;
}