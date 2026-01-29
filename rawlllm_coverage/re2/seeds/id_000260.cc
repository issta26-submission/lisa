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
//<ID> 260
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
    // Query the option to exercise cre2_opt_log_errors.
    int log_errors = cre2_opt_log_errors(opt);
    (void)log_errors; // silence unused-variable warning

    // Build a simple pattern and compile a regexp using the options.
    const char *pattern = "foo";
    int patlen = 3;
    cre2_regexp_t *re = cre2_new(pattern, patlen, opt);
    if (!re) {
        cre2_opt_delete(opt);
        return 2;
    }

    // Retrieve the pattern from the compiled regexp to exercise cre2_pattern.
    const char *got_pattern = cre2_pattern(re);
    if (!got_pattern) {
        cre2_delete(re);
        cre2_opt_delete(opt);
        return 3;
    }

    // Prepare a text and a rewrite string for global replacement.
    cre2_string_t text_and_target = { "foobar foo", 10 }; // "foobar foo" length = 10
    cre2_string_t rewrite = { "baz", 3 };

    // Perform a global replace using the pattern string (exercises cre2_global_replace).
    int rc = cre2_global_replace(got_pattern, &text_and_target, &rewrite);
    if (rc < 0) {
        cre2_delete(re);
        cre2_opt_delete(opt);
        return 4;
    }

    // Clean up
    cre2_delete(re);
    cre2_opt_delete(opt);

    return 66;
}