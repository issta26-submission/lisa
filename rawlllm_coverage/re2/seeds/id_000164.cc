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
//<ID> 164
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    cre2_options_t *opt = cre2_opt_new();
    if (!opt) return 1;

    // Enable Perl character classes via the options (required API use).
    cre2_opt_set_perl_classes(opt, 1);

    // Optionally set some flags to exercise getters (use the getter APIs as required).
    // Use setters if available to set known values first.
    cre2_opt_set_log_errors(opt, 1);
    cre2_opt_set_one_line(opt, 0);

    int log_errors_flag = cre2_opt_log_errors(opt); // required API: getter
    int one_line_flag = cre2_opt_one_line(opt);     // required API: getter

    (void)log_errors_flag; // silence unused-variable warnings if any
    (void)one_line_flag;

    // Create a regex with a named capturing group and a numeric group.
    const char *pattern = "(?P<name>\\w+)-(\\d+)";
    int pattern_len = (int)strlen(pattern);
    cre2_regexp_t *re = cre2_new(pattern, pattern_len, opt);
    if (!re) {
        cre2_opt_delete(opt);
        return 2;
    }

    // Create a named-groups iterator (required API).
    cre2_named_groups_iter_t *iter = cre2_named_groups_iter_new(re);
    if (iter) {
        // We don't need to iterate here; just ensure we can create and delete it.
        cre2_named_groups_iter_delete(iter);
    }

    // Prepare text to match.
    const char *text = "alice-1234 and bob-42";
    int textlen = (int)strlen(text);

    // Prepare match array (nmatch = 3: whole match + 2 capturing groups).
    const int nmatch = 3;
    cre2_string_t matches[nmatch];
    for (int i = 0; i < nmatch; ++i) {
        matches[i].data = nullptr;
        matches[i].length = 0;
    }

    // Perform a match (required API).
    int matched = cre2_match(re, text, textlen, 0, textlen, CRE2_UNANCHORED, matches, nmatch);

    // If matched, do a simple sanity check on returned lengths.
    if (matched > 0) {
        for (int i = 0; i < nmatch && i < matched; ++i) {
            // Ensure reported lengths are non-negative and data pointer is not absurd.
            if (matches[i].length < 0) {
                cre2_delete(re);
                cre2_opt_delete(opt);
                return 3;
            }
        }
    }

    // Clean up.
    cre2_delete(re);
    cre2_opt_delete(opt);

    return 66;
}