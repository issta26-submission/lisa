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
//<ID> 285
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // Create options and enable Perl character classes
    cre2_options_t *opt = cre2_opt_new();
    if (!opt) return 1;
    cre2_opt_set_perl_classes(opt, 1);

    // Pattern with a named capturing group "word"
    const char pattern[] = "(?P<word>\\w+)";
    cre2_regexp_t *re = cre2_new(pattern, (int)(sizeof(pattern) - 1), opt);
    if (!re) {
        cre2_opt_delete(opt);
        return 2;
    }

    // Query the named capturing group index
    int idx = cre2_find_named_capturing_groups(re, "word");
    (void)idx; // use if needed; silence unused-warning

    // Prepare text and rewrite strings for global replace
    const char text_buf[] = "hello 123 world";
    cre2_string_t text_and_target = { text_buf, (int)(sizeof(text_buf) - 1) };

    const char repl_buf[] = "X";
    cre2_string_t rewrite = { repl_buf, (int)(sizeof(repl_buf) - 1) };

    // Perform a global replace using the compiled regex
    int replaced = cre2_global_replace_re(re, &text_and_target, &rewrite);
    (void)replaced; // accept the result; silence unused-warning

    // Clean up
    cre2_delete(re);
    cre2_opt_delete(opt);

    return 66;
}