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
//<ID> 10
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    cre2_options_t *opt = cre2_opt_new();
    if (!opt) {
        return 2;
    }

    /* Query the never-capture flag (getter API required to be used) */
    int never_capture = cre2_opt_never_capture(opt);
    (void)never_capture; /* silence unused variable warning */

    /* Create a set with the options (anchor value 0 used as default) */
    cre2_set *set = cre2_set_new(opt, (cre2_anchor_t)0);
    if (set) {
        /* add a simple pattern and compile the set (best-effort) */
        cre2_set_add_simple(set, "example");
        cre2_set_compile(set);
    }

    /* Create a regexp using the same options and iterate named groups */
    const char pattern[] = "(?P<name>\\w+)-(\\d+)";
    cre2_regexp_t *re = cre2_new(pattern, (int)(sizeof(pattern) - 1), opt);
    cre2_named_groups_iter_t *iter = NULL;
    if (re) {
        iter = cre2_named_groups_iter_new(re);
        if (iter) {
            const char *name = NULL;
            int index = 0;
            while (cre2_named_groups_iter_next(iter, &name, &index)) {
                /* consume name and index (no-op, just exercising the API) */
                (void)name;
                (void)index;
            }
            /* required deletion of the iterator */
            cre2_named_groups_iter_delete(iter);
            iter = NULL;
        }

        /* required deletion of the regexp */
        cre2_delete(re);
        re = NULL;
    }

    /* required deletion of the set */
    if (set) {
        cre2_set_delete(set);
        set = NULL;
    }

    /* required deletion of the options */
    cre2_opt_delete(opt);
    opt = NULL;

    return 66;
}