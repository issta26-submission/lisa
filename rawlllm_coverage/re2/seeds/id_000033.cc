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
//<ID> 33
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // Create options and set one_line flag
    cre2_options_t *opt = cre2_opt_new();
    if (!opt) return 1;
    cre2_opt_set_one_line(opt, 1);

    // Pattern with two capturing groups: (a)(b)
    const char pattern[] = "(a)(b)";
    // compute pattern length without relying on external headers
    int pattern_len = 0;
    while (pattern[pattern_len] != '\0') ++pattern_len;

    // Create a compiled regexp using the options
    cre2_regexp_t *re = cre2_new(pattern, pattern_len, opt);
    if (!re) {
        return 2;
    }

    // Query number of capturing groups
    int num_groups = cre2_num_capturing_groups(re);
    // Expect 2 capturing groups for "(a)(b)"
    if (num_groups != 2) {
        return 3;
    }

    // Prepare text to perform replace on
    const char text[] = "ab";
    int text_len = 0;
    while (text[text_len] != '\0') ++text_len;
    cre2_string_t text_and_target;
    text_and_target.data = text;
    text_and_target.length = text_len;

    // Prepare rewrite string (replacement). Keep it simple.
    const char rewrite_s[] = "X";
    int rewrite_len = 0;
    while (rewrite_s[rewrite_len] != '\0') ++rewrite_len;
    cre2_string_t rewrite;
    rewrite.data = rewrite_s;
    rewrite.length = rewrite_len;

    // Perform a single replace using the pattern (uses pattern string, not the compiled re)
    int replaced = cre2_replace(pattern, &text_and_target, &rewrite);
    if (replaced < 0) {
        return 4;
    }

    // If we reach here, the sequence of API calls succeeded
    return 66;
}