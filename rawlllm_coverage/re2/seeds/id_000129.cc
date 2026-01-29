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
//<ID> 129
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // Create options and query the dot_nl flag.
    cre2_options_t *opt = cre2_opt_new();
    if (!opt) {
        // If options couldn't be allocated, still treat as failure (not success).
        return 1;
    }
    int dot_nl_flag = cre2_opt_dot_nl(opt);
    (void)dot_nl_flag; // use value to avoid unused-variable warnings

    // Use cre2_easy_match on a simple pattern/text without compiling a regexp object.
    const char *easy_pattern = "abc";
    int easy_pattern_len = (int)strlen(easy_pattern);
    const char *easy_text = "xxabcyy";
    int easy_text_len = (int)strlen(easy_text);

    // Prepare match array for cre2_easy_match. We request 2 slots (whole match + one group).
    cre2_string_t easy_matches[2];
    // It's okay to leave .data/.length uninitialized; the function will populate them on match.
    int easy_result = cre2_easy_match(easy_pattern, easy_pattern_len,
                                      easy_text, easy_text_len,
                                      easy_matches, 2);
    (void)easy_result; // ignore result for this test, but call is exercised

    // Compile a regexp using the options and exercise cre2_partial_match_re.
    const char *pattern = "(abc)(yy)?";
    int pattern_len = (int)strlen(pattern);
    cre2_regexp_t *re = cre2_new(pattern, pattern_len, opt);

    if (re) {
        // Prepare input as a single cre2_string_t and an output array for matches (3 slots).
        cre2_string_t input = { easy_text, easy_text_len };
        cre2_string_t out_matches[3];
        // Call partial match with the compiled regexp.
        int partial_result = cre2_partial_match_re(re, &input, out_matches, 3);
        (void)partial_result; // we don't assert on the result here

        // Clean up the compiled regexp.
        cre2_delete(re);
    }

    // Delete the options we created earlier (required by the task).
    cre2_opt_delete(opt);

    // Indicate success per the instructions.
    return 66;
}