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
//<ID> 92
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

    // Configure options using required APIs
    cre2_opt_set_literal(opt, 0);          // treat pattern as a regular regexp (not literal)
    cre2_opt_set_case_sensitive(opt, 1);   // make matching case-sensitive

    // Use the getter as requested (value may be unused)
    int one_line_flag = cre2_opt_one_line(opt);
    (void)one_line_flag;

    // Create a set with these options
    cre2_set *set = cre2_set_new(opt, CRE2_UNANCHORED);
    if (!set) {
        cre2_opt_delete(opt);
        return 2;
    }

    // Build a regexp with two capturing groups "(a)(b)"
    const char pattern[] = "(a)(b)";
    cre2_regexp_t *re = cre2_new(pattern, (int)(sizeof(pattern) - 1), opt);
    if (!re) {
        cre2_set_delete(set);
        cre2_opt_delete(opt);
        return 3;
    }

    // Query number of capturing groups
    int groups = cre2_num_capturing_groups(re);
    if (groups != 2) {
        // cleanup and fail if unexpected
        cre2_delete(re);
        cre2_set_delete(set);
        cre2_opt_delete(opt);
        return 4;
    }

    // Clean up all resources
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // Success
    return 66;
}