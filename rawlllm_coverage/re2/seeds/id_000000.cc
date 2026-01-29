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
//<ID> 0
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    cre2_options_t *opt = cre2_opt_new();
    if (!opt) {
        return 66;
    }

    // Enable Perl character classes on the options
    cre2_opt_set_perl_classes(opt, 1);

    // Compile a regex with two named capturing groups.
    const char *pattern = "(?P<first>\\w+)-(?P<second>\\d+)";
    cre2_regexp_t *re = cre2_new(pattern, (int)strlen(pattern), opt);
    if (!re) {
        cre2_opt_delete(opt);
        return 66;
    }

    // Iterate over named capturing groups (if any)
    cre2_named_groups_iter_t *iter = cre2_named_groups_iter_new(re);
    if (iter) {
        const char *name = nullptr;
        int index = 0;
        while (cre2_named_groups_iter_next(iter, &name, &index)) {
            // touch the values to avoid unused-variable warnings
            (void)name;
            (void)index;
        }
        cre2_named_groups_iter_delete(iter);
    }

    // Prepare text and rewrite strings for a global replace
    char text_buf[] = "abc-123 def-456";
    cre2_string_t text = { text_buf, (int)strlen(text_buf) };

    char rewrite_buf[] = "[$1:$2]";
    cre2_string_t rewrite = { rewrite_buf, (int)strlen(rewrite_buf) };

    // Perform a global replace using the compiled regexp
    (void)cre2_global_replace_re(re, &text, &rewrite);

    // Clean up the compiled regexp and options
    cre2_delete(re);
    cre2_opt_delete(opt);

    return 66;
}