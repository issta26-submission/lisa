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
//<ID> 358
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // Query the current ABI/interface version (no-op other than demonstrating API use).
    int version = cre2_version_interface_current();
    (void)version;

    // Create default options.
    cre2_options_t *opt = cre2_opt_new();
    if (!opt) {
        return 66; // still return success code per requirement, but ensure opt was attempted.
    }

    // Compile a regexp from a pattern (pattern length given explicitly).
    const char *pattern = "a(b*)c";
    int pattern_len = 6; // 'a' '(' 'b' '*' ')' 'c' => 6 bytes
    cre2_regexp_t *re = cre2_new(pattern, pattern_len, opt);

    // Prepare containers for possible-match-range output.
    cre2_string_t min_range = { nullptr, 0 };
    cre2_string_t max_range = { nullptr, 0 };

    if (re) {
        // Ask for possible match range with a small maxlen.
        int pmr_res = cre2_possible_match_range(re, &min_range, &max_range, 16);
        (void)pmr_res; // ignore detailed result for this test sequence
    }

    // Demonstrate the convenience non-compiled consume API:
    // prepare input text as cre2_string_t
    cre2_string_t input = { "aaab", 4 };
    cre2_string_t submatch = { nullptr, 0 };
    // Use a simple pattern string with cre2_consume.
    int consume_res = cre2_consume("a+", &input, &submatch, 1);
    (void)consume_res;
    (void)submatch; // submatch may have been set by the call

    // Use a set: create, add a pattern, compile, and run a match.
    cre2_set *set = cre2_set_new(opt, (cre2_anchor_t)0);
    if (set) {
        // add a simple pattern to the set
        cre2_set_add_simple(set, "a+");
        // compile the set
        cre2_set_compile(set);

        // Prepare an array to receive match indices (opaque semantics; size chosen conservatively).
        int match_indices[8] = {0};
        size_t nmatched = cre2_set_match(set, input.data, (size_t)input.length, match_indices, 8);
        (void)nmatched;
        (void)match_indices;

        // Clean up set
        cre2_set_delete(set);
    }

    // Clean up regexp and options.
    if (re) {
        cre2_delete(re);
    }
    cre2_opt_delete(opt);

    return 66;
}