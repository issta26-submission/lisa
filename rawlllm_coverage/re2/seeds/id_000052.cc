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
//<ID> 52
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // Create options and set the "literal" flag (here we set to 0 to allow regular expressions)
    cre2_options_t *opt = cre2_opt_new();
    if (!opt) return 1;
    cre2_opt_set_literal(opt, 0);

    // Create a regexp with the given options
    const char *pattern = "(foo)(bar)";
    int pattern_len = (int)strlen(pattern);
    cre2_regexp_t *rex = cre2_new(pattern, pattern_len, opt);
    if (!rex) {
        cre2_opt_delete(opt);
        return 2;
    }

    // Use cre2_easy_match to attempt a match and collect up to 3 substrings:
    // full match, group1, group2
    const char *text = "prefix foobar suffix";
    int text_len = (int)strlen(text);
    cre2_string_t matches[3];
    for (int i = 0; i < 3; ++i) {
        matches[i].data = NULL;
        matches[i].length = 0;
    }
    int matched = cre2_easy_match(pattern, pattern_len, text, text_len, matches, 3);
    (void)matched; // we don't require a particular value for this test; just exercise the API

    // Prepare buffers and call cre2_possible_match_range to get min/max possible matches
    char minbuf[64];
    char maxbuf[64];
    // initialize buffers to avoid uninitialized memory if cre2 doesn't fill them entirely
    memset(minbuf, 0, sizeof(minbuf));
    memset(maxbuf, 0, sizeof(maxbuf));
    cre2_string_t min_s = { minbuf, 0 };
    cre2_string_t max_s = { maxbuf, 0 };
    int pmr_ret = cre2_possible_match_range(rex, &min_s, &max_s, (int)sizeof(minbuf));
    (void)pmr_ret; // ignore specific return, we only need to call the API

    // Prepare a mutable target buffer for replace operation.
    // Copy the original text into a local buffer (text_and_target) so replace_re can modify it.
    char targetbuf[256];
    memset(targetbuf, 0, sizeof(targetbuf));
    size_t copy_len = (size_t)text_len;
    if (copy_len >= sizeof(targetbuf)) copy_len = sizeof(targetbuf) - 1;
    memcpy(targetbuf, text, copy_len);
    targetbuf[copy_len] = '\0';
    cre2_string_t text_and_target = { targetbuf, (int)copy_len };

    // Prepare a rewrite string that swaps the two captured groups (\2\1).
    // Note: rewrite syntax depends on the library; this is intended to exercise the API.
    const char *rewrite_literal = "\\2\\1";
    cre2_string_t rewrite = { rewrite_literal, (int)strlen(rewrite_literal) };

    // Call cre2_replace_re to perform a single replace with the compiled regexp.
    int replace_ret = cre2_replace_re(rex, &text_and_target, &rewrite);
    (void)replace_ret; // we don't assert a specific result here

    // Clean up
    cre2_delete(rex);
    cre2_opt_delete(opt);

    return 66;
}