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
//<ID> 87
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // Create options and enable word-boundary handling.
    cre2_options_t *opt = cre2_opt_new();
    if (opt == NULL) {
        // Nothing to clean up; return success sentinel as required.
        return 66;
    }
    cre2_opt_set_word_boundary(opt, 1);

    // Prepare a simple pattern and compile it with the options.
    char pattern[] = "(abc)";
    int pattern_len = (int)(sizeof(pattern) - 1);
    cre2_regexp_t *re = cre2_new(pattern, pattern_len, opt);

    // Prepare text and rewrite buffers for replace_re.
    char text_buf[] = "xxx abc yyy";
    cre2_string_t text_and_target = { text_buf, (int)(sizeof(text_buf) - 1) };

    char rewrite_buf[] = "[$1]";
    cre2_string_t rewrite = { rewrite_buf, (int)(sizeof(rewrite_buf) - 1) };

    // Perform a replacement using the compiled regex (if compilation succeeded).
    if (re != NULL) {
        // call the required API cre2_replace_re
        (void)cre2_replace_re(re, &text_and_target, &rewrite);
    }

    // Prepare input and matches for a partial match using the pattern string API.
    char input_buf[] = "abc";
    cre2_string_t input = { input_buf, (int)(sizeof(input_buf) - 1) };

    cre2_string_t matches[2];
    matches[0].data = NULL; matches[0].length = 0;
    matches[1].data = NULL; matches[1].length = 0;

    // call the required API cre2_partial_match
    (void)cre2_partial_match(pattern, &input, matches, 2);

    // Cleanup compiled regex and options. Use the required cre2_opt_delete.
    if (re != NULL) {
        cre2_delete(re);
    }
    cre2_opt_delete(opt);

    return 66;
}