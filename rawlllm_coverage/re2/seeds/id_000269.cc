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
//<ID> 269
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // Create and configure options
    cre2_options_t *opt = cre2_opt_new();
    if (!opt) return 1;

    // Build a simple pattern with a named capturing group
    const char *pattern = "(?P<g>a+)";
    cre2_regexp_t *re = cre2_new(pattern, (int)strlen(pattern), opt);
    // options no longer needed after creating the regexp
    cre2_opt_delete(opt);
    if (!re) return 2;

    // Create a named-groups iterator and iterate (if created)
    cre2_named_groups_iter_t *iter = cre2_named_groups_iter_new(re);
    if (iter) {
        const char *name = nullptr;
        int index = 0;
        while (cre2_named_groups_iter_next(iter, &name, &index)) {
            // use the values in a no-op way to demonstrate access
            (void)name;
            (void)index;
        }
        cre2_named_groups_iter_delete(iter);
    }

    // Demonstrate cre2_quote_meta: prepare original and output buffers
    const char *orig = "a+b(c)";
    cre2_string_t orig_str;
    orig_str.data = orig;
    orig_str.length = (int)strlen(orig);

    char quoted_buf[128] = {0};
    cre2_string_t quoted_str;
    quoted_str.data = quoted_buf;
    quoted_str.length = (int)sizeof(quoted_buf);

    // Quote the meta-characters from orig into quoted_buf
    int qres = cre2_quote_meta(&quoted_str, &orig_str);
    (void)qres;

    // Demonstrate cre2_consume with a simple pattern and input
    const char *consume_pattern = "a+";
    char input_buf[] = "aaab";
    cre2_string_t input;
    input.data = input_buf;
    input.length = (int)strlen(input_buf);

    char match_buf[64] = {0};
    cre2_string_t match;
    match.data = match_buf;
    match.length = (int)sizeof(match_buf);

    int cres = cre2_consume(consume_pattern, &input, &match, 0);
    (void)cres;

    // Clean up the compiled regular expression
    cre2_delete(re);

    return 66;
}