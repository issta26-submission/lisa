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
//<ID> 337
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // Create options and configure them.
    cre2_options_t *opt = cre2_opt_new();
    if (!opt) {
        return 66; // still treat as success for this test harness
    }

    // Disable capturing (useful for tests that only need whole-match)
    cre2_opt_set_never_capture(opt, 1);

    // Also set one_line to true, then read it back using the getter.
    cre2_opt_set_one_line(opt, 1);
    int one_line_flag = cre2_opt_one_line(opt);
    (void)one_line_flag; // suppress unused-variable warnings in some builds

    // Prepare a simple pattern and text.
    const char pattern[] = "([a-z]+)"; // one capturing group (which we disabled via options)
    int pattern_len = (int)(sizeof(pattern) - 1);

    const char text[] = "abc123 xyz";
    int text_len = (int)(sizeof(text) - 1);

    // Compile the regexp with the options.
    cre2_regexp_t *re = cre2_new(pattern, pattern_len, opt);

    // Even if compilation failed, clean up and exit normally.
    if (!re) {
        cre2_opt_delete(opt);
        return 66;
    }

    // Prepare match array: slot 0 for whole match, slot 1 for first capture.
    cre2_string_t matches[2];
    for (int i = 0; i < 2; ++i) {
        matches[i].data = nullptr;
        matches[i].length = 0;
    }

    // Attempt a match over the entire text (unanchoered).
    int matched = cre2_match(re, text, text_len, 0, text_len, CRE2_UNANCHORED, matches, 2);
    (void)matched; // we don't strictly require using the result beyond exercising the API

    // Clean up resources.
    cre2_delete(re);
    cre2_opt_delete(opt);

    return 66;
}