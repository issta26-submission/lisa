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
//<ID> 96
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // Create options and enable POSIX syntax
    cre2_options_t *opt = cre2_opt_new();
    if (!opt) return 1;
    cre2_opt_set_posix_syntax(opt, 1);

    // Create a set (demonstrates cre2_set_new usage with the options)
    cre2_set *set = cre2_set_new(opt, CRE2_UNANCHORED);
    // set may be NULL on failure; we'll continue but clean up later

    // Create a compiled regexp using the same options
    const char *pattern = "a(bc)";
    int pattern_len = (int)strlen(pattern);
    cre2_regexp_t *re = cre2_new(pattern, pattern_len, opt);
    if (!re) {
        if (set) cre2_set_delete(set);
        cre2_opt_delete(opt);
        return 2;
    }

    // Prepare text and a match container, then run cre2_match
    const char *text = "xxabcxx";
    int text_len = (int)strlen(text);
    cre2_string_t match;
    match.data = nullptr;
    match.length = 0;
    int match_ret = cre2_match(re, text, text_len, 0, text_len, CRE2_UNANCHORED, &match, 1);

    // Use cre2_easy_match with a simple pattern against the same text
    cre2_string_t easy_match;
    easy_match.data = nullptr;
    easy_match.length = 0;
    const char *easy_pattern = "bc";
    int easy_pattern_len = (int)strlen(easy_pattern);
    int easy_ret = cre2_easy_match(easy_pattern, easy_pattern_len, text, text_len, &easy_match, 1);

    // Cleanup
    cre2_delete(re);
    if (set) cre2_set_delete(set);
    cre2_opt_delete(opt);

    // Require both match calls to indicate a match (non-zero). Return 66 on success.
    if (match_ret == 0 || easy_ret == 0) return 3;
    return 66;
}