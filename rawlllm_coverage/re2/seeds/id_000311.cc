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
//<ID> 311
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    /* Create options and set flags */
    cre2_options_t *opt = cre2_opt_new();
    if (opt) {
        cre2_opt_set_case_sensitive(opt, 1);     /* enable case sensitivity */
        cre2_opt_set_never_capture(opt, 0);     /* allow captures (not forcing never-capture) */
    }

    /* Create a regex set (so we can exercise cre2_set_delete later) */
    cre2_set *set = nullptr;
    if (opt) {
        /* cre2_anchor_t is an enum in the library; pass 0 for a valid default anchor */
        set = cre2_set_new(opt, (cre2_anchor_t)0);
    }

    /* Compile a pattern using the options */
    const char *pattern = "\\d+";
    cre2_regexp_t *rex = nullptr;
    if (opt) {
        rex = cre2_new(pattern, (int)strlen(pattern), opt);
    }

    /* Prepare input text and rewrite string for global replace */
    const char *text = "abc 123 def 456";
    cre2_string_t text_and_target;
    text_and_target.data = text;
    text_and_target.length = (int)strlen(text);

    const char *rewrite_s = "#";
    cre2_string_t rewrite;
    rewrite.data = rewrite_s;
    rewrite.length = (int)strlen(rewrite_s);

    /* Perform a global replace using the compiled regexp (if compilation succeeded) */
    if (rex) {
        /* cre2_global_replace_re returns an int status/number; we call and ignore the value here */
        (void)cre2_global_replace_re(rex, &text_and_target, &rewrite);
    }

    /* Clean up created objects */
    if (rex) cre2_delete(rex);
    if (set) cre2_set_delete(set);
    if (opt) cre2_opt_delete(opt);

    return 66;
}