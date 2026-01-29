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
//<ID> 25
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // Create options
    cre2_options_t *opt = cre2_opt_new();
    if (!opt) return 0;

    // Create a set using the options. Use a zero anchor value.
    cre2_set *set = cre2_set_new(opt, (cre2_anchor_t)0);
    if (!set) {
        cre2_opt_delete(opt);
        return 0;
    }

    // Add a simple pattern to the set and compile it.
    cre2_set_add_simple(set, "(foo)(bar)");
    int compile_result = cre2_set_compile(set);
    (void)compile_result; // ignore result for this test sequence

    // Create a regexp directly.
    const char *pattern = "(foo)(bar)?";
    int pattern_len = 0;
    while (pattern[pattern_len] != '\0') ++pattern_len;

    cre2_regexp_t *re = cre2_new(pattern, pattern_len, opt);
    if (!re) {
        cre2_set_delete(set);
        cre2_opt_delete(opt);
        return 0;
    }

    // Query number of capturing groups.
    int num_groups = cre2_num_capturing_groups(re);
    if (num_groups < 0) num_groups = 0;
    int nmatch = num_groups + 1; // include whole-match slot

    // Prepare input text and match output array.
    cre2_string_t text;
    text.data = "foobar";
    text.length = 6;

    cre2_string_t *matches = new cre2_string_t[nmatch];
    for (int i = 0; i < nmatch; ++i) {
        matches[i].data = nullptr;
        matches[i].length = 0;
    }

    // Attempt a partial match using the compiled regexp.
    int partial_ok = cre2_partial_match_re(re, &text, matches, nmatch);
    (void)partial_ok; // result not asserted here; just exercising the API

    // Cleanup
    delete[] matches;
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    return 66;
}