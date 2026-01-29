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
//<ID> 335
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    /* Create options and set case sensitivity */
    cre2_options_t *opt = cre2_opt_new();
    if (!opt) return 1;
    cre2_opt_set_case_sensitive(opt, 1);

    /* Compile a pattern that contains a named capturing group "name" */
    const char *pattern = "(?P<name>abc)";
    int pattern_len = 13; /* length of "(?P<name>abc)" */
    cre2_regexp_t *re = cre2_new(pattern, pattern_len, opt);
    if (!re) {
        cre2_opt_delete(opt);
        return 2;
    }

    /* Query the index of the named capturing group */
    int name_index = cre2_find_named_capturing_groups(re, "name");
    (void)name_index; /* we don't strictly require a specific value here */

    /* Use the simple find-and-consume API with a literal pattern */
    cre2_string_t text;
    const char *input = "xxxxabcyyyy";
    text.data = input;
    text.length = 11; /* length of "xxxxabcyyyy" */

    cre2_string_t match;
    match.data = NULL;
    match.length = 0;

    int found = cre2_find_and_consume("abc", &text, &match, 1);
    /* found may indicate whether a match was consumed; continue regardless */

    /* Use replace_re with the compiled regexp to attempt a replacement */
    cre2_string_t text_and_target;
    const char *text2 = "abc def abc";
    text_and_target.data = text2;
    text_and_target.length = 11; /* length of "abc def abc" */

    cre2_string_t rewrite;
    const char *rew = "X";
    rewrite.data = rew;
    rewrite.length = 1;

    int replace_ret = cre2_replace_re(re, &text_and_target, &rewrite);

    /* Clean up */
    cre2_delete(re);
    cre2_opt_delete(opt);

    /* Consider operations successful if replace_ret is non-negative (API-specific) */
    if (replace_ret < 0) return 5;

    (void)found; /* suppress unused-variable warning if needed */
    return 66;
}