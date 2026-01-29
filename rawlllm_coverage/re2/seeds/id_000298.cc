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
//<ID> 298
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    cre2_options_t *opt = cre2_opt_new();
    if (!opt) return 1;

    /* Query the library interface version (ensure API is callable). */
    int ver = cre2_version_interface_current();
    (void)ver; /* silence unused-var if not used further */

    /* Check case-sensitivity flag on the options object. */
    int case_sensitive = cre2_opt_case_sensitive(opt);
    (void)case_sensitive;

    /* Create a regexp using the options. */
    const char *pattern = "^(?P<name>abc)def$";
    int pat_len = (int)strlen(pattern);
    cre2_regexp_t *re = cre2_new(pattern, pat_len, opt);
    if (!re) {
        cre2_opt_delete(opt);
        return 2;
    }

    /* Create and immediately delete a named-groups iterator if available. */
    cre2_named_groups_iter_t *iter = cre2_named_groups_iter_new(re);
    if (iter) {
        cre2_named_groups_iter_delete(iter);
        iter = NULL;
    }

    /* Create a set and delete it to exercise set lifecycle. */
    cre2_set *set = cre2_set_new(opt, (cre2_anchor_t)0);
    if (set) {
        cre2_set_delete(set);
        set = NULL;
    }

    /* Clean up regexp and options. */
    cre2_delete(re);
    re = NULL;
    cre2_opt_delete(opt);
    opt = NULL;

    return 66;
}