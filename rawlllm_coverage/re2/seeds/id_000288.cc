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
//<ID> 288
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // create options
    cre2_options_t *opt = cre2_opt_new();
    if (!opt) {
        // If we couldn't allocate options, nothing else to do.
        return 66;
    }

    // query some option-derived values
    cre2_encoding_t enc = cre2_opt_encoding(opt);
    auto maxmem = cre2_opt_max_mem(opt);

    // Prepare a pattern and compile a regexp with the options.
    // Use array form so we can compute lengths without calling strlen.
    const char pattern[] = "(foo)(bar)?";
    int pattern_len = (int)(sizeof(pattern) - 1);
    cre2_regexp_t *re = cre2_new(pattern, pattern_len, opt);

    // Prepare input text as a cre2_string_t
    const char text_buf[] = "foobarbaz";
    cre2_string_t text;
    text.data = text_buf;
    text.length = (int)(sizeof(text_buf) - 1);

    // Prepare match storage: allow for up to 3 matches (full + 2 groups)
    const int nmatch = 3;
    cre2_string_t matches[nmatch];
    for (int i = 0; i < nmatch; ++i) {
        matches[i].data = nullptr;
        matches[i].length = 0;
    }

    // If compilation succeeded, attempt to consume from the front of text.
    if (re) {
        int rc = cre2_consume_re(re, &text, matches, nmatch);
        (void)rc; // ignore detailed result for this test harness
        cre2_delete(re);
    }

    // Use the queried values to avoid unused-variable warnings.
    (void)enc;
    (void)maxmem;

    // clean up options
    cre2_opt_delete(opt);

    return 66;
}