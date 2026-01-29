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
//<ID> 279
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
        // couldn't allocate options
        return 1;
    }

    // Set some options required by the test: word boundary and dot matches newline
    cre2_opt_set_word_boundary(opt, 1);
    cre2_opt_set_dot_nl(opt, 1);

    // Pattern with two capturing groups
    const char *pattern = "(\\w+)-(\\w+)";
    // compute pattern length without relying on external headers
    int pattern_len = 0;
    while (pattern[pattern_len] != '\0') ++pattern_len;

    // Compile the regexp with the options
    cre2_regexp_t *re = cre2_new(pattern, pattern_len, opt);

    if (re) {
        // Query number of capturing groups
        int groups = cre2_num_capturing_groups(re);
        (void)groups; // use variable to avoid compiler warnings in this test scaffold

        // Clean up regexp
        cre2_delete(re);
    } else {
        // Compilation failed; proceed to clean up options below
    }

    // Clean up options
    cre2_opt_delete(opt);

    // Indicate success for the test harness
    return 66;
}