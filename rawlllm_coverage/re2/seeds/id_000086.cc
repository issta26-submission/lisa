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
//<ID> 86
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // Create options
    cre2_options_t *opt = cre2_opt_new();
    if (!opt) {
        return 1; // failed to allocate options
    }

    // Create a set using the options. Use anchor value 0 (cast) to avoid
    // depending on specific enum identifiers here.
    cre2_set *set = cre2_set_new(opt, (cre2_anchor_t)0);

    // If set creation succeeded, add a simple pattern.
    if (set) {
        // cre2_set_add_simple returns an int, but we ignore it for this test flow.
        cre2_set_add_simple(set, "hello.*");
    }

    // Create a regexp that contains a named capturing group to exercise
    // cre2_named_groups_iter_new.
    const char *pattern = "(?P<name>hello)";
    cre2_regexp_t *re = cre2_new(pattern, (int)(sizeof("(?P<name>hello)") - 1), opt);

    // Create a named-groups iterator from the regexp (if created).
    cre2_named_groups_iter_t *iter = nullptr;
    if (re) {
        iter = cre2_named_groups_iter_new(re);
    }

    // Clean up iterator if created.
    if (iter) {
        cre2_named_groups_iter_delete(iter);
    }

    // Clean up regexp, set, and options.
    if (re) {
        cre2_delete(re);
    }
    if (set) {
        cre2_set_delete(set);
    }
    cre2_opt_delete(opt);

    return 66;
}