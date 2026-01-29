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
//<ID> 18
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // Create options and enable DOT to match newlines.
    cre2_options_t *opt = cre2_opt_new();
    if (!opt) return 1;
    cre2_opt_set_dot_nl(opt, 1);

    // Create a regexp using the options, then delete it.
    const char *pat = "a.b";
    cre2_regexp_t *re = cre2_new(pat, 3, opt);
    if (re != nullptr) {
        cre2_delete(re);
    }

    // Create a set with the same options. Use a zero anchor (cast to cre2_anchor_t).
    cre2_set *set = cre2_set_new(opt, (cre2_anchor_t)0);
    if (set != nullptr) {
        // Add a simple pattern to the set.
        int add_res = cre2_set_add_simple(set, "foo|bar");
        (void)add_res;
        cre2_set_delete(set);
    }

    // Prepare strings for a replace operation.
    cre2_string_t text_and_target;
    text_and_target.data = "hello world";
    text_and_target.length = 11; // length of "hello world"

    cre2_string_t rewrite;
    rewrite.data = "hi";
    rewrite.length = 2;

    // Perform a one-shot replace: replace "world" with "hi" in text_and_target.
    int rep_res = cre2_replace("world", &text_and_target, &rewrite);
    (void)rep_res;

    // Clean up options.
    cre2_opt_delete(opt);

    return 66;
}