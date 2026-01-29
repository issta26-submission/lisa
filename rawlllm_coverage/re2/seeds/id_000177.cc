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
//<ID> 177
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // Create options and enable logging of errors.
    cre2_options_t *opt = cre2_opt_new();
    if (!opt) {
        return 1;
    }
    cre2_opt_set_log_errors(opt, 1);
    int log_errors = cre2_opt_log_errors(opt);
    (void)log_errors; // use the value to avoid unused-variable warnings

    // Create a set with the options.
    cre2_set *set = cre2_set_new(opt, CRE2_UNANCHORED);

    // Compile a simple pattern with a named capturing group.
    const char pattern[] = "(?P<name>abc)";
    int plen = (int)(sizeof(pattern) - 1);
    cre2_regexp_t *re = cre2_new(pattern, plen, opt);

    if (re) {
        // Iterate named groups using the iterator API.
        cre2_named_groups_iter_t *iter = cre2_named_groups_iter_new(re);
        if (iter) {
            const char *group_name = NULL;
            int group_index = -1;
            while (cre2_named_groups_iter_next(iter, &group_name, &group_index)) {
                // Basic sanity checks (no heavy logic; just ensure values are accessible).
                if (group_name == NULL) {
                    // continue if name unexpectedly NULL
                    continue;
                }
                if (group_index < 0) {
                    // continue if index unexpected
                    continue;
                }
                // (In a real test we might record or verify these values.)
            }
            // delete iterator if available
            cre2_named_groups_iter_delete(iter);
        }

        // Delete compiled regexp.
        cre2_delete(re);
    }

    // Clean up set and options.
    if (set) {
        cre2_set_delete(set);
    }
    cre2_opt_delete(opt);

    return 66;
}