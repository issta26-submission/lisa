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
//<ID> 214
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // Create options and enable never-capture
    cre2_options_t *opt = cre2_opt_new();
    if (!opt) return 1;
    // Use the required API: set never_capture flag
    cre2_opt_never_capture(opt);

    // Prepare an original pattern and obtain a quoted (meta-escaped) version
    cre2_string_t original = { "a(b)c", 5 };
    cre2_string_t quoted = { nullptr, 0 };
    // Use the required API: quote meta characters from original into quoted
    int qres = cre2_quote_meta(&quoted, &original);
    (void)qres; // ignore detailed result for this test sequence

    // Choose pattern source: prefer quoted if provided, otherwise original
    const char *pattern = (quoted.data != nullptr) ? quoted.data : original.data;
    int pattern_len = (quoted.length != 0) ? quoted.length : original.length;

    // Compile a regexp using the chosen pattern and options
    cre2_regexp_t *re = cre2_new(pattern, pattern_len, opt);
    if (!re) {
        cre2_opt_delete(opt);
        return 2;
    }

    // Prepare text to perform a global replace on and a rewrite string
    char text_buf[] = "xx abc yy";
    cre2_string_t text_and_target = { text_buf, (int)strlen(text_buf) };
    // Replacement that uses the first capture group
    cre2_string_t rewrite = { "Z$1", 3 };

    // Use the required API: perform a global replace using the compiled regexp
    int grepres = cre2_global_replace_re(re, &text_and_target, &rewrite);
    (void)grepres; // accept whatever result for this test sequence

    // Create a set of patterns and add one pattern; supply an error buffer
    cre2_set *set = cre2_set_new(opt, (cre2_anchor_t)0);
    if (set) {
        char error_buf[128] = {0};
        // Use the required API: add a pattern to the set (with error reporting buffer)
        cre2_set_add(set, "abc", (size_t)3, error_buf, sizeof(error_buf));
    }

    // Cleanup resources
    cre2_delete(re);
    if (set) cre2_set_delete(set);
    cre2_opt_delete(opt);

    return 66;
}