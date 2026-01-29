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
//<ID> 131
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

    // Configure options: enable word boundary behavior and disable logging of errors
    cre2_opt_set_word_boundary(opt, 1);
    cre2_opt_set_log_errors(opt, 0);

    // Set and retrieve max memory for sanity check
    cre2_opt_set_max_mem(opt, (int64_t)1024);
    int64_t max_mem = cre2_opt_max_mem(opt);
    if (max_mem <= 0) {
        cre2_opt_delete(opt);
        return 2;
    }

    // Build a pattern with a named capturing group
    const char pattern[] = "(?P<name>ab)c";
    int pattern_len = (int)(sizeof(pattern) - 1);

    // Compile the regular expression with the options
    cre2_regexp_t *re = cre2_new(pattern, pattern_len, opt);
    if (!re) {
        cre2_opt_delete(opt);
        return 3;
    }

    // Create an iterator over named capturing groups
    cre2_named_groups_iter_t *iter = cre2_named_groups_iter_new(re);
    if (iter) {
        // Iterate (if any) through named groups to exercise the iterator API
        const char *gname = nullptr;
        int gindex = 0;
        while (cre2_named_groups_iter_next(iter, &gname, &gindex)) {
            // In a real test we might inspect gname and gindex; here we just iterate.
            (void)gname;
            (void)gindex;
        }
        // Clean up iterator
        cre2_named_groups_iter_delete(iter);
    }

    // Clean up compiled regexp and options
    cre2_delete(re);
    cre2_opt_delete(opt);

    // Success
    return 66;
}