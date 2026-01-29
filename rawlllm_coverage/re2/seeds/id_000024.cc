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
//<ID> 24
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
        return 1;
    }

    // Ensure literal flag is set to a known value (use setter then getter).
    cre2_opt_set_literal(opt, 0);
    int literal_flag = cre2_opt_literal(opt);
    (void)literal_flag; // silence unused-variable warnings

    // Pattern with named capturing groups to exercise named-groups iterator.
    const char pattern[] = "(?P<g1>ab)(?P<g2>cd)";
    int pattern_len = (int)(sizeof(pattern) - 1);

    // Compile regexp
    cre2_regexp_t *re = cre2_new(pattern, pattern_len, opt);

    // If compilation succeeded, iterate over named groups
    if (re) {
        cre2_named_groups_iter_t *iter = cre2_named_groups_iter_new(re);
        if (iter) {
            const char *name = NULL;
            int index = 0;
            // Walk through available named groups
            while (cre2_named_groups_iter_next(iter, &name, &index)) {
                // Basic sanity checks on returned values
                if (name == NULL) {
                    // Unexpected, but continue iterating
                    continue;
                }
                if (index < 0) {
                    // Unexpected index, continue
                    continue;
                }
                // (No further action required for this test function)
            }
            cre2_named_groups_iter_delete(iter);
        }
        cre2_delete(re);
    }

    // Clean up options
    cre2_opt_delete(opt);

    // Return success if we were able to create options and (optionally) compile the regex.
    return (opt != NULL && re != NULL) ? 66 : 1;
}