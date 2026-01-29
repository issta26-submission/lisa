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
//<ID> 240
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // Create options and configure longest-match mode.
    cre2_options_t *opt = cre2_opt_new();
    if (!opt) return 1;

    // Enable longest match behavior.
    cre2_opt_set_longest_match(opt, 1);

    // Query the never-capture flag (getter) to exercise that API.
    int never_capture_flag = cre2_opt_never_capture(opt);
    (void)never_capture_flag; // silence unused-variable warnings

    // Prepare a pattern and create a compiled regexp using the options.
    static const char pattern[] = "a(b)c";
    int pattern_len = (int)(sizeof(pattern) - 1);
    cre2_regexp_t *re = cre2_new(pattern, pattern_len, opt);
    if (!re) {
        cre2_opt_delete(opt);
        return 2;
    }

    // Prepare input text as cre2_string_t.
    static const char text[] = "xxabcxx";
    cre2_string_t input = { text, (int)(sizeof(text) - 1) };

    // Prepare placeholders for outputs / rewrites.
    cre2_string_t rewrite = { nullptr, 0 };
    cre2_string_t target = { nullptr, 0 };

    // Use the RE-based find-and-consume call.
    // nmatch set to 10 to allow multiple groups if present.
    int r1 = cre2_find_and_consume_re(re, &input, &rewrite, 10);
    (void)r1; // accept result for fuzzing/testing; don't fail on it here

    // Use the pattern-string find-and-consume call (non-compiled).
    int r2 = cre2_find_and_consume(pattern, &input, &target, 10);
    (void)r2;

    // Attempt a full-match using the compiled regexp.
    // Note: full match takes a const cre2_string_t* for the subject.
    int r3 = cre2_full_match_re(re, &input, &rewrite, 10);
    (void)r3;

    // Clean up.
    cre2_delete(re);
    cre2_opt_delete(opt);

    return 66;
}