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
//<ID> 109
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    cre2_options_t *opt = cre2_opt_new();
    if (!opt) return 1;

    /* Use required option setters/getter */
    cre2_opt_set_log_errors(opt, 1);
    cre2_opt_set_word_boundary(opt, 1);
    /* call the getter (use its return value lightly) */
    int one_line_flag = cre2_opt_one_line(opt);
    (void)one_line_flag;

    /* Compile a regex using these options */
    const char *pattern = "(\\w+)-(\\d+)";
    int pat_len = (int)strlen(pattern);
    cre2_regexp_t *re = cre2_new(pattern, pat_len, opt);
    if (!re) {
        cre2_opt_delete(opt);
        return 2;
    }

    /* Prepare a text to match against */
    const char *text = "abc-123 def-456";
    int textlen = (int)strlen(text);

    /* Prepare match array (limit to a reasonable fixed max) */
    int groups = cre2_num_capturing_groups(re);
    int nmatch = (groups >= 0) ? (groups + 1) : 1;
    const int MAXMATCH = 16;
    if (nmatch > MAXMATCH) nmatch = MAXMATCH;
    cre2_string_t matches[MAXMATCH];
    for (int i = 0; i < nmatch; ++i) {
        matches[i].data = NULL;
        matches[i].length = 0;
    }

    /* Use cre2_match */
    int matched = cre2_match(re, text, textlen, 0, textlen, CRE2_UNANCHORED, matches, nmatch);
    (void)matched; /* we just exercise the API; ignore the exact result here */

    /* Use cre2_find_and_consume with an inline pattern (uncompiled form) */
    cre2_string_t input;
    input.data = text;
    input.length = textlen;

    cre2_string_t consumed_matches[MAXMATCH];
    for (int i = 0; i < MAXMATCH; ++i) {
        consumed_matches[i].data = NULL;
        consumed_matches[i].length = 0;
    }

    int found = cre2_find_and_consume("(\\w+)-", &input, consumed_matches, MAXMATCH);
    (void)found;

    /* Cleanup */
    cre2_delete(re);
    cre2_opt_delete(opt);

    return 66;
}