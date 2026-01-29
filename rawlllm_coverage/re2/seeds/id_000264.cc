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
//<ID> 264
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    /* create options and enable Perl character classes */
    cre2_options_t *opt = cre2_opt_new();
    if (!opt) {
        return 66;
    }
    cre2_opt_set_perl_classes(opt, 1);

    /* query encoding and ensure it's set (use UTF8 if unknown) */
    cre2_encoding_t enc = cre2_opt_encoding(opt);
    if (enc == CRE2_UNKNOWN) {
        cre2_opt_set_encoding(opt, CRE2_UTF8);
    }

    /* compile a pattern with the options */
    static const char pattern[] = "(he)llo\\s(world)";
    int pat_len = (int)(sizeof(pattern) - 1);
    cre2_regexp_t *re = cre2_new(pattern, pat_len, opt);
    if (!re) {
        cre2_opt_delete(opt);
        return 66;
    }

    /* prepare input text and match containers */
    static const char text[] = "hello world";
    cre2_string_t text_str = { text, (int)(sizeof(text) - 1) };
    cre2_string_t matches[4];
    for (int i = 0; i < 4; ++i) {
        matches[i].data = NULL;
        matches[i].length = 0;
    }

    /* use the simple (pattern string) consume API */
    int consume_res = cre2_consume(pattern, &text_str, matches, 2);
    (void)consume_res; /* result intentionally ignored for this sequence */

    /* use the regexp-based partial match API */
    int partial_res = cre2_partial_match_re(re, &text_str, matches, 4);
    (void)partial_res;

    /* cleanup */
    cre2_delete(re);
    cre2_opt_delete(opt);

    return 66;
}