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
//<ID> 253
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // Create options and query the one_line flag (using the required API).
    cre2_options_t *opt = cre2_opt_new();
    if (!opt) {
        // If options couldn't be created, nothing to clean up; treat as success for this test.
        return 66;
    }
    int one_line_flag = cre2_opt_one_line(opt);
    (void)one_line_flag; // silence unused warning

    // Prepare a pattern (with two capturing groups) and compile it into a cre2_regexp_t.
    const char *pattern = "(a)(b)";
    int pattern_len = 6; // length of "(a)(b)"
    cre2_regexp_t *re = cre2_new(pattern, pattern_len, opt);

    // If compilation succeeded, query the number of capturing groups (required API).
    int num_groups = 0;
    if (re) {
        num_groups = cre2_num_capturing_groups(re);
    }
    (void)num_groups; // silence unused warning

    // Create a set, add the pattern, and compile the set (use of cre2_set_compile is required).
    cre2_set *set = cre2_set_new(opt, (cre2_anchor_t)0);
    if (set) {
        cre2_set_add_simple(set, pattern);
        int set_compile_result = cre2_set_compile(set);
        (void)set_compile_result; // silence unused warning
    }

    // Prepare input text and a match container, then run a partial match using the required API.
    cre2_string_t input = { "zabz", 4 };
    cre2_string_t match = { NULL, 0 };
    int partial_res = cre2_partial_match(pattern, &input, &match, 10);
    (void)partial_res; // silence unused warning
    (void)match; // match may be populated by the library; avoid unused-warning

    // Clean up resources created above.
    if (re) cre2_delete(re);
    if (set) cre2_set_delete(set);
    cre2_opt_delete(opt);

    return 66;
}