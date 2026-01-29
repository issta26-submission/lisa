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
//<ID> 324
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

    // Query a couple of option flags (use the required getter APIs)
    int posix_flag = cre2_opt_posix_syntax(opt);
    int literal_flag = cre2_opt_literal(opt);
    (void)posix_flag;
    (void)literal_flag;

    // Compile a simple regexp using the options
    cre2_regexp_t *re = cre2_new("a", 1, opt);

    // Prepare strings for replace operation
    cre2_string_t text_and_target;
    text_and_target.data = "a";
    text_and_target.length = 1;

    cre2_string_t rewrite;
    rewrite.data = "b";
    rewrite.length = 1;

    // Perform a replace using the compiled regexp (if compilation succeeded)
    if (re) {
        (void)cre2_replace_re(re, &text_and_target, &rewrite);
    }

    // Create a set with the same options and add a simple pattern
    cre2_set *set = cre2_set_new(opt, (cre2_anchor_t)0);
    if (set) {
        (void)cre2_set_add_simple(set, "a");
        cre2_set_delete(set);
    }

    // Clean up
    if (re) cre2_delete(re);
    cre2_opt_delete(opt);

    return 66;
}