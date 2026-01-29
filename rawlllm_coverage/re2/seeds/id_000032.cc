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
//<ID> 32
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // Create options
    cre2_options_t *opt = cre2_opt_new();

    // Create a set with the options (must call cre2_set_delete later)
    cre2_set *set = nullptr;
    if (opt) {
        set = cre2_set_new(opt, CRE2_UNANCHORED);
    }

    // Create a regexp with a simple named capture group
    const char *pattern = "(?P<name>ab)c";
    int pattern_len = (int)strlen(pattern);
    cre2_regexp_t *re = cre2_new(pattern, pattern_len, opt);

    // If regexp created, create and iterate a named-groups iterator
    if (re) {
        cre2_named_groups_iter_t *iter = cre2_named_groups_iter_new(re);
        if (iter) {
            const char *name = nullptr;
            int index = 0;
            // Walk through any named groups (if present)
            while (cre2_named_groups_iter_next(iter, &name, &index)) {
                // no-op: just exercising the iterator API
                (void)name;
                (void)index;
            }
            cre2_named_groups_iter_delete(iter);
        }
    }

    // Prepare a cre2_string_t for the match result and call cre2_match
    cre2_string_t match;
    match.data = nullptr;
    match.length = 0;
    const char *text = "abc";
    int textlen = (int)strlen(text);
    if (re) {
        // ask for 1 match (entire match)
        (void)cre2_match(re, text, textlen, 0, textlen, CRE2_UNANCHORED, &match, 1);
    }

    // Cleanup: delete regexp, set and options if they were created
    if (re) cre2_delete(re);
    if (set) cre2_set_delete(set);
    if (opt) cre2_opt_delete(opt);

    return 66;
}