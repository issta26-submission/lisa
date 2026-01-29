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
//<ID> 185
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
        // failed to allocate options
        return 1;
    }

    // Set longest-match flag (demonstrates setter usage)
    cre2_opt_set_longest_match(opt, 1);

    // Query the log-errors flag (demonstrates getter usage)
    int log_errors = cre2_opt_log_errors(opt);

    // Optionally toggle longest-match based on log_errors for test variety
    cre2_opt_set_longest_match(opt, log_errors ? 0 : 1);

    // Prepare a pattern as an array so we can compute its length without headers
    const char pattern[] = "(a|b)+";
    int pattern_len = (int)(sizeof(pattern) - 1); // excludes terminating NUL

    // Compile the regexp with the options
    cre2_regexp_t *re = cre2_new(pattern, pattern_len, opt);
    if (!re) {
        // cleanup and report failure
        cre2_opt_delete(opt);
        return 2;
    }

    // Destroy the compiled regexp
    cre2_delete(re);

    // Destroy the options
    cre2_opt_delete(opt);

    // Success
    return 66;
}