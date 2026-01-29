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
//<ID> 114
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // Create options and configure POSIX syntax
    cre2_options_t *opt = cre2_opt_new();
    if (!opt) return -1;

    cre2_opt_set_posix_syntax(opt, 1);

    // Query whether case sensitivity is enabled (just to exercise the API)
    int case_sensitive = cre2_opt_case_sensitive(opt);
    (void)case_sensitive; // silence unused-variable warnings

    // Create a set with these options (anchor 0 used as a default)
    cre2_set *set = cre2_set_new(opt, (cre2_anchor_t)0);
    if (!set) {
        cre2_opt_delete(opt);
        return -1;
    }

    // Add a simple pattern to the set and compile it
    cre2_set_add_simple(set, "(a)(b)");
    int compile_result = cre2_set_compile(set);
    if (compile_result == 0) {
        cre2_set_delete(set);
        cre2_opt_delete(opt);
        return -1;
    }

    // Create a regexp directly and query number of capturing groups
    const char *pattern = "(a)(b)";
    int pattern_len = (int)(sizeof("(a)(b)") - 1);
    cre2_regexp_t *re = cre2_new(pattern, pattern_len, opt);
    if (!re) {
        cre2_set_delete(set);
        cre2_opt_delete(opt);
        return -1;
    }

    int num_groups = cre2_num_capturing_groups(re);
    if (num_groups < 2) {
        // Unexpected; clean up and fail
        cre2_delete(re);
        cre2_set_delete(set);
        cre2_opt_delete(opt);
        return -1;
    }

    // Clean up resources
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    return 66;
}