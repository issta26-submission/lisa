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
//<ID> 74
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // Create options and set one_line flag
    cre2_options_t *opt = cre2_opt_new();
    if (!opt) return 1;
    cre2_opt_set_one_line(opt, 1);

    // Create a set, add a simple pattern, compile it, then delete the set
    cre2_set *set = cre2_set_new(opt, (cre2_anchor_t)0);
    if (set) {
        cre2_set_add_simple(set, "abc");
        cre2_set_compile(set);
        cre2_set_delete(set); // required API use
        set = nullptr;
    }

    // Prepare a pattern and build a regexp from it
    const char pattern_literal[] = "a(b)c";
    int pattern_len = (int)(sizeof(pattern_literal) - 1);
    cre2_regexp_t *re = cre2_new(pattern_literal, pattern_len, opt);
    if (!re) {
        cre2_opt_delete(opt);
        return 1;
    }

    // Query number of capturing groups (required API use)
    int ncap = cre2_num_capturing_groups(re);
    (void)ncap; // use value (avoid unused warning)

    // Quote meta characters from the original pattern (required API use)
    cre2_string_t original = { pattern_literal, pattern_len };
    cre2_string_t quoted = { nullptr, 0 };
    cre2_quote_meta(&quoted, &original);

    // Prepare text and rewrite for a single replace call (required API use)
    char textbuf[] = "a(b)c a(b)c";
    cre2_string_t text_and_target = { textbuf, (int)(sizeof(textbuf) - 1) };
    cre2_string_t rewrite = { "X", 1 };
    cre2_replace(pattern_literal, &text_and_target, &rewrite);

    // Clean up regexp and options
    cre2_delete(re);
    cre2_opt_delete(opt);

    return 66;
}