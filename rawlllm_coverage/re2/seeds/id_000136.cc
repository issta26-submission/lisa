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
//<ID> 136
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // Create options and tweak one option
    cre2_options_t *opt = cre2_opt_new();
    if (opt) {
        cre2_opt_set_case_sensitive(opt, 1);
    }

    // Compile a regular expression
    const char pattern[] = "(h(e)llo) (world)";
    const int pattern_len = 17; // strlen("(h(e)llo) (world)") == 17
    cre2_regexp_t *re = cre2_new(pattern, pattern_len, opt);

    // Prepare input text for matching
    cre2_string_t text = { "hello world", 11 }; // "hello world" length = 11

    // Prepare space for up to 4 submatches (0 = whole match, then groups)
    cre2_string_t submatches[4];
    for (int i = 0; i < 4; ++i) {
        submatches[i].data = nullptr;
        submatches[i].length = 0;
    }

    // Use cre2_partial_match_re to attempt a partial match against the compiled regex
    int partial_ok = 0;
    if (re) {
        partial_ok = cre2_partial_match_re(re, &text, submatches, 4);
    }

    // Check error code for the compiled regexp (even if match failed/was partial)
    int error_code = 0;
    if (re) {
        error_code = cre2_error_code(re);
    }

    // Try extract using the compiled regexp (rewrite supplied as a string)
    cre2_string_t rewrite = { "[$1,$2]", 7 }; // length of "[$1,$2]" == 7
    cre2_string_t target = { nullptr, 0 }; // allow library to populate or indicate failure
    int extract_res = 0;
    if (re) {
        extract_res = cre2_extract_re(re, &text, &rewrite, &target);
    }

    // Create a set, add a simple pattern, compile and run a set match
    cre2_set *set = cre2_set_new(opt, (cre2_anchor_t)0);
    size_t set_matches = 0;
    if (set) {
        // add a simple pattern to the set
        cre2_set_add_simple(set, "hello");
        // compile the set
        cre2_set_compile(set);
        // prepare an array for matched pattern indices
        int match_indices[8] = { -1 };
        set_matches = cre2_set_match(set, "hello world", 11, match_indices, 8);
        // delete the set (required)
        cre2_set_delete(set);
    }

    // Clean up compiled regexp and options
    if (re) cre2_delete(re);
    if (opt) cre2_opt_delete(opt);

    // Silence unused variable warnings (values may be inspected during fuzzing)
    (void)partial_ok;
    (void)error_code;
    (void)extract_res;
    (void)target;
    (void)set_matches;

    return 66;
}