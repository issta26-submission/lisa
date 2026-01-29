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
//<ID> 181
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // Create options and enable logging of errors.
    cre2_options_t *opt = cre2_opt_new();
    if (!opt) return 1;
    cre2_opt_set_log_errors(opt, 1);

    // Check current perl_classes setting (getter must be used per requirement).
    int perl_classes_flag = cre2_opt_perl_classes(opt);
    // Optionally flip it (use setter if desired). Not required, but safe to exercise.
    // Use the setter only if available: cre2_opt_set_perl_classes(opt, perl_classes_flag ? 0 : 1);

    // Build a simple pattern with a capturing group.
    // Use compile-time known literal lengths to avoid needing strlen.
    const char *pattern = "^(a+)$";
    int pattern_len = (int)(sizeof("^(a+)$") - 1);

    // Create the regexp with the options.
    cre2_regexp_t *rex = cre2_new(pattern, pattern_len, opt);
    if (!rex) {
        cre2_opt_delete(opt);
        return 2;
    }

    // Prepare input text as a cre2_string_t and an array to hold capture groups.
    cre2_string_t text = { "aaaa", (int)(sizeof("aaaa") - 1) };
    cre2_string_t matches[2];
    matches[0].data = nullptr; matches[0].length = 0;
    matches[1].data = nullptr; matches[1].length = 0;

    // Try a full match using the compiled regexp.
    // nmatch = 2 to retrieve the whole match and the first capture group.
    int full_ok = cre2_full_match_re(rex, &text, matches, 2);
    // full_ok typically is nonzero on match; we don't treat failure as fatal here.

    // Query the possible match range for the regexp.
    cre2_string_t min_range = { nullptr, 0 };
    cre2_string_t max_range = { nullptr, 0 };
    int pmr_ok = cre2_possible_match_range(rex, &min_range, &max_range, 16);
    // pmr_ok can be used to check whether range information was provided.

    // Cleanup.
    cre2_delete(rex);
    cre2_opt_delete(opt);

    // Return success code as requested.
    return 66;
}