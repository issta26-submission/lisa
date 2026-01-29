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
//<ID> 201
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    cre2_options_t *opt = cre2_opt_new();
    if (!opt) return 1;

    // Configure options: set max memory and enable Perl character classes.
    cre2_opt_set_max_mem(opt, (int64_t)32 * 1024 * 1024); // 32MB
    cre2_opt_set_perl_classes(opt, 1);

    // Create a set with the options. Use anchor value 0 (default).
    cre2_set *set = cre2_set_new(opt, (cre2_anchor_t)0);
    if (!set) {
        cre2_opt_delete(opt);
        return 2;
    }

    // Add a simple pattern with two capturing groups.
    const char *pattern = "^(a)(b)$";
    if (cre2_set_add_simple(set, pattern) != 0) {
        // treat non-zero as error from add_simple
        cre2_set_delete(set);
        cre2_opt_delete(opt);
        return 3;
    }

    // Compile the set.
    int compile_ok = cre2_set_compile(set);
    if (!compile_ok) {
        cre2_set_delete(set);
        cre2_opt_delete(opt);
        return 4;
    }

    // Also create a compiled regexp directly to query number of capturing groups.
    int pattern_len = (int)strlen(pattern);
    cre2_regexp_t *re = cre2_new(pattern, pattern_len, opt);
    if (!re) {
        cre2_set_delete(set);
        cre2_opt_delete(opt);
        return 5;
    }

    // Query number of capturing groups (should be 2 for the pattern above).
    int ngroups = cre2_num_capturing_groups(re);
    (void)ngroups; // we don't enforce exact value here, just exercise the API

    // Clean up
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    return 66;
}