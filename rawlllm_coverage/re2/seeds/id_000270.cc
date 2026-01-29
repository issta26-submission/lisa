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
//<ID> 270
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    cre2_options_t *opt = cre2_opt_new();
    if (!opt) return 0;

    /* Configure options using setters, then verify with the requested getters */
    cre2_opt_set_posix_syntax(opt, 1);
    cre2_opt_set_longest_match(opt, 0);
    cre2_opt_set_dot_nl(opt, 1);

    int posix = cre2_opt_posix_syntax(opt);
    int longest = cre2_opt_longest_match(opt);
    int dotnl = cre2_opt_dot_nl(opt);

    if (posix != 1 || longest != 0 || dotnl != 1) {
        cre2_opt_delete(opt);
        return 0;
    }

    /* Pattern with two named capture groups */
    const char pattern_literal[] = "(?P<first>\\w+)-(?P<second>\\d+)";
    const char *pattern = pattern_literal;
    int pattern_len = (int)(sizeof(pattern_literal) - 1);

    cre2_regexp_t *re = cre2_new(pattern, pattern_len, opt);
    if (!re) {
        cre2_opt_delete(opt);
        return 0;
    }

    /* Iterate over named groups using the required iterator API */
    cre2_named_groups_iter_t *iter = cre2_named_groups_iter_new(re);
    if (iter) {
        const char *name = NULL;
        int index = -1;
        int named_count = 0;
        while (cre2_named_groups_iter_next(iter, &name, &index)) {
            /* Basic sanity checks on returned values */
            if (name == NULL) {
                /* continue but note unexpected state */
            }
            if (index < 0) {
                /* continue but note unexpected state */
            }
            ++named_count;
        }
        cre2_named_groups_iter_delete(iter);
        (void)named_count; /* suppress unused warning if any */
    }

    cre2_delete(re);
    cre2_opt_delete(opt);

    return 66;
}