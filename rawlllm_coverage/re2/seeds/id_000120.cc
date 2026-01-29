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
//<ID> 120
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // Create options and set literal mode
    cre2_options_t *opt = cre2_opt_new();
    if (!opt) return 1;
    cre2_opt_set_literal(opt, 1);

    // Create a regex using the options
    const char *pattern = "abc";
    int pattern_len = (int)strlen(pattern);
    cre2_regexp_t *rex = cre2_new(pattern, pattern_len, opt);

    // Prepare outputs for possible match range
    cre2_string_t min_str = { nullptr, 0 };
    cre2_string_t max_str = { nullptr, 0 };
    // Ask for possible match range (maxlen chosen arbitrarily)
    (void)cre2_possible_match_range(rex, &min_str, &max_str, 16);

    // Create a set with the same options (anchor value 0)
    cre2_set *set = cre2_set_new(opt, (cre2_anchor_t)0);

    // Prepare text and match array for cre2_set_match
    const char *text = "abcdef";
    size_t text_len = strlen(text);
    int match_indices[8];
    for (size_t i = 0; i < sizeof(match_indices)/sizeof(match_indices[0]); ++i) match_indices[i] = -1;
    size_t found = cre2_set_match(set, text, text_len, match_indices, (size_t)(sizeof(match_indices)/sizeof(match_indices[0])));
    (void)found; // ignore result for this test sequence

    // Use cre2_consume with a subject string and a match output
    cre2_string_t subject = { "abcdef", (int)strlen("abcdef") };
    cre2_string_t consume_match = { nullptr, 0 };
    // The first parameter is given as a pattern string for the convenience API
    int consume_res = cre2_consume("abc", &subject, &consume_match, 1);
    (void)consume_res; // ignore for this test

    // Cleanup
    if (rex) cre2_delete(rex);
    if (set) cre2_set_delete(set);
    cre2_opt_delete(opt);

    return 66;
}