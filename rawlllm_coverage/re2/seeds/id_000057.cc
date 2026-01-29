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
//<ID> 57
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // Create options and enable DOT matches newline
    cre2_options_t *opt = cre2_opt_new();
    if (!opt) return 1;
    cre2_opt_set_dot_nl(opt, 1);

    // Create a set using the options (anchor value cast to silence type issues)
    cre2_set *set = cre2_set_new(opt, (cre2_anchor_t)0);
    if (!set) {
        cre2_opt_delete(opt);
        return 2;
    }

    // Add a couple of simple patterns to the set (ignore integer return codes)
    cre2_set_add_simple(set, "foo.*bar");
    cre2_set_add_simple(set, "^(?P<name>abc)");

    // Compile the set (ignore return code)
    cre2_set_compile(set);

    // Create a regexp that has a named capturing group and iterate over named groups
    const char *pattern = "(?P<name>abc)";
    int pattern_len = (int)(sizeof("(?P<name>abc)") - 1);
    cre2_regexp_t *re = cre2_new(pattern, pattern_len, opt);
    if (re) {
        cre2_named_groups_iter_t *iter = cre2_named_groups_iter_new(re);
        if (iter) {
            const char *name = NULL;
            int index = 0;
            // Iterate through named groups (if any). We do not rely on existence.
            while (cre2_named_groups_iter_next(iter, &name, &index)) {
                // Use name and index in a no-op way to avoid unused-variable warnings
                (void)name;
                (void)index;
            }
            cre2_named_groups_iter_delete(iter);
        }
        cre2_delete(re);
    }

    // Clean up set and options
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    return 66;
}