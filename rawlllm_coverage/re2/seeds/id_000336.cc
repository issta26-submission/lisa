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
//<ID> 336
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // Create options and enable logging of errors.
    cre2_options_t *opt = cre2_opt_new();
    cre2_opt_set_log_errors(opt, 1);

    // Create a set using the options (anchor unanchored).
    cre2_set *set = cre2_set_new(opt, CRE2_UNANCHORED);

    // Prepare a pattern and compile a regexp using the same options.
    const char pattern[] = "a(b)c";
    int pattern_len = (int)(sizeof(pattern) - 1);
    cre2_regexp_t *re = cre2_new(pattern, pattern_len, opt);

    // Prepare input text and cre2_string_t containers.
    const char text[] = "zabczz";
    int text_len = (int)(sizeof(text) - 1);
    cre2_string_t text_str = { text, text_len };
    cre2_string_t consumed_match = { nullptr, 0 };
    cre2_string_t match_out = { nullptr, 0 };

    // Use cre2_find_and_consume with a simple pattern "b" to exercise that API.
    int rc1 = cre2_find_and_consume("b", &text_str, &consumed_match, 1);

    // Use cre2_match to attempt matching the compiled regexp against the text.
    int rc2 = cre2_match(re, text, text_len, 0, text_len, CRE2_UNANCHORED, &match_out, 2);

    // Silence unused-result warnings (we don't enforce specific outcomes here).
    (void)rc1;
    (void)rc2;
    (void)consumed_match.data;
    (void)match_out.data;

    // Clean up resources in a safe order.
    if (re) cre2_delete(re);
    if (set) cre2_set_delete(set);
    if (opt) cre2_opt_delete(opt);

    return 66;
}