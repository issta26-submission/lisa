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
//<ID> 63
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // Create options and set the one_line flag
    cre2_options_t *opt = cre2_opt_new();
    if (!opt) return 1;
    cre2_opt_set_one_line(opt, 1);

    // Pattern with a named capture group to exercise named-groups iterator
    const char *pattern = "(?P<name>a)b";
    cre2_regexp_t *re = cre2_new(pattern, (int)strlen(pattern), opt);
    if (!re) {
        cre2_opt_delete(opt);
        return 1;
    }

    // Create a named-groups iterator and iterate (if any)
    cre2_named_groups_iter_t *iter = cre2_named_groups_iter_new(re);
    if (iter) {
        const char *gname = nullptr;
        int gindex = 0;
        while (cre2_named_groups_iter_next(iter, &gname, &gindex)) {
            // In a test/fuzz context we simply touch the values to ensure they're accessible.
            if (gname) {
                (void)gname;
            }
            (void)gindex;
        }
        cre2_named_groups_iter_delete(iter);
    }

    // Prepare text and rewrite strings for replace_re
    char text_buf[64] = "ab";
    cre2_string_t text_and_target = { text_buf, (int)strlen(text_buf) };
    const char *rewrite_s = "X";
    cre2_string_t rewrite = { rewrite_s, (int)strlen(rewrite_s) };

    // Perform a replacement using the compiled regexp
    // (result ignored; this exercises cre2_replace_re)
    (void)cre2_replace_re(re, &text_and_target, &rewrite);

    // Clean up the compiled regexp and options
    cre2_delete(re);
    cre2_opt_delete(opt);

    return 66;
}