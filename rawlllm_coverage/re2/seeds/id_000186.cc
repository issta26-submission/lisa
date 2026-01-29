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
//<ID> 186
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

    /* set case sensitivity on the options */
    cre2_opt_set_case_sensitive(opt, 1);

    /* create a regexp using the options */
    const char *pat = "abc";
    cre2_regexp_t *re = cre2_new(pat, 3, opt);

    /* create a set using the same options (anchor value 0 used as default) */
    cre2_set *set = cre2_set_new(opt, (cre2_anchor_t)0);
    if (!set) {
        if (re) cre2_delete(re);
        cre2_opt_delete(opt);
        return 1;
    }

    /* add simple patterns to the set */
    (void)cre2_set_add_simple(set, "abc");
    (void)cre2_set_add_simple(set, "def");

    /* cleanup */
    cre2_set_delete(set);
    if (re) cre2_delete(re);
    cre2_opt_delete(opt);

    return 66;
}