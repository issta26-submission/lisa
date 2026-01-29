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
//<ID> 292
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    cre2_options_t *opt = cre2_opt_new();
    if (!opt) return 1;

    /* Enable dot matching newline and verify the option */
    cre2_opt_set_dot_nl(opt, 1);
    int dot_nl_enabled = cre2_opt_dot_nl(opt);
    (void)dot_nl_enabled; /* keep variable to show we used the getter */

    /* Create a set with these options (use 0 as anchor value) */
    cre2_set *set = cre2_set_new(opt, (cre2_anchor_t)0);
    if (!set) {
        cre2_opt_delete(opt);
        return 2;
    }

    /* Add a simple pattern to the set and compile it */
    cre2_set_add_simple(set, "foo");
    int compile_rc = cre2_set_compile(set);
    (void)compile_rc; /* ignore specific return semantics for this test */

    /* Create a regexp directly and attempt a full match using the re-version API */
    const char *pattern = "foo";
    cre2_regexp_t *rex = cre2_new(pattern, (int)strlen(pattern), opt);
    if (!rex) {
        cre2_set_delete(set);
        cre2_opt_delete(opt);
        return 3;
    }

    /* Prepare input and match storage */
    const char *text = "foo";
    cre2_string_t input;
    input.data = text;
    input.length = (int)strlen(text);

    cre2_string_t match;
    match.data = NULL;
    match.length = 0;

    /* Perform a full match using the compiled regexp object */
    int matched = cre2_full_match_re(rex, &input, &match, 1);
    (void)matched; /* result used only for test flow */

    /* Cleanup */
    cre2_delete(rex);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    return 66;
}