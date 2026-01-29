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
//<ID> 145
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // Create options
    cre2_options_t *opt = cre2_opt_new();
    if (!opt) return 1;

    // Configure options (required API usage)
    cre2_opt_set_one_line(opt, 1);
    cre2_opt_set_never_nl(opt, 0);

    // Create a set using the options. Use a zero value for anchor (cast to cre2_anchor_t).
    cre2_set *set = cre2_set_new(opt, (cre2_anchor_t)0);
    if (!set) {
        cre2_opt_delete(opt);
        return 2;
    }

    // Add a simple pattern, compile and attempt a match.
    cre2_set_add_simple(set, "a");
    cre2_set_compile(set);

    int matches[4] = { -1, -1, -1, -1 };
    size_t found = cre2_set_match(set, "a", 1, matches, 4);
    (void)found; // silence unused-variable warnings in testing contexts

    // Clean up (required API usage)
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    return 66;
}