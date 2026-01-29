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
//<ID> 200
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // Create options and enable case sensitivity
    cre2_options_t *opt = cre2_opt_new();
    if (!opt) return 1;
    cre2_opt_set_case_sensitive(opt, 1);
    // Use the getter as required
    int cs = cre2_opt_case_sensitive(opt);
    (void)cs; // silence unused variable warning if any

    // Compile a regexp using the options
    const char *pattern = "(he(llo))";
    int pattern_len = (int)strlen(pattern);
    cre2_regexp_t *rex = cre2_new(pattern, pattern_len, opt);
    if (!rex) {
        cre2_opt_delete(opt);
        return 2;
    }

    // Prepare text and match containers
    const char *text_cstr = "hello world";
    cre2_string_t text = { text_cstr, (int)strlen(text_cstr) };
    cre2_string_t match = { NULL, 0 };

    // Use the non-compiled find-and-consume API with the pattern string
    // Request 1 match (nmatch = 1)
    (void)cre2_find_and_consume(pattern, &text, &match, 1);

    // Prepare a replacement string and call replace using the compiled regexp
    cre2_string_t rewrite = { "hi", 2 };
    (void)cre2_replace_re(rex, &text, &rewrite);

    // Create a set, add a simple pattern, and compile it (use cre2_set_compile as required)
    cre2_set *set = cre2_set_new(opt, (cre2_anchor_t)0);
    if (set) {
        (void)cre2_set_add_simple(set, "hello");
        (void)cre2_set_compile(set);
        cre2_set_delete(set);
    }

    // Cleanup
    cre2_delete(rex);
    cre2_opt_delete(opt);

    return 66;
}