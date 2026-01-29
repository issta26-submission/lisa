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
//<ID> 244
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // Create options and set the one_line flag.
    cre2_options_t *opt = cre2_opt_new();
    if (!opt) {
        // If options couldn't be created, nothing to clean up; still return success per spec.
        return 66;
    }

    // Enable one-line mode and verify via the getter.
    cre2_opt_set_one_line(opt, 1);
    int one_line_enabled = cre2_opt_one_line(opt);
    (void)one_line_enabled; // silence unused-warning; value could be used for branching if needed.

    // Prepare a simple pattern and create a regex with the options.
    const char pattern[] = "^hello";
    int pattern_len = (int)(sizeof(pattern) - 1); // exclude terminating NUL
    cre2_regexp_t *re = cre2_new(pattern, pattern_len, opt);

    // Prepare input and match containers.
    const char text[] = "hello world";
    cre2_string_t input = { text, (int)(sizeof(text) - 1) };
    cre2_string_t match = { nullptr, 0 };

    // Attempt to consume from the start of the input using the compiled regexp (if it was created).
    if (re) {
        // nmatch set to 1: we only expect one overall match here.
        (void)cre2_consume_re(re, &input, &match, 1);
    }

    // Demonstrate creating and deleting a pattern set using the same options.
    cre2_set *set = cre2_set_new(opt, (cre2_anchor_t)0);
    if (set) {
        // Add the same simple pattern and compile the set; ignore return codes for brevity.
        cre2_set_add_simple(set, pattern);
        cre2_set_compile(set);

        // Delete the set using the required API.
        cre2_set_delete(set);
    }

    // Clean up the regexp and options.
    if (re) cre2_delete(re);
    cre2_opt_delete(opt);

    return 66;
}