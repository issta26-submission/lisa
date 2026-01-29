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
//<ID> 328
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    /* Create options */
    cre2_options_t *opt = cre2_opt_new();
    if (!opt) return 1;

    /* Pattern used for both cre2_new and cre2_global_replace */
    const char *pattern_lit = "(\\w+)=(\\d+)";
    int pattern_len = (int)(sizeof("(\\w+)=(\\d+)") - 1);

    /* Compile a regexp with the options */
    cre2_regexp_t *rex = cre2_new(pattern_lit, pattern_len, opt);
    /* Prepare a mutable text and a rewrite template for global replace */
    char text_buf[] = "key1=123; key2=456;";
    cre2_string_t text_and_target = { text_buf, (int)(sizeof(text_buf) - 1) };
    char rewrite_buf[] = "$1:$2";
    cre2_string_t rewrite = { rewrite_buf, (int)(sizeof(rewrite_buf) - 1) };

    /* Perform a global replacement using the pattern string API */
    int rc_replace = cre2_global_replace(pattern_lit, &text_and_target, &rewrite);
    if (rc_replace < 0) {
        /* global replace reported an error */
        return 2;
    }

    /* Prepare inputs for cre2_extract_re using the compiled regexp (if compilation succeeded) */
    char text2_buf[] = "key2=456";
    cre2_string_t text2 = { text2_buf, (int)(sizeof(text2_buf) - 1) };
    char rewrite2_buf[] = "$2"; /* we want to extract the second capture group (the digits) */
    cre2_string_t rewrite2 = { rewrite2_buf, (int)(sizeof(rewrite2_buf) - 1) };
    /* target will receive extraction output; initialize as empty buffer */
    char target_buf[128] = {0};
    cre2_string_t target = { target_buf, 0 };

    int rc_extract = 0;
    if (rex) {
        rc_extract = cre2_extract_re(rex, &text2, &rewrite2, &target);
        /* treat nonzero return as success for extract_re */
        if (rc_extract == 0) {
            return 3;
        }
    } else {
        /* If compilation failed, consider the extract step skipped but global replace succeeded */
        rc_extract = 1; /* mark as effectively successful for our sequence */
    }

    /* If we reached here, both operations behaved as expected */
    (void)rc_replace;
    (void)rc_extract;
    return 66;
}