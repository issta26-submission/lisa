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
//<ID> 19
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // Create options for set and regex creation
    cre2_options_t *opt = cre2_opt_new();
    if (!opt) {
        return 1; // failure to allocate options
    }

    // Create a set with the provided options
    cre2_set *set = cre2_set_new(opt, CRE2_UNANCHORED);
    if (set) {
        // Add a couple of simple patterns
        cre2_set_add_simple(set, "foo");
        cre2_set_add_simple(set, "a(b)c");
        // Compile the set (required API)
        cre2_set_compile(set);
    }

    // Create a regexp object to use with cre2_replace_re and named groups iterator
    const char *pattern = "a(b)c";
    cre2_regexp_t *rex = cre2_new(pattern, (int)strlen(pattern), opt);

    // Prepare strings for replacement API
    cre2_string_t text_and_target = { "xabcx", 5 };
    cre2_string_t rewrite = { "Z$1", 3 };

    // Use cre2_replace_re if regex was created
    if (rex) {
        (void)cre2_replace_re(rex, &text_and_target, &rewrite);
    }

    // Create and then delete a named-groups iterator (ensure we call the delete API)
    cre2_named_groups_iter_t *iter = nullptr;
    if (rex) {
        iter = cre2_named_groups_iter_new(rex);
    }
    if (iter) {
        cre2_named_groups_iter_delete(iter); // required API usage
    }

    // Cleanup created objects
    if (rex) cre2_delete(rex);
    if (set) cre2_set_delete(set);
    cre2_opt_delete(opt);

    // Indicate success as requested
    return 66;
}