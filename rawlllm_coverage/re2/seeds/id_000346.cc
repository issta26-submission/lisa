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
//<ID> 346
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // Create options object
    cre2_options_t *opt = cre2_opt_new();
    if (!opt) return 1;

    // Set case sensitivity option (use setter)
    cre2_opt_set_case_sensitive(opt, 1);

    // Query several option getters to exercise API
    int longest = cre2_opt_longest_match(opt);
    int never_nl = cre2_opt_never_nl(opt);
    int literal = cre2_opt_literal(opt);

    // Prepare input and a small rewrite/target buffer
    cre2_string_t text = { "hello123world", 12 };
    cre2_string_t rewrite = { "X", 1 };

    // Use cre2_find_and_consume with a simple literal pattern
    int rc = cre2_find_and_consume("123", &text, &rewrite, 0);

    // Basic sanity checks to make use of returned values and avoid warnings.
    // We don't require a specific rc value for success here; just ensure APIs were callable.
    if (!opt || rc < -1000 || longest < -1000 || never_nl < -1000 || literal < -1000) {
        cre2_opt_delete(opt);
        return 2;
    }

    // Clean up
    cre2_opt_delete(opt);

    // Indicate overall success as requested
    return 66;
}