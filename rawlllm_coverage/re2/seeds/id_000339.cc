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
//<ID> 339
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

    /* Set encoding, longest-match behavior, and enable logging of errors */
    cre2_opt_set_encoding(opt, CRE2_UTF8);
    cre2_opt_set_longest_match(opt, 1);
    cre2_opt_set_log_errors(opt, 1);

    /* Prepare pattern, input text (text_and_target), and rewrite strings */
    const char *pattern = "a";

    char text_buf[] = "abracadabra a a";
    cre2_string_t text_and_target;
    text_and_target.data = text_buf;
    text_and_target.length = (int)(sizeof(text_buf) - 1); /* exclude NUL */

    char rewrite_buf[] = "X";
    cre2_string_t rewrite;
    rewrite.data = rewrite_buf;
    rewrite.length = (int)(sizeof(rewrite_buf) - 1);

    /* Perform a global replace using the pattern */
    int rc = cre2_global_replace(pattern, &text_and_target, &rewrite);

    /* Clean up options */
    cre2_opt_delete(opt);

    if (rc < 0) {
        /* indicate failure (non-66) */
        return 2;
    }

    /* Success */
    return 66;
}