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
//<ID> 219
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    /* Create options */
    cre2_options_t *opt = cre2_opt_new();
    if (!opt) {
        return 1; /* failure creating options */
    }

    /* Use setter to change case sensitivity */
    cre2_opt_set_case_sensitive(opt, 1);

    /* Read the literal flag (getter) */
    int literal = cre2_opt_literal(opt);
    (void)literal; /* silence unused-variable warnings */

    /* Toggle case sensitivity again */
    cre2_opt_set_case_sensitive(opt, 0);

    /* Query version interface revision */
    int rev = cre2_version_interface_revision();
    (void)rev; /* we don't assert on the value here */

    /* Prepare a named-groups iterator pointer and delete it safely.
       Use NULL here; many C APIs accept NULL for delete/free functions. */
    cre2_named_groups_iter_t *iter = NULL;
    cre2_named_groups_iter_delete(iter);

    /* Clean up options */
    cre2_opt_delete(opt);

    return 66;
}