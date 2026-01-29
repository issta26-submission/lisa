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
//<ID> 217
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // Create options
    cre2_options_t *opt = cre2_opt_new();
    if (!opt) return 1;

    // Query the 'literal' option (uses required API cre2_opt_literal)
    int literal_flag = cre2_opt_literal(opt);
    (void)literal_flag; // silence unused variable warning if any

    // Prepare a pattern (with a named capturing group) and compute its length without relying on external headers
    const char *pattern = "(?P<name>abc)|def";
    int pattern_len = 0;
    while (pattern[pattern_len] != '\0') ++pattern_len;

    // Compile the regexp with the options (uses required API cre2_new)
    cre2_regexp_t *re = cre2_new(pattern, pattern_len, opt);
    if (!re) {
        cre2_opt_delete(opt);
        return 2;
    }

    // Iterate over named groups using the iterator and cre2_named_groups_iter_next (required)
    cre2_named_groups_iter_t *iter = cre2_named_groups_iter_new(re);
    if (iter) {
        const char *group_name = NULL;
        int group_index = 0;
        while (cre2_named_groups_iter_next(iter, &group_name, &group_index)) {
            // Use the returned name/index in a minimal way (no I/O here).
            if (group_name == NULL) break;
            (void)group_index;
        }
        cre2_named_groups_iter_delete(iter);
    }

    // Create a set, add some simple patterns and compile it (uses cre2_set_compile as required)
    cre2_set *set = cre2_set_new(opt, (cre2_anchor_t)0);
    if (set) {
        cre2_set_add_simple(set, "abc");
        cre2_set_add_simple(set, "def");
        int compile_rc = cre2_set_compile(set);
        (void)compile_rc; // accept either success or failure for this test sequence
        cre2_set_delete(set);
    }

    // Clean up regexp and options
    cre2_delete(re);
    cre2_opt_delete(opt);

    return 66;
}