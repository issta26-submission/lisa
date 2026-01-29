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
//<ID> 254
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // Create options and enable Perl character classes.
    cre2_options_t *opt = cre2_opt_new();
    if (!opt) {
        return 66;
    }
    cre2_opt_set_perl_classes(opt, 1);
    // Use the getter as requested.
    int perl_flag = cre2_opt_perl_classes(opt);
    (void)perl_flag; // silence unused-var if not otherwise inspected

    // Use a pattern with two named capturing groups.
    const char pattern[] = "(?P<foo>abc)(?P<bar>def)";
    int pattern_len = (int)(sizeof(pattern) - 1);

    // Compile the regexp with the options.
    cre2_regexp_t *re = cre2_new(pattern, pattern_len, opt);

    // Prepare variables for error inspection.
    const char *errstr = NULL;
    cre2_string_t errarg = { NULL, 0 };

    if (re == NULL) {
        // If compilation failed, try to obtain an error string via the regexp pointer.
        // (Some implementations may return NULL re; defensively check.)
        errstr = cre2_error_string(re);
        if (errstr) {
            cre2_error_arg(re, &errarg);
        }
        // Clean up options and return success code per spec.
        cre2_opt_delete(opt);
        return 66;
    }

    // If compilation succeeded, still call error string / arg to satisfy usage.
    errstr = cre2_error_string(re);
    cre2_error_arg(re, &errarg);

    // Iterate over named capturing groups using the iterator API.
    cre2_named_groups_iter_t *iter = cre2_named_groups_iter_new(re);
    if (iter != NULL) {
        const char *name = NULL;
        int index = 0;
        // Call the iterator next function repeatedly.
        while (cre2_named_groups_iter_next(iter, &name, &index)) {
            // Use the returned name and index in a no-op way to avoid unused warnings.
            (void)name;
            (void)index;
        }
        cre2_named_groups_iter_delete(iter);
    }

    // Request a possible match range for the compiled regexp.
    cre2_string_t min_range = { NULL, 0 };
    cre2_string_t max_range = { NULL, 0 };
    int maxlen = 10;
    int pmr = cre2_possible_match_range(re, &min_range, &max_range, maxlen);
    (void)pmr;
    (void)min_range;
    (void)max_range;

    // Clean up compiled regexp and options.
    cre2_delete(re);
    cre2_opt_delete(opt);

    return 66;
}