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
//<ID> 94
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    /* Prepare options and set literal mode */
    cre2_options_t *opt = cre2_opt_new();
    if (!opt) return 1;
    cre2_opt_set_literal(opt, 1);

    /* Compile a simple pattern with the options */
    const char *pattern = "a+b";
    int pattern_len = (int)(sizeof("a+b") - 1);
    cre2_regexp_t *re = cre2_new(pattern, pattern_len, opt);
    if (!re) {
        cre2_opt_delete(opt);
        return 2;
    }

    /* Query any error code (0 normally means no error) */
    int err = cre2_error_code(re);
    (void)err; /* keep variable referenced for correctness */

    /* Prepare an original string containing metacharacters and ask cre2 to quote it */
    cre2_string_t original;
    original.data = "a+b?c.*";
    original.length = (int)(sizeof("a+b?c.*") - 1);
    cre2_string_t quoted;
    quoted.data = NULL;
    quoted.length = 0;
    int qres = cre2_quote_meta(&quoted, &original);
    (void)qres;

    /* Use cre2_find_and_consume with the same simple pattern on a text */
    cre2_string_t text;
    text.data = "prefix a+b suffix";
    text.length = (int)(sizeof("prefix a+b suffix") - 1);
    cre2_string_t match;
    match.data = NULL;
    match.length = 0;
    int fres = cre2_find_and_consume(pattern, &text, &match, 1);
    (void)fres;

    /* Clean up */
    cre2_delete(re);
    cre2_opt_delete(opt);

    return 66;
}