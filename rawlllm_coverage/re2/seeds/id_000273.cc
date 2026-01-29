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
//<ID> 273
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // Create options and set encoding
    cre2_options_t *opt = cre2_opt_new();
    if (!opt) return 1;

    cre2_opt_set_encoding(opt, CRE2_UTF8);

    // Prepare a simple pattern and compile it with the options.
    // Use a fixed-size array so we can compute the pattern length without strlen.
    const char pattern[] = "^(Hello)\\s+(World)";
    int pattern_len = (int)(sizeof(pattern) - 1);

    cre2_regexp_t *re = cre2_new(pattern, pattern_len, opt);
    if (!re) {
        cre2_opt_delete(opt);
        return 2;
    }

    // Prepare input text as a cre2_string_t
    cre2_string_t text;
    text.data = "Hello World";
    text.length = (int)(sizeof("Hello World") - 1);

    // Prepare a cre2_string_t to receive the match (cre2_consume will populate it)
    cre2_string_t match;
    match.data = NULL;
    match.length = 0;

    // Call cre2_consume using the pattern string API variant.
    // Request up to 4 capture slots (including whole match).
    int consume_result = cre2_consume(pattern, &text, &match, 4);
    (void)consume_result; // we don't need to act on the result for this test

    // Prepare an argument container and ask the compiled regexp for its error argument (if any).
    cre2_string_t errarg;
    errarg.data = NULL;
    errarg.length = 0;
    cre2_error_arg(re, &errarg);

    // Clean up
    cre2_delete(re);
    cre2_opt_delete(opt);

    return 66;
}