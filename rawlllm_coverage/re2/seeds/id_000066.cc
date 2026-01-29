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
//<ID> 66
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // Create options and configure them
    cre2_options_t *opt = cre2_opt_new();
    if (!opt) return 1;

    // Set POSIX syntax flag (toggle on)
    cre2_opt_set_posix_syntax(opt, 1);

    // Set dot matches newline flag and query it
    cre2_opt_set_dot_nl(opt, 1);
    int dot_nl = cre2_opt_dot_nl(opt);
    (void)dot_nl; // silence unused-variable warnings if any

    // Create a pattern set using these options
    cre2_set *set = cre2_set_new(opt, CRE2_UNANCHORED);
    if (!set) {
        cre2_opt_delete(opt);
        return 2;
    }

    // Add a simple pattern and compile the set
    cre2_set_add_simple(set, "abc");
    cre2_set_compile(set);

    // Try a match against the set
    int match_index = -1;
    size_t matched = cre2_set_match(set, "abc", 3, &match_index, 1);
    (void)matched;
    (void)match_index;

    // Build a regexp using the same options
    const char pattern[] = "(?P<name>abc)"; // named capturing group "name"
    cre2_regexp_t *re = cre2_new(pattern, (int)(sizeof(pattern) - 1), opt);
    if (!re) {
        cre2_set_delete(set);
        cre2_opt_delete(opt);
        return 3;
    }

    // Retrieve error argument (if any) into a cre2_string_t
    cre2_string_t errarg = { NULL, 0 };
    cre2_error_arg(re, &errarg);
    (void)errarg;

    // Iterate over named capturing groups
    cre2_named_groups_iter_t *iter = cre2_named_groups_iter_new(re);
    if (iter) {
        const char *name = NULL;
        int index = 0;
        while (cre2_named_groups_iter_next(iter, &name, &index)) {
            // Use the name and index in some trivial way (no-op here)
            (void)name;
            (void)index;
        }
        cre2_named_groups_iter_delete(iter);
    }

    // Cleanup
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    return 66;
}