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
//<ID> 107
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

    // Set literal mode (required API use)
    cre2_opt_set_literal(opt, 1);

    // Query dot_nl option (required API use)
    int dotnl_before = cre2_opt_dot_nl(opt);
    (void)dotnl_before; // silence unused warning if any

    // Create a set with a specific anchor (required API use)
    cre2_set *set = cre2_set_new(opt, CRE2_ANCHOR_START);
    if (!set) {
        cre2_opt_delete(opt);
        return 2;
    }

    // Add a simple pattern to the set and compile it.
    // Use add_simple to avoid needing error buffers.
    cre2_set_add_simple(set, "hello");
    cre2_set_compile(set);

    // Create a regexp with one capturing group to exercise cre2_num_capturing_groups.
    const char pattern[] = "a(b)c";
    int pat_len = (int)(sizeof(pattern) - 1);
    cre2_regexp_t *re = cre2_new(pattern, pat_len, opt);
    if (re) {
        int num_groups = cre2_num_capturing_groups(re);
        (void)num_groups; // use result (no-op) to avoid warnings
        cre2_delete(re);
    }

    // Clean up
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    return 66;
}