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
//<ID> 54
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // Create options and set some flags
    cre2_options_t *opt = cre2_opt_new();
    if (!opt) return 1;

    // Use required option-setting APIs
    cre2_opt_set_log_errors(opt, 1);
    cre2_opt_set_word_boundary(opt, 1);

    // Query encoding (required API usage)
    cre2_encoding_t enc = cre2_opt_encoding(opt);
    (void)enc; // silence unused-variable warnings if any

    // Create a set using these options
    cre2_set *set = cre2_set_new(opt, CRE2_UNANCHORED);
    if (!set) {
        cre2_opt_delete(opt);
        return 2;
    }

    // Add a simple pattern to the set (required API usage)
    const char *set_pattern = "foo.*bar";
    int add_ret = cre2_set_add_simple(set, set_pattern);
    (void)add_ret; // accept any non-fatal return for this test

    // Build a regexp directly and test cre2_match (required API usage)
    const char *pattern = "foo";
    cre2_regexp_t *re = cre2_new(pattern, 3, opt); // pattern length = 3
    if (!re) {
        cre2_set_delete(set);
        cre2_opt_delete(opt);
        return 3;
    }

    // Prepare text and match structures
    const char *text = "xxfooxx";
    int textlen = 7; // known length of the literal above

    cre2_string_t match;
    match.data = nullptr;
    match.length = 0;

    // Attempt a match over the whole input
    int mret = cre2_match(re, text, textlen, 0, textlen, CRE2_UNANCHORED, &match, 1);
    (void)mret; // acceptance of either match or no-match; just exercising API

    // Cleanup
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    return 66;
}