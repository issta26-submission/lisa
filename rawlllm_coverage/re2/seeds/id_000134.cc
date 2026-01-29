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
//<ID> 134
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // Create options and configure them
    cre2_options_t *opt = cre2_opt_new();
    if (!opt) {
        // If options allocation failed, still follow the spec to return success code for this test harness.
        return 66;
    }
    // Use required option-setting APIs
    cre2_opt_set_never_capture(opt, 0); // allow capturing groups
    cre2_opt_set_log_errors(opt, 1);    // enable logging of errors

    // Pattern with two capturing groups
    const char *pattern = "(a)(b)";
    int pattern_len = 6; // length of "(a)(b)"

    // Compile a regexp with the options
    cre2_regexp_t *re = cre2_new(pattern, pattern_len, opt);

    // Query number of capturing groups (uses required API)
    int groups = 0;
    if (re) {
        groups = cre2_num_capturing_groups(re);
    }

    // Prepare input text and match storage for find_and_consume
    cre2_string_t text = { "ab", 2 };
    const int nmatch = 3; // whole match + 2 capturing groups
    cre2_string_t matches[nmatch];
    for (int i = 0; i < nmatch; ++i) {
        matches[i].data = NULL;
        matches[i].length = 0;
    }

    // Use the non-_re find_and_consume API with the same pattern
    int found = cre2_find_and_consume(pattern, &text, matches, nmatch);

    // Clean up
    if (re) cre2_delete(re);
    cre2_opt_delete(opt);

    // Use the gathered values somehow (avoid unused-variable warnings in some builds)
    (void)groups;
    (void)found;

    return 66;
}