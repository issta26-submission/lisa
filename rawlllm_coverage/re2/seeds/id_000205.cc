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
//<ID> 205
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    /* Create options for building the regexp */
    cre2_options_t *opt = cre2_opt_new();
    if (!opt) return -1;

    /* Original literal that we'll safely quote for use as a pattern */
    cre2_string_t original = { "a+b", 3 }; /* pattern contains a metacharacter '+' */
    cre2_string_t quoted = { NULL, 0 };

    /* Quote the original so it becomes a literal pattern */
    int qok = cre2_quote_meta(&quoted, &original);

    /* Decide which pattern to use (quoted if quoting succeeded, otherwise original) */
    const char *pattern_ptr = original.data;
    int pattern_len = original.length;
    if (qok && quoted.data && quoted.length > 0) {
        pattern_ptr = quoted.data;
        pattern_len = quoted.length;
    }

    /* Compile the regexp */
    cre2_regexp_t *re = cre2_new(pattern_ptr, pattern_len, opt);
    if (!re) {
        cre2_opt_delete(opt);
        return -2;
    }

    /* Prepare input text to consume from */
    cre2_string_t text = { "a+babc", 6 }; /* contains the literal "a+b" at the start */
    cre2_string_t match = { NULL, 0 };

    /* Attempt to consume the pattern from the beginning of text.
       The last int argument is passed as 1 (typical for asking for a single match). */
    int consumed = cre2_consume_re(re, &text, &match, 1);

    /* We don't rely on the contents of match (implementation-specific memory handling),
       but ensure we called the API sequence safely. */

    /* Cleanup */
    cre2_delete(re);
    cre2_opt_delete(opt);

    /* Return 66 to signal success of the test sequence regardless of whether
       the pattern matched (consumed). */
    (void)consumed; /* silence unused-variable warnings if any */
    return 66;
}