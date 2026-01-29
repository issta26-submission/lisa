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
//<ID> 140
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // Create options
    cre2_options_t *opt = cre2_opt_new();
    if (!opt) {
        return 1;
    }

    // Query some option values (use required APIs)
    int literal_flag = cre2_opt_literal(opt);
    (void)literal_flag; // silence unused variable warning

    int64_t maxmem = cre2_opt_max_mem(opt);
    (void)maxmem;

    // Create a set using the options. Use anchor value 0 (no anchoring).
    cre2_set *set = cre2_set_new(opt, (cre2_anchor_t)0);
    if (set) {
        // Add a simple pattern to the set (required API)
        int add_res = cre2_set_add_simple(set, "hello.*");
        (void)add_res;

        // Clean up the set (required API)
        cre2_set_delete(set);
        set = nullptr;
    }

    // Create a simple regexp with a named group so we can create an iterator
    cre2_regexp_t *re = cre2_new("(?P<name>abc)", 13, opt); // pattern length is 13
    if (re) {
        // Create and immediately delete a named-groups iterator (required API)
        cre2_named_groups_iter_t *iter = cre2_named_groups_iter_new(re);
        if (iter) {
            cre2_named_groups_iter_delete(iter);
            iter = nullptr;
        }

        // Delete the regexp
        cre2_delete(re);
        re = nullptr;
    }

    // Delete options
    cre2_opt_delete(opt);
    opt = nullptr;

    return 66;
}