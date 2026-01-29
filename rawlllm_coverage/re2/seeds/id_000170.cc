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
//<ID> 170
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // Create options and set the longest-match flag
    cre2_options_t *opt = cre2_opt_new();
    if (!opt) return 1;
    cre2_opt_set_longest_match(opt, 1);

    // Prepare a pattern and compile it with the options
    const char *pattern = "a(bc)?";
    int pattern_len = (int)strlen(pattern);
    cre2_regexp_t *re = cre2_new(pattern, pattern_len, opt);

    // Prepare text and match storage
    const char *text = "abc";
    int textlen = (int)strlen(text);
    cre2_string_t matches[2];
    matches[0].data = nullptr; matches[0].length = 0;
    matches[1].data = nullptr; matches[1].length = 0;

    // Use cre2_match with the compiled regexp if compilation succeeded
    int match_result = 0;
    if (re) {
        match_result = cre2_match(re, text, textlen, 0, textlen, CRE2_UNANCHORED, matches, 2);
    }

    // Also exercise cre2_full_match (uncompiled-easy API)
    cre2_string_t input;
    input.data = text;
    input.length = textlen;
    cre2_string_t out;
    out.data = nullptr;
    out.length = 0;
    int full_match_result = cre2_full_match(pattern, &input, &out, 1);

    // Cleanup compiled regexp and options
    if (re) cre2_delete(re);
    cre2_opt_delete(opt);

    // Consider the sequence successful if either matching call reported success (non-zero)
    if (match_result != 0 || full_match_result != 0) return 66;
    return 1;
}