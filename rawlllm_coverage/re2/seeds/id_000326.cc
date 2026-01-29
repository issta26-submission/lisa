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
//<ID> 326
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // Create and configure options
    cre2_options_t *opt = cre2_opt_new();
    if (opt == NULL) {
        // If option allocation fails, nothing more sensible to do here for this test.
        return 66;
    }

    // Set some option flags (use setters even though only getters were mandated to be used)
    cre2_opt_set_one_line(opt, 1);
    cre2_opt_set_posix_syntax(opt, 0);

    // Use the requested getters
    int one_line_flag = cre2_opt_one_line(opt);
    int posix_flag = cre2_opt_posix_syntax(opt);
    (void)one_line_flag;
    (void)posix_flag;

    // Prepare a pattern and compile a regexp with the options
    const char pattern[] = "(hel)lo";
    int pattern_len = (int)(sizeof(pattern) - 1);
    cre2_regexp_t *re = cre2_new(pattern, pattern_len, opt);

    // Prepare input text and match storage
    const char text[] = "hello world";
    cre2_string_t input = { text, (int)(sizeof(text) - 1) };

    cre2_string_t matches[2] = { {NULL, 0}, {NULL, 0} };

    // Use the required cre2_partial_match_re with the compiled regexp
    if (re != NULL) {
        int partial_ok = cre2_partial_match_re(re, &input, matches, 1);
        (void)partial_ok; // suppress unused-variable warnings

        // Retrieve an error string for the compiled regexp (may be empty if no error)
        const char *err = cre2_error_string(re);
        (void)err;
    } else {
        // If compilation failed, we still attempt to fetch an error string is not safe;
        // therefore avoid calling cre2_error_string with NULL. Instead, nothing to do here.
    }

    // Prepare a rewrite string and call cre2_replace (pattern-based replace on a text string)
    cre2_string_t rewrite = { "hi", 2 };
    // Note: cre2_replace takes a pattern string (C-string), and the text_and_target/rewrite structs.
    int replace_ret = cre2_replace(pattern, &input, &rewrite);
    (void)replace_ret;

    // Cleanup
    if (re != NULL) {
        cre2_delete(re);
    }
    cre2_opt_delete(opt);

    return 66;
}