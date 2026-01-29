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
//<ID> 22
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    /* Create options for regex/set construction */
    cre2_options_t *opt = cre2_opt_new();

    /* Create a regexp with a named capturing group "(?P<name>foo)". */
    const char *pattern = "(?P<name>foo)";
    cre2_regexp_t *rex = cre2_new(pattern, (int)strlen(pattern), opt);

    /* Create a set, add simple patterns, and compile it. */
    cre2_set *set = cre2_set_new(opt, (cre2_anchor_t)0);
    if (set) {
        cre2_set_add_simple(set, "foo");
        cre2_set_add_simple(set, "baz");
        /* compile the set to prepare for matching */
        cre2_set_compile(set);
    }

    /* If regexp was created, iterate over named groups (if any). */
    if (rex) {
        cre2_named_groups_iter_t *iter = cre2_named_groups_iter_new(rex);
        const char *name = NULL;
        int index = 0;
        while (iter && cre2_named_groups_iter_next(iter, &name, &index)) {
            /* consume name/index to exercise the API; no-op here */
            (void)name;
            (void)index;
        }
        if (iter) cre2_named_groups_iter_delete(iter);
    }

    /* Prepare text and rewrite strings and perform a global replace using the compiled regexp. */
    char text_buffer[] = "foo foo baz";
    cre2_string_t text_and_target = { text_buffer, (int)strlen(text_buffer) };
    cre2_string_t rewrite = { "repl", (int)strlen("repl") };
    if (rex) {
        /* perform replacement in-place (API may potentially modify target) */
        (void)cre2_global_replace_re(rex, &text_and_target, &rewrite);
    }

    /* Cleanup resources */
    if (set) cre2_set_delete(set);
    if (rex) cre2_delete(rex);
    if (opt) cre2_opt_delete(opt);

    /* Indicate success as requested. */
    return 66;
}