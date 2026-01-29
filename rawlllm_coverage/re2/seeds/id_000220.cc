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
//<ID> 220
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
        return 1;
    }

    // Query library interface version and an option getter to exercise those APIs
    int ver = cre2_version_interface_current();
    (void)ver; // use variable to avoid unused-warning

    int never_capture_flag = cre2_opt_never_capture(opt);
    (void)never_capture_flag;

    // Create a set with the options
    cre2_set *set = cre2_set_new(opt, CRE2_ANCHOR_BOTH);
    if (!set) {
        cre2_opt_delete(opt);
        return 1;
    }

    // Add a simple valid pattern
    char errbuf[128] = {0};
    cre2_set_add(set, "abc", (size_t)3, errbuf, sizeof(errbuf));

    // Add an intentionally-bad pattern to exercise the error buffer path
    // Pattern "([)" is invalid (unclosed parenthesis)
    char errbuf2[128] = {0};
    cre2_set_add(set, "([)", (size_t)3, errbuf2, sizeof(errbuf2));
    // We won't rely on the return value semantics here; just ensure the call is exercised.
    (void)errbuf2;

    // Build a regexp with a named group to exercise the named-groups iterator
    const char *pattern = "(?P<name>abc)";
    int pattern_len = 13; // length of "(?P<name>abc)"
    cre2_regexp_t *re = cre2_new(pattern, pattern_len, opt);
    if (re) {
        cre2_named_groups_iter_t *iter = cre2_named_groups_iter_new(re);
        if (iter) {
            const char *gname = NULL;
            int gindex = 0;
            // iterate through any named groups (may be zero or more)
            while (cre2_named_groups_iter_next(iter, &gname, &gindex)) {
                // use values to avoid unused warnings
                if (gname == NULL) break;
                (void)gindex;
            }
            cre2_named_groups_iter_delete(iter);
        }
        cre2_delete(re);
    }

    // Clean up set and options
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    return 66;
}