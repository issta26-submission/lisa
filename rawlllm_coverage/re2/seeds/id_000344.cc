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
//<ID> 344
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // Create options and enable word-boundary behavior.
    cre2_options_t *opt = cre2_opt_new();
    if (opt) {
        cre2_opt_set_word_boundary(opt, 1);
    }

    // Prepare a pattern that uses word boundaries and compile a regexp with the options.
    const char pattern[] = "\\bhello\\b";
    cre2_regexp_t *re = cre2_new(pattern, (int)(sizeof(pattern) - 1), opt);

    // Prepare input text and match storage.
    const char text[] = "hello world";
    cre2_string_t input = { text, (int)(sizeof(text) - 1) };
    cre2_string_t matches[2];
    matches[0].data = NULL; matches[0].length = 0;
    matches[1].data = NULL; matches[1].length = 0;

    // If compilation succeeded, try to consume from the start of the input.
    if (re) {
        // nmatch = 2 to capture the full match (and allow one capture group if present).
        (void)cre2_consume_re(re, &input, matches, 2);
        cre2_delete(re);
    }

    // Clean up options.
    if (opt) cre2_opt_delete(opt);

    return 66;
}