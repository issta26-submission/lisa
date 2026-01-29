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
//<ID> 239
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // Create options and configure them.
    cre2_options_t *opt = cre2_opt_new();
    if (!opt) {
        // If options couldn't be created, nothing else to do.
        return 66;
    }

    // Use setters to enable the features, then use the required getter APIs.
    cre2_opt_set_perl_classes(opt, 1);
    cre2_opt_set_never_nl(opt, 1);

    int perl_classes_enabled = cre2_opt_perl_classes(opt);
    int never_nl_enabled = cre2_opt_never_nl(opt);
    (void)perl_classes_enabled;
    (void)never_nl_enabled;

    // Create a set and add a simple pattern to it.
    cre2_set *set = cre2_set_new(opt, (cre2_anchor_t)0);
    if (set) {
        const char add_pattern[] = "foo[0-9]+";
        cre2_set_add_simple(set, add_pattern);
    }

    // Compile a regexp using the same options.
    const char pattern[] = "([a-z]+)([0-9]+)";
    cre2_regexp_t *rex = cre2_new(pattern, (int)(sizeof(pattern) - 1), opt);

    // Prepare text and rewrite strings for global replacement.
    cre2_string_t text_and_target;
    text_and_target.data = "foo123bar";
    text_and_target.length = (int)(sizeof("foo123bar") - 1);

    cre2_string_t rewrite;
    rewrite.data = "$1-REPL-$2";
    rewrite.length = (int)(sizeof("$1-REPL-$2") - 1);

    // Perform a global replace using the compiled regexp (if created).
    if (rex) {
        cre2_global_replace_re(rex, &text_and_target, &rewrite);
        cre2_delete(rex);
    }

    // Cleanup set and options.
    if (set) cre2_set_delete(set);
    cre2_opt_delete(opt);

    return 66;
}