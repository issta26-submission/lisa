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
//<ID> 305
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // Create options for compilation
    cre2_options_t *opt = cre2_opt_new();
    if (!opt) {
        return 1; // failed to create options
    }

    // Pattern to compile (use sizeof to get length without relying on strlen)
    const char pattern[] = "^a(bc+)?d{1,3}$";
    int pattern_len = (int)(sizeof(pattern) - 1);

    // Compile the regular expression with the provided options
    cre2_regexp_t *re = cre2_new(pattern, pattern_len, opt);
    if (!re) {
        cre2_opt_delete(opt);
        return 2; // compile failure
    }

    // Prepare holders for possible-match range results
    cre2_string_t min_range = { nullptr, 0 };
    cre2_string_t max_range = { nullptr, 0 };

    // Ask for possible match range (maxlen chosen reasonably)
    int rc = cre2_possible_match_range(re, &min_range, &max_range, 64);
    (void)rc; // result checked/ignored; this call exercises the API

    // Clean up compiled regexp and options
    cre2_delete(re);
    cre2_opt_delete(opt);

    // Success
    return 66;
}