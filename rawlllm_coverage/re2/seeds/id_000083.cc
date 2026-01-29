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
//<ID> 83
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

    // Read the current one_line setting, then enable one_line and case sensitivity.
    int before_one_line = cre2_opt_one_line(opt);
    (void)before_one_line; // unused but demonstrates read usage

    cre2_opt_set_one_line(opt, 1);
    int after_one_line = cre2_opt_one_line(opt);
    (void)after_one_line;

    cre2_opt_set_case_sensitive(opt, 1);

    // Build a simple pattern and compile a regexp with the options.
    const char *pattern = "Hello";
    int pattern_len = 5; // length of "Hello"

    cre2_regexp_t *re = cre2_new(pattern, pattern_len, opt);
    if (!re) {
        cre2_opt_delete(opt);
        return 2;
    }

    // Check for any compile errors
    int err = cre2_error_code(re);
    if (err != 0) {
        cre2_delete(re);
        cre2_opt_delete(opt);
        return 3;
    }

    // Use cre2_easy_match to match the pattern against some text.
    const char *text = "Hello, world";
    int text_len = 12; // length of "Hello, world"

    cre2_string_t match[1];
    match[0].data = nullptr;
    match[0].length = 0;

    int matched = cre2_easy_match(pattern, pattern_len, text, text_len, match, 1);
    (void)matched; // we don't require a specific outcome for this test

    // Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    return 66;
}