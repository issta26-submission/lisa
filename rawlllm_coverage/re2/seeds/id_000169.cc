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
//<ID> 169
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // Create options
    cre2_options_t *opt = cre2_opt_new();
    if (!opt) {
        return 66;
    }

    // Query a couple of option flags (no-op getters for testing)
    int dot_nl_flag = cre2_opt_dot_nl(opt);
    (void)dot_nl_flag;
    int posix_flag = cre2_opt_posix_syntax(opt);
    (void)posix_flag;

    // Create a set with the options (anchor unanchored)
    cre2_set *set = cre2_set_new(opt, CRE2_UNANCHORED);

    // Build a simple regexp and input text
    const char *pattern = "(ab)(c)?";
    int pattern_len = (int)strlen(pattern);
    cre2_regexp_t *re = cre2_new(pattern, pattern_len, opt);

    // Prepare text and match storage for find_and_consume
    const char *input = "abcxyz";
    cre2_string_t text;
    text.data = input;
    text.length = (int)strlen(input);

    // Reserve space for up to 3 matches (full + 2 groups)
    cre2_string_t matches[3];
    for (int i = 0; i < 3; ++i) {
        matches[i].data = nullptr;
        matches[i].length = 0;
    }

    // If regexp compiled, attempt find_and_consume_re
    if (re) {
        // nmatch = 3 (space provided)
        int nmatched = cre2_find_and_consume_re(re, &text, matches, 3);
        (void)nmatched; // ignore result for this test sequence
    }

    // Cleanup
    if (re) cre2_delete(re);
    if (set) cre2_set_delete(set);
    cre2_opt_delete(opt);

    return 66;
}