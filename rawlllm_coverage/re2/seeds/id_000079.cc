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
//<ID> 79
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // Create default options for the set and regexp creation.
    cre2_options_t *opt = cre2_opt_new();
    // Create a cre2_set with no anchoring using the options (opt may be NULL in some builds,
    // but we handle that below).
    cre2_set *set = cre2_set_new(opt, CRE2_UNANCHORED);

    // Add a couple of simple patterns to the set.
    if (set) {
        cre2_set_add_simple(set, "foo");
        cre2_set_add_simple(set, "bar");
        // We don't call cre2_set_compile here because it's not required for this sequence,
        // and the task focuses on using specific functions.
    }

    // Build a regexp with a named capturing group and an optional capturing group.
    const char pattern[] = "(?P<name>foo)(bar)?";
    int pattern_len = (int)(sizeof(pattern) - 1);
    cre2_regexp_t *re = cre2_new(pattern, pattern_len, opt);

    // Query the number of capturing groups (should be >= 1 for the above pattern).
    int num_groups = 0;
    if (re) {
        num_groups = cre2_num_capturing_groups(re);
    }

    // Create an iterator for named groups and walk any names that exist.
    cre2_named_groups_iter_t *iter = nullptr;
    if (re) {
        iter = cre2_named_groups_iter_new(re);
        if (iter) {
            const char *name = nullptr;
            int index = 0;
            while (cre2_named_groups_iter_next(iter, &name, &index)) {
                // Use the name and index in a no-op way to demonstrate correct usage.
                // (In a real test we might record or assert the values.)
                (void)name;
                (void)index;
            }
            cre2_named_groups_iter_delete(iter);
            iter = nullptr;
        }
    }

    // Prepare text and match storage for a partial-match attempt.
    const char text_data[] = "xxfoobarxx";
    cre2_string_t text = { text_data, (int)(sizeof(text_data) - 1) };

    // Allocate a small matches array. Provide space for a few captures.
    const int max_matches = 10;
    cre2_string_t matches[max_matches];
    for (int i = 0; i < max_matches; ++i) {
        matches[i].data = nullptr;
        matches[i].length = 0;
    }

    // Attempt a partial match using the compiled regexp (if available).
    // We call cre2_partial_match_re with the text and the matches buffer.
    if (re) {
        (void)cre2_partial_match_re(re, &text, matches, max_matches);
    }

    // Cleanup created objects.
    if (re) {
        cre2_delete(re);
        re = nullptr;
    }
    if (set) {
        cre2_set_delete(set);
        set = nullptr;
    }
    if (opt) {
        cre2_opt_delete(opt);
        opt = nullptr;
    }

    // Return 66 to indicate success as required.
    return 66;
}