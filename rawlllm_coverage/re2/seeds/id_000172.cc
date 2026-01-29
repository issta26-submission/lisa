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
//<ID> 172
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // Create options
    cre2_options_t *opt = cre2_opt_new();
    if (!opt) {
        return 66;
    }

    // Enable Perl character classes
    cre2_opt_set_perl_classes(opt, 1);

    // Set a maximum memory value (use a literal that will convert to the expected int64_t)
    cre2_opt_set_max_mem(opt, 4LL * 1024LL * 1024LL);

    // Read back the max memory value (call the required getter)
    long long maxmem = (long long)cre2_opt_max_mem(opt);
    (void)maxmem; // silence unused-variable warnings in test contexts

    // Create a set using the options
    cre2_set *set = cre2_set_new(opt, CRE2_UNANCHORED);

    // Prepare a pattern with a named capturing group
    const char pattern[] = "(?P<foo>bar)";

    // Compile a regexp using the same options
    cre2_regexp_t *re = cre2_new(pattern, (int)(sizeof(pattern) - 1), opt);

    // Query the named capturing group "foo" (use the required API)
    int group_index = -1;
    if (re) {
        group_index = cre2_find_named_capturing_groups(re, "foo");
        (void)group_index; // allow ignoring the result in this test harness
    }

    // Add the pattern to the set (if set was created) and compile the set
    if (set) {
        cre2_set_add_simple(set, pattern);
        cre2_set_compile(set);
    }

    // Clean up resources (use the required opt delete)
    if (re) {
        cre2_delete(re);
    }
    if (set) {
        cre2_set_delete(set);
    }
    cre2_opt_delete(opt);

    return 66;
}