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
//<ID> 318
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // Create options
    cre2_options_t *opt = cre2_opt_new();
    if (!opt) {
        return 1; // failed to allocate options
    }

    // Set word boundary flag using required API
    cre2_opt_set_word_boundary(opt, 1);

    // Query the log_errors flag using required API (store/use to avoid unused-variable warnings)
    int log_errors = cre2_opt_log_errors(opt);

    // Prepare a pattern and create a regexp using the options
    char pattern[] = "^(foo)(bar)(baz)?$";
    int pattern_len = (int)(sizeof(pattern) - 1);

    cre2_regexp_t *re = cre2_new(pattern, pattern_len, opt);
    if (!re) {
        cre2_opt_delete(opt);
        return 2; // failed to compile regexp
    }

    // Use required API to get number of capturing groups
    int num_groups = cre2_num_capturing_groups(re);

    // Basic sanity checks to ensure values are reasonable (not strictly necessary)
    if (num_groups < 0) {
        // cleanup and report failure
        cre2_delete(re);
        cre2_opt_delete(opt);
        return 3;
    }

    // Clean up
    cre2_delete(re);
    cre2_opt_delete(opt);

    // Use read values somehow (no-op cast) to avoid compiler warnings
    (void)log_errors;
    (void)num_groups;

    return 66;
}