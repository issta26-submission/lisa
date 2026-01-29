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
//<ID> 291
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    cre2_options_t *opt = cre2_opt_new();
    if (!opt) return 1;

    // Enable perl classes and word-boundary behavior via setters (defensive).
    cre2_opt_set_perl_classes(opt, 1);
    cre2_opt_set_word_boundary(opt, 1);

    // Verify getters (required APIs usage).
    if (!cre2_opt_perl_classes(opt)) {
        cre2_opt_delete(opt);
        return 2;
    }
    if (!cre2_opt_word_boundary(opt)) {
        cre2_opt_delete(opt);
        return 3;
    }

    // Use a pattern with two named capturing groups "foo" and "bar".
    const char pattern[] = "(?P<foo>abc)(?P<bar>def)";
    cre2_regexp_t *re = cre2_new(pattern, (int)(sizeof(pattern) - 1), opt);
    if (!re) {
        cre2_opt_delete(opt);
        return 4;
    }

    // Find named groups by name (required API usage).
    int idx_foo = cre2_find_named_capturing_groups(re, "foo");
    int idx_bar = cre2_find_named_capturing_groups(re, "bar");
    int idx_baz = cre2_find_named_capturing_groups(re, "baz"); // should not exist

    if (idx_foo < 0 || idx_bar < 0) {
        // Unexpected: named groups not found
        cre2_delete(re);
        cre2_opt_delete(opt);
        return 5;
    }
    // idx_baz is expected to be negative (not found). We don't treat it as fatal.

    // Iterate over named groups using the iterator API (required usage).
    cre2_named_groups_iter_t *iter = cre2_named_groups_iter_new(re);
    if (!iter) {
        cre2_delete(re);
        cre2_opt_delete(opt);
        return 6;
    }

    const char *name = nullptr;
    int index = 0;
    int found_count = 0;
    while (cre2_named_groups_iter_next(iter, &name, &index)) {
        // Basic sanity checks on returned values.
        if (!name || index < 0) {
            cre2_named_groups_iter_delete(iter);
            cre2_delete(re);
            cre2_opt_delete(opt);
            return 7;
        }
        ++found_count;
    }

    // Clean up.
    cre2_named_groups_iter_delete(iter);
    cre2_delete(re);
    cre2_opt_delete(opt);

    // Expect at least the two named groups we created.
    if (found_count < 2) return 8;

    return 66;
}