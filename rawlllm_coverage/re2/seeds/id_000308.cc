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
//<ID> 308
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    cre2_options_t *opt = cre2_opt_new();
    if (!opt) return 1;

    // Enable word-boundary handling on options.
    cre2_opt_set_word_boundary(opt, 1);

    // Create a set with these options (use a zero anchor value).
    cre2_set *set = cre2_set_new(opt, (cre2_anchor_t)0);
    if (!set) {
        cre2_opt_delete(opt);
        return 2;
    }

    // Add a couple of simple patterns and compile the set.
    cre2_set_add_simple(set, "foo");
    cre2_set_add_simple(set, "bar");
    cre2_set_compile(set);

    // Text to search.
    const char *text = "foo and bar baz";
    size_t text_len = strlen(text);

    // Buffer to receive matched pattern indices from the set.
    int match_indices[8];
    for (size_t i = 0; i < 8; ++i) match_indices[i] = -1;

    // Use cre2_set_match to find which patterns in the set match the text.
    size_t matched_count = cre2_set_match(set, text, text_len, match_indices, 8);
    (void)matched_count; // suppress unused-variable warning if not used later

    // Prepare input cre2_string_t for partial matching.
    cre2_string_t input;
    input.data = text;
    input.length = (int)text_len;

    // Prepare an array for partial match captures (no named captures here).
    cre2_string_t captures[2];
    for (int i = 0; i < 2; ++i) { captures[i].data = NULL; captures[i].length = 0; }

    // Use cre2_partial_match to look for the literal "bar" inside the text.
    int pm_res = cre2_partial_match("bar", &input, captures, 1);
    (void)pm_res;

    // Create a regex with a named capturing group so we can obtain a named-groups iterator.
    const char *named_pat = "(?P<name>bar)";
    cre2_regexp_t *re_named = cre2_new(named_pat, (int)strlen(named_pat), opt);
    if (re_named) {
        // Create and immediately delete the named-groups iterator (required API usage).
        cre2_named_groups_iter_t *iter = cre2_named_groups_iter_new(re_named);
        if (iter) {
            // We do not need to iterate in this test; just exercise creation + deletion.
            cre2_named_groups_iter_delete(iter);
        }
        cre2_delete(re_named);
    }

    // Clean up set and options.
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    return 66;
}