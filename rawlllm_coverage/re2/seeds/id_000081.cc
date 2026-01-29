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
//<ID> 81
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // Create options object
    cre2_options_t *opt = cre2_opt_new();
    if (!opt) {
        return 1;
    }

    // Enable logging of errors
    cre2_opt_set_log_errors(opt, 1);

    // Ensure literal flag behavior: set off, query, then set on and query
    cre2_opt_set_literal(opt, 0);
    int lit_before = cre2_opt_literal(opt);
    (void)lit_before;

    cre2_opt_set_literal(opt, 1);
    int lit_after = cre2_opt_literal(opt);
    (void)lit_after;

    // Compile a simple pattern with one capturing group
    const char pattern[] = "a(b)c";
    int pattern_len = (int)(sizeof(pattern) - 1);
    cre2_regexp_t *re = cre2_new(pattern, pattern_len, opt);
    if (!re) {
        cre2_opt_delete(opt);
        return 2;
    }

    // Query number of capturing groups (should be 1 for "a(b)c")
    int num_groups = cre2_num_capturing_groups(re);
    (void)num_groups;

    // Clean up
    cre2_delete(re);
    cre2_opt_delete(opt);

    return 66;
}