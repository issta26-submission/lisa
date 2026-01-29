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
//<ID> 296
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // Create and configure options
    cre2_options_t *opt = cre2_opt_new();
    if (opt) {
        cre2_opt_set_perl_classes(opt, 1);
        cre2_opt_set_one_line(opt, 1);
    }

    // Prepare a pattern with a named capturing group and compile it.
    const char *pattern = "(?P<name>foo)";
    int pat_len = 0;
    while (pattern[pat_len]) ++pat_len;

    cre2_regexp_t *re = cre2_new(pattern, pat_len, opt);

    // If compilation succeeded, create a named-groups iterator and then delete it.
    cre2_named_groups_iter_t *iter = NULL;
    if (re) {
        iter = cre2_named_groups_iter_new(re);
    }
    if (iter) {
        // Normally one would iterate with cre2_named_groups_iter_next; here we just ensure
        // creation and deletion work together.
        cre2_named_groups_iter_delete(iter);
        iter = NULL;
    }

    // Use cre2_easy_match to run a quick match without compiling.
    const char *simple_pat = "foo";
    const char *text = "xxxfoozzz";
    int simple_pat_len = 0;
    while (simple_pat[simple_pat_len]) ++simple_pat_len;
    int text_len = 0;
    while (text[text_len]) ++text_len;

    cre2_string_t match;
    match.data = NULL;
    match.length = 0;
    // Request up to 1 match slot
    (void)cre2_easy_match(simple_pat, simple_pat_len, text, text_len, &match, 1);

    // Cleanup compiled regexp and options
    if (re) cre2_delete(re);
    if (opt) cre2_opt_delete(opt);

    return 66;
}