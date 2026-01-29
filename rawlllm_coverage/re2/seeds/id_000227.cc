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
//<ID> 227
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // Create options and configure them
    cre2_options_t *opt = cre2_opt_new();
    if (!opt) {
        return 66;
    }
    // Use required option-setting APIs
    cre2_opt_set_never_capture(opt, 0);    // allow capturing groups (0 = false)
    cre2_opt_set_longest_match(opt, 1);    // prefer longest match

    // Pattern contains a named capturing group "word"
    const char *pattern = "(?P<word>hello) world";
    int pattern_len = (int)(sizeof("(?P<word>hello) world") - 1);

    // Compile the regular expression using the options
    cre2_regexp_t *re = cre2_new(pattern, pattern_len, opt);
    if (!re) {
        cre2_opt_delete(opt);
        return 66;
    }

    // Create an iterator for named groups and iterate (if available)
    cre2_named_groups_iter_t *iter = cre2_named_groups_iter_new(re);
    if (iter) {
        const char *gname = NULL;
        int gindex = -1;
        // Iterate through any named groups (no-op body, just exercising the API)
        while (cre2_named_groups_iter_next(iter, &gname, &gindex)) {
            if (!gname) break;
            // Normally one might inspect gname/gindex; here we simply loop.
        }
        cre2_named_groups_iter_delete(iter);
    }

    // Prepare text and rewrite strings for cre2_replace
    cre2_string_t text_and_target;
    text_and_target.data = "hello world";
    text_and_target.length = (int)(sizeof("hello world") - 1);

    cre2_string_t rewrite;
    rewrite.data = "hi";
    rewrite.length = (int)(sizeof("hi") - 1);

    // Perform a replace using the pattern (non-compiled API)
    // This exercises cre2_replace as requested.
    (void)cre2_replace(pattern, &text_and_target, &rewrite);

    // Clean up compiled regexp and options
    cre2_delete(re);
    cre2_opt_delete(opt);

    return 66;
}