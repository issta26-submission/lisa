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
//<ID> 104
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    cre2_options_t *opt = cre2_opt_new();
    if (!opt) return 1;

    // Ask the options object if one_line is set initially.
    int initial_one_line = cre2_opt_one_line(opt);

    // Set the never-capture flag (required to be used per instruction).
    cre2_opt_set_never_capture(opt, 1);

    // Toggle one_line on using the setter and verify via the getter.
    cre2_opt_set_one_line(opt, 1);
    int one_line_now = cre2_opt_one_line(opt);

    // Create a set using these options. Pass 0 for anchor (default/no anchor).
    cre2_set *set = cre2_set_new(opt, (cre2_anchor_t)0);
    if (!set) {
        cre2_opt_delete(opt);
        return 2;
    }

    // Add simple patterns to the set (uses required API cre2_set_add_simple).
    int add1 = cre2_set_add_simple(set, "simple");
    int add2 = cre2_set_add_simple(set, "group(\\d+)"); // contains a capturing group

    // Even if adds fail, continue to attempt compile and cleanup.
    (void)add1;
    (void)add2;

    // Compile the set (ignore return for fuzzing-style usage).
    cre2_set_compile(set);

    // Build a regexp directly (using the same options). Pattern includes a capturing group.
    const char *pattern = "a(b)c";
    int pat_len = (int)strlen(pattern);
    cre2_regexp_t *re = cre2_new(pattern, pat_len, opt);
    if (!re) {
        // cleanup
        cre2_set_delete(set);
        cre2_opt_delete(opt);
        return 3;
    }

    // Query the number of capturing groups (required API usage).
    int num_groups = cre2_num_capturing_groups(re);
    (void)num_groups; // in this test we don't assert on value, just exercise the API

    // Cleanup all allocated objects.
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // Return success code as requested.
    return 66;
}