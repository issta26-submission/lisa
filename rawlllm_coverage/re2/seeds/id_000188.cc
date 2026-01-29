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
//<ID> 188
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // Create options and set never_nl
    cre2_options_t *opt = cre2_opt_new();
    if (!opt) return 1;
    cre2_opt_set_never_nl(opt, 1);

    // Create a regexp with a named capturing group
    const char *pattern = "(?P<name>ab)c";
    int pattern_len = (int)strlen(pattern);
    cre2_regexp_t *re = cre2_new(pattern, pattern_len, opt);
    if (!re) {
        cre2_opt_delete(opt);
        return 2;
    }

    // Create a named-groups iterator (uses the created regexp)
    cre2_named_groups_iter_t *iter = cre2_named_groups_iter_new(re);
    // we don't iterate in this test, but ensure we delete the iterator if created later

    // Create a set, add a simple pattern, and compile it
    cre2_set *set = cre2_set_new(opt, (cre2_anchor_t)0);
    if (set) {
        // add a simple pattern to the set
        cre2_set_add_simple(set, "a(b)c");
        // compile the set (check return but continue regardless)
        int compile_ret = cre2_set_compile(set);
        (void)compile_ret;
    }

    // Use cre2_easy_match to match a simple pattern against text
    const char *text = "abc";
    int text_len = (int)strlen(text);
    cre2_string_t easy_matches[2] = {{NULL, 0}, {NULL, 0}};
    int easy_ret = cre2_easy_match("a(b)c", (int)strlen("a(b)c"), text, text_len, easy_matches, 2);
    (void)easy_ret; // result is not critical for this sequence test

    // Prepare input and output structures for cre2_consume_re
    cre2_string_t input = { text, text_len };
    cre2_string_t consume_matches[2] = {{NULL, 0}, {NULL, 0}};
    // Attempt to consume from the beginning using the compiled regexp
    int consume_ret = cre2_consume_re(re, &input, consume_matches, 2);
    (void)consume_ret;

    // Cleanup
    if (iter) cre2_named_groups_iter_delete(iter);
    if (re) cre2_delete(re);
    if (set) cre2_set_delete(set);
    cre2_opt_delete(opt);

    return 66;
}