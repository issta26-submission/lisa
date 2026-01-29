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
//<ID> 55
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    cre2_options_t *opt = cre2_opt_new();
    if (!opt) return 1;

    // Enable logging of errors on the options object.
    cre2_opt_set_log_errors(opt, 1);

    // Create a set using the options. Use anchor value 0 (no special anchor).
    cre2_set *set = cre2_set_new(opt, (cre2_anchor_t)0);
    if (!set) {
        cre2_opt_delete(opt);
        return 2;
    }

    // Add a simple pattern to the set.
    int add_res = cre2_set_add_simple(set, "foo");
    (void)add_res; // ignore for this test

    // Create a compiled regexp using the same options.
    const char *pattern = "bar";
    int pattern_len = (int)strlen(pattern);
    cre2_regexp_t *re = cre2_new(pattern, pattern_len, opt);

    // Prepare text/target buffer and rewrite string for cre2_replace.
    char buffer[256];
    const char *initial = "one foo two foo three";
    // Ensure null-terminated and set length accordingly.
    strncpy(buffer, initial, sizeof(buffer) - 1);
    buffer[sizeof(buffer) - 1] = '\0';
    cre2_string_t text_and_target = { buffer, (int)strlen(buffer) };
    cre2_string_t rewrite = { "X", 1 };

    // Perform a single replace using the pattern "foo".
    int replace_res = cre2_replace("foo", &text_and_target, &rewrite);
    (void)replace_res; // allow the test to continue regardless of result

    // Clean up created objects.
    if (re) cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    return 66;
}