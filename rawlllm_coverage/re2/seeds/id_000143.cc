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
//<ID> 143
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // Create and configure options
    cre2_options_t *opt = cre2_opt_new();
    if (!opt) {
        return 66;
    }

    // Enable literal mode and read it back
    cre2_opt_set_literal(opt, 1);
    int literal_flag = cre2_opt_literal(opt);
    (void)literal_flag; // use the value to avoid unused-variable warnings

    // Build a regex using the options
    const char pattern[] = "(a)(b)c";
    int pattern_len = (int)(sizeof(pattern) - 1);
    cre2_regexp_t *re = cre2_new(pattern, pattern_len, opt);

    // Query number of capturing groups (behavior may vary with literal option)
    if (re) {
        int groups = cre2_num_capturing_groups(re);
        (void)groups; // just exercise the API
        cre2_delete(re);
        re = nullptr;
    }

    // Create a set using the same options, add a simple pattern, then delete the set
    cre2_set *set = cre2_set_new(opt, (cre2_anchor_t)0);
    if (set) {
        int add_result = cre2_set_add_simple(set, "abc");
        (void)add_result;
        cre2_set_delete(set);
        set = nullptr;
    }

    // Delete the options
    cre2_opt_delete(opt);
    opt = nullptr;

    return 66;
}