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
//<ID> 224
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // Create options and enable Perl character classes.
    cre2_options_t *opt = cre2_opt_new();
    if (opt) {
        cre2_opt_set_perl_classes(opt, 1);
    }

    // Pattern for use with cre2_new and cre2_replace.
    const char *pattern = "a(b)c";
    int patlen = 0;
    while (pattern[patlen]) ++patlen;

    // Compile a regexp using the options.
    cre2_regexp_t *re = cre2_new(pattern, patlen, opt);
    if (re) {
        // Query the stored pattern string from the compiled regexp.
        const char *stored = cre2_pattern(re);
        (void)stored; // silence unused-variable warnings in test contexts

        // Destroy the compiled regexp.
        cre2_delete(re);
        re = nullptr;
    }

    // Prepare a buffer to be used as text_and_target for replacement.
    // Use a mutable buffer so that implementations that modify the buffer can do so.
    char buffer[] = "axc abc axc a1c";
    int buflen = 0;
    while (buffer[buflen]) ++buflen;
    cre2_string_t text_and_target = { buffer, buflen };

    // Replacement string
    const char *replace_str = "Z";
    int rlen = 0;
    while (replace_str[rlen]) ++rlen;
    cre2_string_t rewrite = { replace_str, rlen };

    // Perform a single replace using the pattern (not the compiled regexp).
    // The return value is intentionally captured but not asserted here.
    int replace_rc = cre2_replace(pattern, &text_and_target, &rewrite);
    (void)replace_rc;

    // Clean up options.
    if (opt) cre2_opt_delete(opt);

    return 66;
}