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
//<ID> 257
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // Create options and enable POSIX syntax, then read it back.
    cre2_options_t *opt = cre2_opt_new();
    if (opt) {
        cre2_opt_set_posix_syntax(opt, 1);
        // Use required API: query the posix_syntax flag
        int posix_flag = cre2_opt_posix_syntax(opt);
        (void)posix_flag; // keep compiler quiet if unused
    }

    // Compile a pattern with a named capturing group and a regular group.
    const char pattern[] = "(?P<name>abc)(def)";
    cre2_regexp_t *re = cre2_new(pattern, (int)strlen(pattern), opt);

    // If regex compiled, iterate named groups (uses required API cre2_named_groups_iter_next).
    cre2_named_groups_iter_t *iter = nullptr;
    if (re) {
        iter = cre2_named_groups_iter_new(re);
        if (iter) {
            const char *name = nullptr;
            int index = 0;
            // iterate through named groups (if any)
            while (cre2_named_groups_iter_next(iter, &name, &index)) {
                // Minimal use: observe name and index (no-op).
                if (name) {
                    (void)name;
                    (void)index;
                }
            }
            // delete iterator when done
            cre2_named_groups_iter_delete(iter);
            iter = nullptr;
        }
    }

    // Prepare buffers for consume_re: input text and an array for submatches.
    // Use mutable char arrays to be safe if the library accesses memory.
    char input_buf[] = "abcdef";
    cre2_string_t input = { input_buf, (int)strlen(input_buf) };
    // Reserve space for up to 4 submatches (full match + groups).
    cre2_string_t submatches[4];
    for (int i = 0; i < 4; ++i) {
        submatches[i].data = nullptr;
        submatches[i].length = 0;
    }

    // Use required API: try to consume from the beginning of the input.
    if (re) {
        int consumed = cre2_consume_re(re, &input, submatches, 4);
        (void)consumed; // treat as informational in this test sequence
    }

    // Prepare a text-and-target buffer and a rewrite string for replace_re.
    char text_and_target_buf[] = "abc def abc";
    cre2_string_t text_and_target = { text_and_target_buf, (int)strlen(text_and_target_buf) };
    char rewrite_buf[] = "$1X"; // simple rewrite (may be pattern-dependent)
    cre2_string_t rewrite = { rewrite_buf, (int)strlen(rewrite_buf) };

    // Use required API: perform a replace operation using the compiled regex.
    if (re) {
        int replaced = cre2_replace_re(re, &text_and_target, &rewrite);
        (void)replaced;
    }

    // Clean up resources.
    if (re) cre2_delete(re);
    if (opt) cre2_opt_delete(opt);

    return 66;
}