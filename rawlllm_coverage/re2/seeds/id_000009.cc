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
//<ID> 9
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // Create default options
    cre2_options_t *opt = cre2_opt_new();
    if (opt) {
        // set a couple of options to exercise the API (safe defaults)
        cre2_opt_set_case_sensitive(opt, 1);
        cre2_opt_set_perl_classes(opt, 1);
    }

    // Create a set (anchor value zero casted to cre2_anchor_t)
    cre2_set *set = nullptr;
    // cre2_anchor_t is an enum/type in the cre2 headers; cast 0 to it.
    set = cre2_set_new(opt, (cre2_anchor_t)0);

    // Try compiling the set (if allocation succeeded)
    int set_compile_result = 0;
    if (set) {
        set_compile_result = cre2_set_compile(set);
        // We don't require any particular return value here; just exercise the call.
        (void)set_compile_result;
    }

    // Pattern and input to test full-match API
    const char *pattern = "^(hello)(world)$";
    // compute pattern length manually to avoid requiring headers
    int pattern_len = 0;
    while (pattern[pattern_len] != '\0') ++pattern_len;

    // Create a compiled regexp using the same options
    cre2_regexp_t *re = cre2_new(pattern, pattern_len, opt);

    // Query number of capturing groups (if compilation succeeded)
    int num_groups = 0;
    if (re) {
        num_groups = cre2_num_capturing_groups(re);
        if (num_groups < 0) num_groups = 0;
    }

    // Prepare the input text and an array to receive matches.
    // full match typically needs 1 + num_groups slots
    int nmatch = 1 + num_groups;
    if (nmatch < 1) nmatch = 1;
    cre2_string_t text;
    text.data = "helloworld";
    text.length = 10;

    // Allocate match storage and initialize
    cre2_string_t *matches = nullptr;
    matches = new cre2_string_t[nmatch];
    for (int i = 0; i < nmatch; ++i) {
        matches[i].data = nullptr;
        matches[i].length = 0;
    }

    // Use the pattern-based full match API (not the compiled-re variant)
    // Signature: int cre2_full_match(const char *, const cre2_string_t *, cre2_string_t *, int)
    int full_match_res = cre2_full_match(pattern, &text, matches, nmatch);
    (void)full_match_res; // result is used only to exercise the API

    // Cleanup
    if (re) cre2_delete(re);
    if (set) cre2_set_delete(set);
    if (opt) cre2_opt_delete(opt);
    delete [] matches;

    return 66;
}