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
//<ID> 245
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    cre2_options_t *opt = cre2_opt_new();
    if (!opt) {
        return 1; // failed to allocate options
    }

    // Set the literal option and verify via the getter.
    cre2_opt_set_literal(opt, 1);
    int literal_val = cre2_opt_literal(opt);
    if (literal_val != 1) {
        // unexpected, but continue to exercise other APIs; we'll still clean up
    }

    // Create a regular expression using the options.
    const char *pattern = "abc";
    cre2_regexp_t *re = cre2_new(pattern, 3, opt);
    if (!re) {
        cre2_opt_delete(opt);
        return 2; // failed to compile regexp
    }

    // Create a set, add a simple pattern, compile it, then match against text.
    cre2_set *set = cre2_set_new(opt, (cre2_anchor_t)0);
    if (!set) {
        cre2_delete(re);
        cre2_opt_delete(opt);
        return 3; // failed to create set
    }

    // Add a pattern and compile the set (ignore return codes for fuzz-driver style).
    cre2_set_add_simple(set, "abc");
    cre2_set_compile(set);

    const char *text = "zzabczz";
    int match_buf[8];
    for (size_t i = 0; i < sizeof(match_buf)/sizeof(match_buf[0]); ++i) match_buf[i] = -1;

    // Call cre2_set_match to exercise the API.
    size_t nmatches = cre2_set_match(set, text, (size_t)strlen(text), match_buf, (size_t)(sizeof(match_buf)/sizeof(match_buf[0])));

    // Clean up all allocated objects.
    cre2_set_delete(set);
    cre2_delete(re);
    cre2_opt_delete(opt);

    // If we reached here, consider the sequence successful.
    (void)nmatches; // avoid unused-variable warnings in case result isn't inspected
    (void)literal_val;

    return 66;
}