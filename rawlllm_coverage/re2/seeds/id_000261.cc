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
//<ID> 261
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // Create options
    cre2_options_t *opt = cre2_opt_new();
    if (!opt) return 1;

    // Set some options (use setters available in the library)
    cre2_opt_set_dot_nl(opt, 1);
    cre2_opt_set_never_capture(opt, 0);

    // Use the required getters
    int dot_nl_flag = cre2_opt_dot_nl(opt);
    int never_capture_flag = cre2_opt_never_capture(opt);
    (void)dot_nl_flag;
    (void)never_capture_flag;

    // Prepare a pattern and compile a regexp with the options
    const char pattern[] = "(he)llo(world)";
    cre2_regexp_t *re = cre2_new(pattern, (int)(sizeof(pattern) - 1), opt);
    if (!re) {
        cre2_opt_delete(opt);
        return 2;
    }

    // Prepare input text and match storage
    char text_buf[] = "helloworld and beyond";
    cre2_string_t text = { text_buf, (int)(sizeof(text_buf) - 1) };

    // Prepare an array for captures: entire match + two groups = 3
    cre2_string_t matches[3] = { {nullptr, 0}, {nullptr, 0}, {nullptr, 0} };

    // Attempt to consume from the start using the compiled regexp
    int consumed = cre2_consume_re(re, &text, matches, 3);
    (void)consumed; // Accept both success/failure for this test sequence

    // Prepare a rewrite string and a target for extraction
    const char rewrite_s[] = "$1-$2";
    cre2_string_t rewrite = { rewrite_s, (int)(sizeof(rewrite_s) - 1) };
    cre2_string_t target = { nullptr, 0 };

    // Attempt to extract using the compiled regexp
    int extracted = cre2_extract_re(re, &text, &rewrite, &target);
    (void)extracted;
    (void)target; // target may be filled by the library

    // Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    return 66;
}