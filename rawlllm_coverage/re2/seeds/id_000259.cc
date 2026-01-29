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
//<ID> 259
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // Create options and set the literal flag.
    cre2_options_t *opt = cre2_opt_new();
    if (!opt) return 1;

    cre2_opt_set_literal(opt, 1);

    // Query the never_capture flag (getter).
    int never_capture_flag = cre2_opt_never_capture(opt);
    (void)never_capture_flag; // silence unused-variable warnings

    // Create a set using the options and then add/compile a simple pattern.
    cre2_set *set = cre2_set_new(opt, (cre2_anchor_t)0);
    if (set) {
        cre2_set_add_simple(set, "foo");
        (void)cre2_set_compile(set);
        cre2_set_delete(set);
        set = nullptr;
    }

    // Prepare a pattern with a named capturing group and compile a regexp.
    const char pattern[] = "(?P<g1>abc)";
    int pat_len = (int)(sizeof(pattern) - 1);
    cre2_regexp_t *re = cre2_new(pattern, pat_len, opt);
    if (re) {
        // Create a named-groups iterator for the compiled regexp.
        cre2_named_groups_iter_t *iter = cre2_named_groups_iter_new(re);
        if (iter) {
            const char *name = NULL;
            int index = 0;
            // Iterate over named groups (if any).
            while (cre2_named_groups_iter_next(iter, &name, &index)) {
                (void)name;
                (void)index;
            }
            cre2_named_groups_iter_delete(iter);
            iter = nullptr;
        }
        cre2_delete(re);
        re = nullptr;
    }

    // Clean up options.
    cre2_opt_delete(opt);
    opt = nullptr;

    return 66;
}