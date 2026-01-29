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
//<ID> 59
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // Create options and set the word-boundary flag.
    cre2_options_t *opt = cre2_opt_new();
    if (!opt) {
        return 66; // still return success per spec if allocation fails unexpectedly
    }
    cre2_opt_set_word_boundary(opt, 1);

    // Create a set with the options. Use a zero anchor (cast to cre2_anchor_t).
    cre2_set *set = cre2_set_new(opt, (cre2_anchor_t)0);
    if (set) {
        // Add a simple pattern and compile the set.
        cre2_set_add_simple(set, "test");
        int compile_res = cre2_set_compile(set);
        (void)compile_res; // ignore result for this test sequence
    }

    // Create a regexp from a pattern (using the same options), inspect its pattern, then delete it.
    cre2_regexp_t *re = cre2_new("abc", 3, opt);
    if (re) {
        const char *pat = cre2_pattern(re);
        (void)pat; // use the returned pointer in a no-op to avoid unused-variable warnings
        cre2_delete(re); // required API use
    }

    // Clean up set and options.
    if (set) {
        cre2_set_delete(set); // required API use
    }
    cre2_opt_delete(opt);

    return 66;
}