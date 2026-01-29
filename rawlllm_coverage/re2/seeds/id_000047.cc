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
//<ID> 47
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    cre2_options_t *opt = cre2_opt_new();
    if (!opt) return -1;

    // Enable Perl character classes in options
    cre2_opt_set_perl_classes(opt, 1);

    // Create a set with the options; pass a default anchor value of 0
    cre2_set *set = cre2_set_new(opt, (cre2_anchor_t)0);
    if (!set) {
        cre2_opt_delete(opt);
        return -1;
    }

    // Add a couple of simple patterns to the set
    cre2_set_add_simple(set, "foo");
    cre2_set_add_simple(set, "bar.*baz");

    // Compile the set (ignore numeric return semantics here; call required API)
    cre2_set_compile(set);

    // Create a regular expression using the same options to exercise named-groups iterator
    const char *pattern = "(?P<name>abc)";
    cre2_regexp_t *re = cre2_new(pattern, (int)strlen(pattern), opt);
    if (re) {
        // Create a named-groups iterator for the regexp and iterate if any
        cre2_named_groups_iter_t *iter = cre2_named_groups_iter_new(re);
        if (iter) {
            const char *gname = nullptr;
            int gindex = 0;
            while (cre2_named_groups_iter_next(iter, &gname, &gindex)) {
                // iterate through named groups (no-op; just exercise API)
                (void)gname;
                (void)gindex;
            }
            cre2_named_groups_iter_delete(iter);
        }

        // Delete the regexp (required API use)
        cre2_delete(re);
    }

    // Clean up set and options
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    return 66;
}