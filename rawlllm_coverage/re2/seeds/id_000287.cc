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
//<ID> 287
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    cre2_options_t *opt = cre2_opt_new();
    cre2_set *set = nullptr;
    if (opt) {
        // Use a default anchor value (0) cast to cre2_anchor_t to create the set.
        set = cre2_set_new(opt, (cre2_anchor_t)0);
    }

    const char *pattern = "(foo)|(bar)";
    size_t pattern_len = strlen(pattern);

    // Prepare an error buffer for cre2_set_add.
    char errbuf[128];
    memset(errbuf, 0, sizeof(errbuf));

    // Try to add the pattern to the set if the set was created.
    if (set) {
        // cre2_set_add returns non-zero on error; we ignore the result for this test.
        (void)cre2_set_add(set, pattern, pattern_len, errbuf, sizeof(errbuf));
    }

    // Compile a regexp using the same options.
    cre2_regexp_t *re = nullptr;
    if (opt) {
        re = cre2_new(pattern, (int)pattern_len, opt);
    }

    // If compilation succeeded, delete the regexp (required API usage).
    if (re) {
        cre2_delete(re);
        re = nullptr;
    }

    // Clean up the set (required API usage).
    if (set) {
        cre2_set_delete(set);
        set = nullptr;
    }

    // Clean up options if created.
    if (opt) {
        cre2_opt_delete(opt);
        opt = nullptr;
    }

    return 66;
}