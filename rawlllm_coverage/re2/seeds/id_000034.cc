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
//<ID> 34
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

    // Use required options API
    cre2_opt_set_literal(opt, 1);

    // Pattern as an array so we can get compile-time length without calling strlen
    const char pattern[] = "a(b)c";
    int pattern_len = (int)(sizeof(pattern) - 1);

    // Create a regexp object
    cre2_regexp_t *re = cre2_new(pattern, pattern_len, opt);
    if (!re) {
        cre2_opt_delete(opt);
        return 2;
    }

    // Create a named-groups iterator and immediately delete it (exercise iterator lifecycle)
    cre2_named_groups_iter_t *iter = cre2_named_groups_iter_new(re);
    if (iter) {
        cre2_named_groups_iter_delete(iter);
    }

    // Create a set and delete it to exercise set lifecycle
    cre2_set *set = cre2_set_new(opt, (cre2_anchor_t)0);
    if (set) {
        cre2_set_delete(set);
    }

    // Clean up regexp and options
    cre2_delete(re);
    cre2_opt_delete(opt);

    return 66;
}