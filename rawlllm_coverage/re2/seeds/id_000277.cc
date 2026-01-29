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
//<ID> 277
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // Create options and set a couple of flags required by the task.
    cre2_options_t *opt = cre2_opt_new();
    if (opt) {
        cre2_opt_set_log_errors(opt, 1);
        cre2_opt_set_dot_nl(opt, 1);
    }

    // Prepare a simple pattern and compile it with the options (if available).
    const char *pattern = "a";
    cre2_regexp_t *rex = cre2_new(pattern, (int)strlen(pattern), opt);

    // Prepare text/target and rewrite buffers for replace_re.
    char textbuf[] = "abc";
    cre2_string_t text_and_target = { textbuf, (int)strlen(textbuf) };
    // rewrite will be updated by cre2_replace_re; provide a writable buffer pointer (const char* in struct is allowed).
    // We use an internal buffer and set length to 0 initially.
    static char rewrite_storage[64];
    cre2_string_t rewrite = { rewrite_storage, 0 };

    // If compilation succeeded, attempt a replacement using the compiled regexp.
    if (rex) {
        (void)cre2_replace_re(rex, &text_and_target, &rewrite);
    }

    // Use cre2_partial_match (pattern string API) against a cre2_string_t input.
    cre2_string_t input = { "abc", 3 };
    cre2_string_t match_out = { nullptr, 0 };
    (void)cre2_partial_match(pattern, &input, &match_out, 1);

    // Clean up compiled regexp and options.
    if (rex) cre2_delete(rex);
    if (opt) cre2_opt_delete(opt);

    // Exercise cre2_set_delete as requested; deleting a nullptr is safe to call.
    cre2_set_delete(nullptr);

    return 66;
}