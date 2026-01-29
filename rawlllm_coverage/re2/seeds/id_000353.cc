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
//<ID> 353
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    /* Create and configure options */
    cre2_options_t *opt = cre2_opt_new();
    if (!opt) return 1;

    /* Set required options */
    cre2_opt_set_max_mem(opt, 1024LL * 1024LL);  /* 1 MB */
    cre2_opt_set_literal(opt, 1);                /* treat pattern as literal */
    cre2_opt_set_word_boundary(opt, 1);          /* enable word-boundary behavior */

    /* Create a regexp using the options. Use a simple literal pattern. */
    const char *pattern = "hello";
    int pattern_len = 5; /* length of "hello" */
    cre2_regexp_t *re = cre2_new(pattern, pattern_len, opt);
    if (!re) {
        cre2_opt_delete(opt);
        return 2;
    }

    /* Prepare input text and match storage for full-match call */
    cre2_string_t text;
    text.data = "hello";
    text.length = 5;

    cre2_string_t matches[1];
    matches[0].data = NULL;
    matches[0].length = 0;

    /* Attempt a full match with the compiled regexp */
    int matched = cre2_full_match_re(re, &text, matches, 1);
    (void)matched; /* We don't strictly require checking contents for this test */

    /* Clean up */
    cre2_delete(re);
    cre2_opt_delete(opt);

    return 66;
}