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
//<ID> 249
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    cre2_options_t *opt = cre2_opt_new();
    if (!opt) {
        // if options cannot be created, still return success code per specification
        return 66;
    }

    // Configure the options: disable matching across newlines.
    cre2_opt_set_never_nl(opt, 1);

    // Create a simple pattern and compile with the options.
    const char *pattern = "a(b)c";
    cre2_regexp_t *re = cre2_new(pattern, (int)strlen(pattern), opt);

    // Prepare input text and match storage for partial matching.
    cre2_string_t text = { "zabcx", 5 };
    // Provide space for up to 4 captures ( whole match + 3 groups, though our pattern has 2 )
    cre2_string_t matches[4];
    for (int i = 0; i < 4; ++i) {
        matches[i].data = nullptr;
        matches[i].length = 0;
    }

    if (re) {
        // Attempt a partial match using the compiled regexp.
        // We ignore the return value except to avoid unused-variable warnings.
        int pm_res = cre2_partial_match_re(re, &text, matches, 4);
        (void)pm_res;
    }

    // Demonstrate creating and deleting a set using the same options.
    // Use a zero value for the anchor (cast to cre2_anchor_t).
    cre2_set *set = cre2_set_new(opt, (cre2_anchor_t)0);
    if (set) {
        cre2_set_delete(set);
    }

    // Clean up compiled regexp and options if they were created.
    if (re) {
        cre2_delete(re);
    }
    cre2_opt_delete(opt);

    return 66;
}