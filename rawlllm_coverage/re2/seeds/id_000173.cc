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
//<ID> 173
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    cre2_options_t *opt = cre2_opt_new();
    if (!opt) {
        return 1;
    }

    /* Set case sensitivity on the options */
    cre2_opt_set_case_sensitive(opt, 1);

    /* Create a set with the options (anchor at start) */
    cre2_set *set = cre2_set_new(opt, CRE2_ANCHOR_START);
    if (!set) {
        cre2_opt_delete(opt);
        return 2;
    }

    /* Create a regexp using the same options. Pattern "test" length 4. */
    const char *pattern = "test";
    cre2_regexp_t *re = cre2_new(pattern, 4, opt);
    if (!re) {
        /* Clean up and fail if regexp creation failed */
        cre2_set_delete(set);
        cre2_opt_delete(opt);
        return 3;
    }

    /* Use the regexp in trivial way by immediately deleting it (ensures cre2_delete is exercised) */
    cre2_delete(re);

    /* Clean up the set and options */
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    return 66;
}