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
//<ID> 88
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // Create options and configure flags.
    cre2_options_t *opt = cre2_opt_new();
    if (!opt) return 1;
    cre2_opt_set_perl_classes(opt, 1);
    cre2_opt_set_log_errors(opt, 0);

    // Create a pattern set using the options. Use a zero anchor value.
    cre2_set *set = cre2_set_new(opt, (cre2_anchor_t)0);
    if (!set) {
        cre2_opt_delete(opt);
        return 2;
    }

    // Add a simple pattern and compile the set.
    cre2_set_add_simple(set, "example");
    int compile_rc = cre2_set_compile(set);
    // (We don't require a specific compile result here; just exercise the API.)

    // Build a regexp to obtain a named-groups iterator, then delete the iterator.
    const char *pattern = "(?P<name>abc)";
    int pattern_len = (int)(sizeof("(?P<name>abc)") - 1);
    cre2_regexp_t *re = cre2_new(pattern, pattern_len, opt);
    if (re) {
        cre2_named_groups_iter_t *iter = cre2_named_groups_iter_new(re);
        if (iter) {
            // use the iterator only to exercise creation + deletion
            cre2_named_groups_iter_delete(iter);
        }
        cre2_delete(re);
    }

    // Clean up set and options.
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    (void)compile_rc; // silence unused variable warnings if any
    return 66;
}