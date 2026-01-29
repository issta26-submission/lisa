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
//<ID> 345
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // Create options and set the 'literal' flag
    cre2_options_t *opt = cre2_opt_new();
    if (!opt) return 1;
    cre2_opt_set_literal(opt, 1);

    // Create a set with these options (anchoring unanchored)
    cre2_set *set = cre2_set_new(opt, CRE2_UNANCHORED);
    if (!set) {
        cre2_opt_delete(opt);
        return 2;
    }

    // Add a simple pattern to the set and compile it (ignore errors for this test)
    cre2_set_add_simple(set, "(a)(b(c))?");
    cre2_set_compile(set);

    // Prepare a pattern and compile it into a cre2_regexp_t
    // Use sizeof-literal trick to get pattern length without relying on strlen
    const char pattern_literal[] = "(a)(b(c))?";
    int pattern_len = (int)(sizeof(pattern_literal) - 1);
    cre2_regexp_t *re = cre2_new(pattern_literal, pattern_len, opt);
    if (!re) {
        cre2_set_delete(set);
        cre2_opt_delete(opt);
        return 3;
    }

    // Query number of capturing groups and allocate match array accordingly
    int num_groups = cre2_num_capturing_groups(re);
    if (num_groups < 0) {
        cre2_delete(re);
        cre2_set_delete(set);
        cre2_opt_delete(opt);
        return 4;
    }
    int nmatch = num_groups + 1; // include whole-match slot

    cre2_string_t *matches = new cre2_string_t[nmatch];
    for (int i = 0; i < nmatch; ++i) {
        matches[i].data = nullptr;
        matches[i].length = 0;
    }

    // Prepare input text for a partial match
    const char input_text[] = "ab";
    cre2_string_t input = { input_text, (int)(sizeof(input_text) - 1) };

    // Use cre2_partial_match (pattern as C-string, input, output matches, nmatch)
    int partial_ok = cre2_partial_match(pattern_literal, &input, matches, nmatch);

    // We don't strictly require a particular result for this fuzz-oriented test,
    // but ensure we performed the API calls and cleaned up.
    (void)partial_ok; // silence unused variable warnings if any

    // Cleanup
    delete[] matches;
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    return 66;
}