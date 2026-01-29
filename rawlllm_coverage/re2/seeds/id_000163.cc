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
//<ID> 163
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    cre2_options_t *opt = cre2_opt_new();
    if (!opt) {
        return 1;
    }

    // Read current log errors flag (use of required API)
    int log_errors_before = cre2_opt_log_errors(opt);

    // Set never_capture flag (use of required API)
    cre2_opt_set_never_capture(opt, 1);

    // Create a set with these options (use of required API cre2_set_new)
    cre2_set *set = cre2_set_new(opt, CRE2_UNANCHORED);
    if (!set) {
        cre2_opt_delete(opt);
        return 2;
    }

    // Add a simple pattern to the set (use of required API)
    int add_res = cre2_set_add_simple(set, "a.*b");
    (void)add_res; // tolerate any result for this test sequence

    // Build a regexp using the same options to exercise partial match
    const char pattern[] = "a(b)?";
    cre2_regexp_t *rex = cre2_new(pattern, (int)(sizeof(pattern) - 1), opt);
    if (!rex) {
        cre2_set_delete(set);
        cre2_opt_delete(opt);
        return 3;
    }

    // Prepare input and match storage for partial matching (use of required API cre2_partial_match_re)
    const char text[] = "axxb";
    cre2_string_t input = { text, (int)(sizeof(text) - 1) };
    cre2_string_t matches[3];
    // Initialize match slots to safe defaults
    for (int i = 0; i < 3; ++i) {
        matches[i].data = nullptr;
        matches[i].length = 0;
    }

    // Attempt a partial match
    int pm = cre2_partial_match_re(rex, &input, matches, 3);
    (void)pm;
    (void)log_errors_before;

    // Cleanup
    cre2_delete(rex);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    return 66;
}