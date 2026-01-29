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
//<ID> 69
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    cre2_options_t *opt = cre2_opt_new();
    if (!opt) return 1;

    /* Set literal mode on the options */
    cre2_opt_set_literal(opt, 1);

    /* Create a set with the options (use anchor value 0) */
    cre2_set *set = cre2_set_new(opt, (cre2_anchor_t)0);
    if (!set) {
        cre2_opt_delete(opt);
        return 1;
    }

    /* Add a couple of simple patterns to the set and compile it */
    cre2_set_add_simple(set, "foo");
    cre2_set_add_simple(set, "bar");
    int compile_res = cre2_set_compile(set);

    /* Create a regexp object using the same options */
    cre2_regexp_t *re = cre2_new("foo", 3, opt);
    if (!re) {
        cre2_set_delete(set);
        cre2_opt_delete(opt);
        return 1;
    }

    /* Prepare strings for replace and global_replace operations */
    cre2_string_t text = { "foo bar foo", 11 };
    cre2_string_t rewrite = { "baz", 3 };
    int rep_res = cre2_replace_re(re, &text, &rewrite);

    cre2_string_t gtext = { "foo foo foo", 11 };
    cre2_string_t grewrite = { "qux", 3 };
    int glob_res = cre2_global_replace_re(re, &gtext, &grewrite);

    /* Cleanup */
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    /* Consider non-zero compile and non-negative replace results as success */
    if (compile_res != 0 && rep_res >= 0 && glob_res >= 0) return 66;
    return 1;
}