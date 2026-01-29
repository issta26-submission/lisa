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
//<ID> 11
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    cre2_options_t *opt = cre2_opt_new();
    if (!opt) return -1;

    /* Set the literal option as required by the task. */
    cre2_opt_set_literal(opt, 1);

    /* Create a regex with a named capturing group. */
    const char *pattern = "(?P<name>abc)(def)";
    cre2_regexp_t *re = cre2_new(pattern, (int)strlen(pattern), opt);
    if (!re) {
        cre2_opt_delete(opt);
        return -1;
    }

    /* Iterate over named groups (if any) using the iterator API. */
    cre2_named_groups_iter_t *iter = cre2_named_groups_iter_new(re);
    if (iter) {
        const char *grp_name = nullptr;
        int grp_index = 0;
        while (cre2_named_groups_iter_next(iter, &grp_name, &grp_index)) {
            /* Use the returned name/index in a trivial way to exercise the API. */
            if (grp_name == nullptr) {
                /* continue if unexpected null name */
                continue;
            }
            /* (No-op) */
            (void)grp_index;
        }
        /* Required: delete the iterator when done. */
        cre2_named_groups_iter_delete(iter);
    }

    /* Create a set, add a simple pattern, and compile it. */
    cre2_set *set = cre2_set_new(opt, (cre2_anchor_t)0);
    if (!set) {
        cre2_delete(re);
        cre2_opt_delete(opt);
        return -1;
    }

    /* Add a simple pattern to the set. */
    (void)cre2_set_add_simple(set, "abc");

    /* Compile the set (required API usage). */
    int compile_result = cre2_set_compile(set);
    (void)compile_result; /* ignore specific result for this test */

    /* Cleanup all allocated objects. */
    cre2_set_delete(set);
    cre2_delete(re);
    cre2_opt_delete(opt);

    return 66;
}