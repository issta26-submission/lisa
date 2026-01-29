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
//<ID> 150
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // Create options and set encoding, then query it
    cre2_options_t *opt = cre2_opt_new();
    if (opt) {
        cre2_opt_set_encoding(opt, CRE2_UTF8);
    }
    // Use the required API: cre2_opt_encoding
    cre2_encoding_t enc = CRE2_UNKNOWN;
    if (opt) {
        enc = cre2_opt_encoding(opt);
        (void)enc; // silence unused-variable warnings in some builds
    }

    // Prepare a pattern and compile a regexp using the options
    const char *pattern = "ab(c)"; // pattern length is 5
    int pattern_len = 5;
    cre2_regexp_t *re = cre2_new(pattern, pattern_len, opt);

    // If compilation succeeded, attempt to find and consume from a text
    if (re) {
        // Input text that contains "abc" so the pattern "ab(c)" can match
        const char *text_data = "zzabczz";
        cre2_string_t text = { text_data, 7 };

        // Prepare a match container (initialized empty)
        cre2_string_t match = { NULL, 0 };

        // Use the required API: cre2_find_and_consume_re
        // Last argument '1' gives room for a single match/capture structure as used by the API.
        // We ignore the return value here (fuzz-style usage).
        (void)cre2_find_and_consume_re(re, &text, &match, 1);

        // Delete the compiled regexp as required
        cre2_delete(re);
        re = NULL;
    }

    // Use cre2_replace (non-regexp API) to exercise replacement functionality.
    // text_and_target is both input and (potential) output for cre2_replace.
    cre2_string_t text_and_target = { "one b two b", 12 }; // original text length 12
    cre2_string_t rewrite = { "B", 1 }; // replacement text
    // Use the required API: cre2_replace
    (void)cre2_replace("b", &text_and_target, &rewrite);

    // Clean up options
    if (opt) {
        cre2_opt_delete(opt);
        opt = NULL;
    }

    return 66;
}