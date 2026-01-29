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
//<ID> 5
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // Create options and enable logging of errors.
    cre2_options_t *opt = cre2_opt_new();
    if (!opt) return 1;
    cre2_opt_set_log_errors(opt, 1);

    // Pattern with two named capturing groups and one unnamed group.
    const char pattern[] = "(?P<first>foo)(?P<second>bar)(baz)";
    int pattern_len = (int)(sizeof(pattern) - 1);

    // Compile the regular expression.
    cre2_regexp_t *re = cre2_new(pattern, pattern_len, opt);
    if (!re) {
        cre2_opt_delete(opt);
        return 2;
    }

    // Verify number of capturing groups (should be 3).
    int num_groups = cre2_num_capturing_groups(re);
    if (num_groups != 3) {
        cre2_delete(re);
        cre2_opt_delete(opt);
        return 3;
    }

    // Create an iterator for named groups and iterate over them.
    cre2_named_groups_iter_t *iter = cre2_named_groups_iter_new(re);
    if (!iter) {
        cre2_delete(re);
        cre2_opt_delete(opt);
        return 4;
    }

    int found_named = 0;
    const char *name = NULL;
    int index = -1;
    while (cre2_named_groups_iter_next(iter, &name, &index)) {
        // Basic validation: name should be non-NULL and index within range.
        if (!name || index < 1 || index > num_groups) {
            cre2_named_groups_iter_delete(iter);
            cre2_delete(re);
            cre2_opt_delete(opt);
            return 5;
        }
        ++found_named;
    }

    // Expect exactly 2 named groups in the pattern.
    if (found_named != 2) {
        cre2_named_groups_iter_delete(iter);
        cre2_delete(re);
        cre2_opt_delete(opt);
        return 6;
    }

    // Clean up and return success code.
    cre2_named_groups_iter_delete(iter);
    cre2_delete(re);
    cre2_opt_delete(opt);
    return 66;
}