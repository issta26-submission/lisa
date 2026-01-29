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
//<ID> 184
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

    /* Set POSIX syntax on and verify using the getter */
    cre2_opt_set_posix_syntax(opt, 1);
    int posix = cre2_opt_posix_syntax(opt);
    if (posix != 1) {
        cre2_opt_delete(opt);
        return 2;
    }

    /* Create a cre2 set with the options and an anchor, then delete it */
    cre2_set *set = cre2_set_new(opt, CRE2_ANCHOR_START);
    if (!set) {
        cre2_opt_delete(opt);
        return 3;
    }
    cre2_set_delete(set);

    /* Create a simple regexp and then delete it */
    cre2_regexp_t *re = cre2_new("a", 1, opt);
    if (re) {
        cre2_delete(re);
    }

    /* Clean up options */
    cre2_opt_delete(opt);

    return 66;
}