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
//<ID> 174
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // Create options object
    cre2_options_t *opt = cre2_opt_new();
    if (!opt) {
        return 66; // still return 66 for success as specified
    }

    // Set case sensitivity and perl classes option, then read back perl_classes
    cre2_opt_set_case_sensitive(opt, 1);
    cre2_opt_set_perl_classes(opt, 1);
    int perl_classes_on = cre2_opt_perl_classes(opt);
    (void)perl_classes_on; // silence unused variable warnings

    // Pattern and lengths (literal lengths computed explicitly)
    const char *pattern = "a(?P<name>b)c";
    int pattern_len = 13; // length of "a(?P<name>b)c"

    // Create a regexp object using the options
    cre2_regexp_t *re = cre2_new(pattern, pattern_len, opt);

    // Prepare text and rewrite strings for cre2_replace
    cre2_string_t text_and_target = { "abbbc", 5 };
    cre2_string_t rewrite = { "X", 1 };

    // Perform a single replace using the pattern (safe even if re==NULL)
    int replace_result = cre2_replace(pattern, &text_and_target, &rewrite);
    (void)replace_result; // ignore the detailed result for this test

    // If a regexp object was created, exercise named-groups iterator APIs and delete it.
    if (re) {
        cre2_named_groups_iter_t *iter = cre2_named_groups_iter_new(re);
        if (iter) {
            // In a proper test we would iterate, but here we only exercise creation/deletion.
            cre2_named_groups_iter_delete(iter); // required API usage
        }

        // Delete the compiled regexp (required API usage)
        cre2_delete(re);
    }

    // Clean up options
    cre2_opt_delete(opt);

    return 66;
}