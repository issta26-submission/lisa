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
//<ID> 306
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // Create options and enable dot matches newline
    cre2_options_t *opt = cre2_opt_new();
    if (!opt) return 1;
    cre2_opt_set_dot_nl(opt, 1);

    // Use a pattern that contains a named group to exercise the named-groups iterator
    const char pattern[] = "(?P<name>foo)|(bar)";
    int patlen = (int)(sizeof(pattern) - 1);

    // Compile the regular expression with the options
    cre2_regexp_t *re = cre2_new(pattern, patlen, opt);

    // Track success before cleanup
    bool ok = (re != nullptr);

    // If compilation succeeded, create and delete a named-groups iterator
    cre2_named_groups_iter_t *iter = nullptr;
    if (re) {
        iter = cre2_named_groups_iter_new(re);
    }
    if (iter) {
        cre2_named_groups_iter_delete(iter);
    }

    // Delete the regexp and options
    if (re) {
        cre2_delete(re);
    }
    cre2_opt_delete(opt);

    return ok ? 66 : 1;
}