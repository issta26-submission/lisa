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
//<ID> 167
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    cre2_options_t *opt = cre2_opt_new();
    if (!opt) {
        return 66;
    }

    /* Use the version interface revision to decide case sensitivity */
    int rev = cre2_version_interface_revision();
    if (rev >= 1) {
        cre2_opt_set_case_sensitive(opt, 1);
    } else {
        cre2_opt_set_case_sensitive(opt, 0);
    }

    /* Limit memory usage for the compiled regexp (set to ~4MB) */
    cre2_opt_set_max_mem(opt, 4LL * 1024 * 1024);

    /* Pattern with a named capturing group "foo" */
    const char *pattern = "(?P<foo>abc)";
    int pattern_len = 12; /* length of "(?P<foo>abc)" */

    cre2_regexp_t *re = cre2_new(pattern, pattern_len, opt);
    if (re) {
        /* Look up the named capturing group */
        int idx = cre2_find_named_capturing_groups(re, "foo");
        (void)idx; /* use the result (could be -1 if not found) */

        /* Clean up the regexp */
        cre2_delete(re);
    }

    /* Clean up options */
    cre2_opt_delete(opt);

    return 66;
}