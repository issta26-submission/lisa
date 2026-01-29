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
//<ID> 23
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

    // Enable word boundary handling, disable capturing, and limit memory
    cre2_opt_set_word_boundary(opt, 1);
    cre2_opt_set_never_capture(opt, 1);
    cre2_opt_set_max_mem(opt, (int64_t)(16 * 1024 * 1024)); // 16MB

    // Prepare a pattern and a subject for an easy match
    const char *pattern = "(foo)bar";
    int pattern_len = (int)strlen(pattern);
    const char *text = "xx foobar yy";
    int text_len = (int)strlen(text);

    // Prepare match array (nmatch = 10)
    cre2_string_t matches[10];
    for (int i = 0; i < 10; ++i) {
        matches[i].data = NULL;
        matches[i].length = 0;
    }

    // Perform an easy match
    int matched = cre2_easy_match(pattern, pattern_len, text, text_len, matches, 10);
    (void)matched; // keep for potential debugging; continue regardless

    // Prepare for a replace operation: replace "foo" with "baz" in text_and_target
    const char *replace_pat = "foo";
    cre2_string_t text_and_target;
    text_and_target.data = text;
    text_and_target.length = text_len;

    cre2_string_t rewrite;
    const char *repstr = "baz";
    rewrite.data = repstr;
    rewrite.length = (int)strlen(repstr);

    // Perform replace (may modify text_and_target)
    int replace_ret = cre2_replace(replace_pat, &text_and_target, &rewrite);
    (void)replace_ret;

    // Clean up options
    cre2_opt_delete(opt);

    return 66;
}