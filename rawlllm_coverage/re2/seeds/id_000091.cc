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
//<ID> 91
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // Create options and enable longest-match behavior.
    cre2_options_t *opt = cre2_opt_new();
    if (opt) {
        cre2_opt_set_longest_match(opt, 1);
    }

    // Pattern and text (lengths provided explicitly).
    const char *pattern = "ab(c)?"; // length = 6
    const int pattern_len = 6;
    const char *text = "xababc";    // length = 6
    const int text_len = 6;

    // Compile the regular expression with the options.
    cre2_regexp_t *re = cre2_new(pattern, pattern_len, opt);

    // Prepare match arrays for cre2_match and cre2_partial_match_re.
    cre2_string_t matches[3];
    for (int i = 0; i < 3; ++i) {
        matches[i].data = NULL;
        matches[i].length = 0;
    }

    // Use cre2_match to attempt a match over the C string.
    if (re) {
        int matched = cre2_match(re, text, text_len, 0, text_len, CRE2_UNANCHORED, matches, 3);
        (void)matched; // suppress unused-variable warning; result intentionally ignored
    }

    // Prepare a cre2_string_t representing the input text for partial match API.
    cre2_string_t text_str;
    text_str.data = text;
    text_str.length = text_len;

    cre2_string_t p_matches[3];
    for (int i = 0; i < 3; ++i) {
        p_matches[i].data = NULL;
        p_matches[i].length = 0;
    }

    // Use cre2_partial_match_re to attempt a partial match using the compiled regexp.
    if (re) {
        int pmatched = cre2_partial_match_re(re, &text_str, p_matches, 3);
        (void)pmatched; // intentionally ignore result
    }

    // Clean up.
    if (re) cre2_delete(re);
    if (opt) cre2_opt_delete(opt);

    return 66;
}