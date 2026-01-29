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
//<ID> 20
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    cre2_options_t *opt = cre2_opt_new();
    if (!opt) return 1;

    /* Configure options: enable POSIX syntax and set literal flag off */
    cre2_opt_set_posix_syntax(opt, 1);
    cre2_opt_set_literal(opt, 0);

    /* Pattern with a named capturing group */
    const char pattern[] = "(?P<name>abc)";
    int pattern_len = (int)(sizeof(pattern) - 1);

    /* Compile the regexp using the options */
    cre2_regexp_t *re = cre2_new(pattern, pattern_len, opt);
    if (!re) {
        cre2_opt_delete(opt);
        return 2;
    }

    /* Create an iterator for named capturing groups and iterate if available */
    cre2_named_groups_iter_t *iter = cre2_named_groups_iter_new(re);
    if (iter) {
        const char *gname = nullptr;
        int gindex = 0;
        while (cre2_named_groups_iter_next(iter, &gname, &gindex)) {
            /* Use the returned name and index in a harmless way */
            if (gname == nullptr || gindex < 0) {
                /* continue on unexpected values */
                continue;
            }
        }
        cre2_named_groups_iter_delete(iter);
    }

    /* Clean up compiled regexp and options */
    cre2_delete(re);
    cre2_opt_delete(opt);

    return 66;
}