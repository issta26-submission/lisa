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
//<ID> 338
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // Create options and set the one-line option.
    cre2_options_t *opt = cre2_opt_new();
    if (!opt) {
        // Unable to create options; nothing to clean up.
        return 66;
    }
    cre2_opt_set_one_line(opt, 1);

    // Prepare a pattern and compile it with the options.
    const char *pattern = "ab+c";
    int pattern_len = 4; // length of "ab+c"
    cre2_regexp_t *re = cre2_new(pattern, pattern_len, opt);

    // Prepare original cre2_string_t and a buffer for the quoted result.
    cre2_string_t original;
    original.data = pattern;
    original.length = pattern_len;

    char quoted_buf[256] = {0};
    cre2_string_t quoted;
    quoted.data = quoted_buf;
    quoted.length = (int)sizeof(quoted_buf);

    // Quote meta-characters from original into quoted.
    // We ignore the return value but call the API to exercise it.
    (void)cre2_quote_meta(&quoted, &original);

    if (re) {
        // Prepare input text and a match array for full match call.
        const char *text_c = "abbbbbc";
        cre2_string_t text;
        text.data = text_c;
        text.length = 7; // length of "abbbbbc"

        const int nmatch = 4;
        cre2_string_t matches[nmatch];
        for (int i = 0; i < nmatch; ++i) {
            matches[i].data = nullptr;
            matches[i].length = 0;
        }

        // Attempt a full match using the compiled regexp.
        // We ignore the return value but call the API to exercise it.
        (void)cre2_full_match_re(re, &text, matches, nmatch);

        // Clean up the compiled regexp.
        cre2_delete(re);
    }

    // Clean up options.
    cre2_opt_delete(opt);

    return 66;
}