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
//<ID> 123
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // Create options and enable logging of errors.
    cre2_options_t *opt = cre2_opt_new();
    if (!opt) return 1;
    cre2_opt_set_log_errors(opt, 1);

    // Query the dot-nl setting (uses required API cre2_opt_dot_nl).
    int dotnl = cre2_opt_dot_nl(opt);
    (void)dotnl;

    // Create a set with the options (anchor passed as 0).
    cre2_set *set = cre2_set_new(opt, (cre2_anchor_t)0);
    if (!set) {
        cre2_opt_delete(opt);
        return 2;
    }

    // Add a simple pattern and compile the set (uses required API cre2_set_compile).
    cre2_set_add_simple(set, "a(b)c");
    int compile_res = cre2_set_compile(set);
    (void)compile_res;

    // Prepare input and output match arrays for partial matching.
    const char txt[] = "abc";
    cre2_string_t input = { txt, 3 };
    cre2_string_t matches[3];
    for (int i = 0; i < 3; ++i) {
        matches[i].data = nullptr;
        matches[i].length = 0;
    }

    // Perform a partial match using a pattern string (uses required API cre2_partial_match).
    int pm = cre2_partial_match("a(b)c", &input, matches, 3);
    (void)pm;

    // Clean up the set (uses required API cre2_set_delete) and options.
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    return 66;
}