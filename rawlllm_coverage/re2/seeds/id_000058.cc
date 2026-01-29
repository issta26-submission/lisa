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
//<ID> 58
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // Create options and enable "never capture"
    cre2_options_t *opt = cre2_opt_new();
    if (!opt) return -1;
    cre2_opt_set_never_capture(opt, 1);

    // Verify getter works (value not strictly required, but call it)
    int never_capture_flag = cre2_opt_never_capture(opt);
    (void)never_capture_flag;

    // Prepare a pattern and compile it with the options
    const char *pattern = "a(b)c";
    int pattern_len = 5; // length of "a(b)c" without the terminating NUL
    cre2_regexp_t *re = cre2_new(pattern, pattern_len, opt);
    if (!re) {
        cre2_opt_delete(opt);
        return -2;
    }

    // Prepare text and match storage
    cre2_string_t text = { "abc", 3 };
    cre2_string_t full_matches[2] = { {NULL, 0}, {NULL, 0} };
    cre2_string_t partial_matches[2] = { {NULL, 0}, {NULL, 0} };

    // Use the compiled regex to perform a full match
    int full_result = cre2_full_match_re(re, &text, full_matches, 2);
    (void)full_result; // accept result, just ensure API is exercised

    // Use the convenience partial match API (pattern as C string)
    int partial_result = cre2_partial_match(pattern, &text, partial_matches, 2);
    (void)partial_result;

    // Clean up
    cre2_delete(re);
    cre2_opt_delete(opt);

    return 66;
}