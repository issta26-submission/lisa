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
//<ID> 304
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // Create options and adjust a couple of flags
    cre2_options_t *opt = cre2_opt_new();
    if (!opt) return 1;

    // Set literal mode on and query never_nl option (demonstration of getter)
    cre2_opt_set_literal(opt, 1);
    int never_nl_val = cre2_opt_never_nl(opt);
    (void)never_nl_val; // silence unused variable warning

    // Create a set with the options and add a simple pattern
    cre2_set *set = cre2_set_new(opt, (cre2_anchor_t)0);
    if (set) {
        cre2_set_add_simple(set, "foo");
        // Compile the set
        int compile_res = cre2_set_compile(set);
        (void)compile_res; // ignore for this test
    }

    // Prepare text and match structures for a find-and-consume call using pattern string
    cre2_string_t text;
    text.data = "xxfooYY";
    text.length = (int)(sizeof("xxfooYY") - 1); // 7

    cre2_string_t match;
    match.data = nullptr;
    match.length = 0;

    // Use the non-re2 find-and-consume API with pattern "foo"
    int found = cre2_find_and_consume("foo", &text, &match, 1);
    (void)found; // for fuzz driver we just exercise the API

    // Create a compiled regexp and then delete it to exercise cre2_delete
    cre2_regexp_t *re = cre2_new("bar", 3, opt);
    if (re) {
        cre2_delete(re);
    }

    // Clean up set and options
    if (set) cre2_set_delete(set);
    cre2_opt_delete(opt);

    return 66;
}