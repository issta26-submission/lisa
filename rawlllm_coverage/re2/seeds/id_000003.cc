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
//<ID> 3
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // Create options and set word boundary flag
    cre2_options_t *opt = cre2_opt_new();
    if (!opt) {
        return 66;
    }
    cre2_opt_set_word_boundary(opt, 1);

    // Build a regexp using the options
    const char *pattern = "ab";
    cre2_regexp_t *rex = cre2_new(pattern, 2, opt);

    // Create a set with the same options and try to add a simple pattern
    cre2_set *set = cre2_set_new(opt, (cre2_anchor_t)0);
    char errbuf[128] = {0};
    if (set) {
        // add a simple pattern to the set, capture any error text in errbuf
        (void)cre2_set_add(set, "x", 1, errbuf, sizeof(errbuf));
    }

    // Prepare text and replacement for global replace using the compiled regexp
    cre2_string_t text_and_target;
    text_and_target.data = "abxab";
    text_and_target.length = 5;

    cre2_string_t rewrite;
    rewrite.data = "Y";
    rewrite.length = 1;

    if (rex) {
        (void)cre2_global_replace_re(rex, &text_and_target, &rewrite);
    }

    // Clean up
    if (rex) cre2_delete(rex);
    if (set) cre2_set_delete(set);
    cre2_opt_delete(opt);

    return 66;
}