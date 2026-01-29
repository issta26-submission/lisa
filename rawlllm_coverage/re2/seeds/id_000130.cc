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
//<ID> 130
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // Prepare literal pattern and text so we can compute lengths without strlen.
    const char pattern_literal[] = "ab(c)";
    const int pattern_len = (int)(sizeof(pattern_literal) - 1);

    const char text_literal[] = "xxabccc";
    const int text_len = (int)(sizeof(text_literal) - 1);

    // Create options
    cre2_options_t *opt = cre2_opt_new();
    // Create a compiled regexp using the options
    cre2_regexp_t *re = nullptr;
    if (opt) {
        re = cre2_new(pattern_literal, pattern_len, opt);
    }

    // Prepare cre2_string_t for partial match API
    cre2_string_t text_str;
    text_str.data = text_literal;
    text_str.length = text_len;

    // Prepare an array for captured substrings (initialized empty)
    cre2_string_t captures[2];
    for (int i = 0; i < 2; ++i) {
        captures[i].data = nullptr;
        captures[i].length = 0;
    }

    // Use cre2_partial_match with the pattern string (direct pattern-based API)
    // Attempt one capture (nmatch = 1)
    int partial_res = cre2_partial_match(pattern_literal, &text_str, captures, 1);
    (void)partial_res; // result examined only for side effects in this test sequence

    // Create a set, add the same pattern, compile it and perform a set match
    cre2_set *set = nullptr;
    // cre2_anchor_t is provided by the cre2 headers; pass 0 as a valid enum value for unanchored in this test.
    set = cre2_set_new(opt, (cre2_anchor_t)0);
    if (set) {
        // Add pattern simply
        cre2_set_add_simple(set, pattern_literal);
        // Compile the set
        cre2_set_compile(set);
        // Prepare match result buffer
        int match_ids[8];
        for (size_t i = 0; i < sizeof(match_ids)/sizeof(match_ids[0]); ++i) match_ids[i] = -1;
        // Perform set match on the same text
        size_t nmatched = cre2_set_match(set, text_literal, (size_t)text_len, match_ids, sizeof(match_ids)/sizeof(match_ids[0]));
        (void)nmatched;
    }

    // Clean up allocated objects if they were created
    if (re) cre2_delete(re);
    if (set) cre2_set_delete(set);
    if (opt) cre2_opt_delete(opt);

    // Indicate success for this test harness
    return 66;
}