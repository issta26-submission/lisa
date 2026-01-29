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
//<ID> 108
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // Create options for regex/set construction.
    cre2_options_t *opt = cre2_opt_new();
    if (!opt) {
        // If options couldn't be created, nothing to clean up; still treat as success for this test harness.
        return 66;
    }

    // Tweak an option to exercise option API (optional).
    cre2_opt_set_case_sensitive(opt, 1);

    // Create a pattern set using the options.
    cre2_set *set = cre2_set_new(opt, CRE2_UNANCHORED);
    if (set) {
        // Add a simple pattern and compile the set.
        cre2_set_add_simple(set, "foo[0-9]+");
        cre2_set_compile(set);
    }

    // Create a regexp object directly.
    const char *pattern = "([a-z]+)([0-9]*)";
    int pattern_len = (int)strlen(pattern);
    cre2_regexp_t *re = cre2_new(pattern, pattern_len, opt);

    if (re) {
        // Query number of capturing groups.
        int num_groups = cre2_num_capturing_groups(re);
        (void)num_groups; // silence unused variable warnings in test context

        // Prepare a cre2_string_t to receive an error argument (if any).
        cre2_string_t arg;
        arg.data = nullptr;
        arg.length = 0;
        cre2_error_arg(re, &arg);

        // Create and then delete a named-groups iterator to exercise that API.
        cre2_named_groups_iter_t *iter = cre2_named_groups_iter_new(re);
        if (iter) {
            cre2_named_groups_iter_delete(iter);
        }

        // Delete the regexp when done.
        cre2_delete(re);
    }

    // Clean up set and options.
    if (set) cre2_set_delete(set);
    cre2_opt_delete(opt);

    return 66;
}