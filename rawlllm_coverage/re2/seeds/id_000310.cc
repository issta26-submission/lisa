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
//<ID> 310
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // Create options and enable Perl character classes
    cre2_options_t *opt = cre2_opt_new();
    if (!opt) return 1;
    cre2_opt_set_perl_classes(opt, 1);

    // Query max memory (demonstrate use of cre2_opt_max_mem)
    int64_t max_mem = cre2_opt_max_mem(opt);
    (void)max_mem; // silence unused-variable warnings in test contexts

    // Create a pattern set and add a simple pattern
    cre2_set *set = cre2_set_new(opt, (cre2_anchor_t)0);
    if (!set) {
        cre2_opt_delete(opt);
        return 2;
    }
    int add_ret = cre2_set_add_simple(set, "foo");
    (void)add_ret; // ignore result for this test

    // Optionally compile the set (safe to call from API list)
    cre2_set_compile(set);

    // Create a compiled regular expression
    const char pat[] = "bar";
    cre2_regexp_t *rex = cre2_new(pat, (int)(sizeof(pat) - 1), opt);
    if (!rex) {
        cre2_set_delete(set);
        cre2_opt_delete(opt);
        return 3;
    }

    // Prepare text and rewrite strings
    char text_buf[] = "foo bar foo";
    cre2_string_t text_and_target;
    text_and_target.data = text_buf;
    text_and_target.length = (int)(sizeof(text_buf) - 1);

    char rewrite_buf[] = "X";
    cre2_string_t rewrite;
    rewrite.data = rewrite_buf;
    rewrite.length = (int)(sizeof(rewrite_buf) - 1);

    // Use pattern-based global replace
    int gr1 = cre2_global_replace("foo", &text_and_target, &rewrite);
    (void)gr1;

    // Use compiled-regex global replace
    int gr2 = cre2_global_replace_re(rex, &text_and_target, &rewrite);
    (void)gr2;

    // Clean up
    cre2_delete(rex);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    return 66;
}