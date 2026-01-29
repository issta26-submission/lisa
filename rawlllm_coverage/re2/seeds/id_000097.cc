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
//<ID> 97
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    cre2_options_t *opt = cre2_opt_new();
    if (!opt) return 1;
    /* Enable dot to match newline as part of the options */
    cre2_opt_set_dot_nl(opt, 1);

    /* Create a pattern with one named and one unnamed capturing group */
    const char pattern[] = "(?P<first>foo)(bar)";
    int pattern_len = (int)(sizeof(pattern) - 1);

    cre2_regexp_t *re = cre2_new(pattern, pattern_len, opt);
    if (!re) {
        cre2_opt_delete(opt);
        return 2;
    }

    /* Query number of capturing groups */
    int ncap = cre2_num_capturing_groups(re);
    /* Basic sanity check: there should be at least two capturing groups */
    if (ncap < 2) {
        cre2_delete(re);
        cre2_opt_delete(opt);
        return 3;
    }

    /* Iterate named capturing groups (if any) */
    cre2_named_groups_iter_t *iter = cre2_named_groups_iter_new(re);
    if (iter) {
        const char *name = NULL;
        int index = -1;
        while (cre2_named_groups_iter_next(iter, &name, &index)) {
            /* simple use of returned values; avoid dereferencing if name is null */
            (void)name;
            (void)index;
        }
        /* required cleanup of the iterator */
        cre2_named_groups_iter_delete(iter);
    }

    /* Create a set with the same options and add simple patterns */
    cre2_set *set = cre2_set_new(opt, (cre2_anchor_t)0);
    if (set) {
        /* add a few simple patterns */
        cre2_set_add_simple(set, "foo");
        cre2_set_add_simple(set, "bar");
        cre2_set_add_simple(set, "(?P<first>foo)(bar)");
        cre2_set_delete(set);
    }

    /* cleanup */
    cre2_delete(re);
    cre2_opt_delete(opt);

    return 66;
}