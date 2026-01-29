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
//<ID> 50
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // Create options and set literal mode
    cre2_options_t *opt = cre2_opt_new();
    if (!opt) return 1;
    cre2_opt_set_literal(opt, 1);

    // Prepare a pattern and compile a regexp using the options
    const char pattern[] = "a(b)c";
    int patlen = (int)(sizeof(pattern) - 1);
    cre2_regexp_t *re = cre2_new(pattern, patlen, opt);
    if (!re) {
        cre2_opt_delete(opt);
        return 2;
    }

    // Query number of capturing groups
    int groups = cre2_num_capturing_groups(re);

    // Use cre2_full_match with a simple input and a match slot
    const char text[] = "abc";
    cre2_string_t input = { text, (int)(sizeof(text) - 1) };
    cre2_string_t match = { nullptr, 0 };
    int full_match_res = cre2_full_match(pattern, &input, &match, 1);

    // Use cre2_replace to replace a single character in a small buffer
    // Provide a mutable buffer for text_and_target (cre2_string_t stores a const char* but a char[] can convert)
    char text_buf[] = "abc";
    cre2_string_t text_and_target = { text_buf, (int)(sizeof(text_buf) - 1) };
    cre2_string_t rewrite = { "X", 1 };
    int replace_res = cre2_replace("b", &text_and_target, &rewrite);

    // Clean up
    cre2_delete(re);
    cre2_opt_delete(opt);

    // Use results to avoid unused-variable warnings (treat negative groups as an error)
    if (groups < 0) return 3;
    (void)full_match_res;
    (void)replace_res;

    return 66;
}