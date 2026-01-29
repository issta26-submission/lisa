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
//<ID> 266
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    cre2_options_t *opt = cre2_opt_new();
    if (opt) {
        /* enable dot to match newline and enable logging of errors */
        cre2_opt_set_dot_nl(opt, 1);
        cre2_opt_set_log_errors(opt, 1);
    }

    /* create a set using the options (use anchor value 0) */
    cre2_set *set = cre2_set_new(opt, (cre2_anchor_t)0);

    const char *pattern = "hello.*world";
    if (set) {
        /* add a simple pattern and compile the set */
        cre2_set_add_simple(set, pattern);
        cre2_set_compile(set);
    }

    /* text contains a newline that should be matched by '.' when dot_nl is set */
    const char *text = "hello\nworld";
    size_t text_len = (size_t)strlen(text);

    /* buffer to receive indexes of matching patterns from the set */
    int matches_buf[8] = {0};
    size_t match_count = 0;
    if (set) {
        match_count = cre2_set_match(set, text, text_len, matches_buf, sizeof(matches_buf)/sizeof(matches_buf[0]));
    }

    /* use cre2_easy_match to check the same pattern against the text */
    cre2_string_t match_res[2];
    match_res[0].data = NULL; match_res[0].length = 0;
    match_res[1].data = NULL; match_res[1].length = 0;

    int easy_res = cre2_easy_match(pattern, (int)strlen(pattern), text, (int)text_len, match_res, 2);

    /* cleanup */
    if (set) cre2_set_delete(set);
    if (opt) cre2_opt_delete(opt);

    /* silence unused variable warnings in builds that check them */
    (void)match_count;
    (void)matches_buf;
    (void)match_res;
    (void)easy_res;

    return 66;
}