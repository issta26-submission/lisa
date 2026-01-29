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
//<ID> 233
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // Create options for compiling/using regexps and sets.
    cre2_options_t *opt = cre2_opt_new();

    // Option pointer may be NULL in unusual circumstances; handle gracefully.
    cre2_set *set = nullptr;
    if (opt) {
        // Query the maximum memory option (exercise cre2_opt_max_mem).
        int64_t maxmem = cre2_opt_max_mem(opt);
        (void)maxmem; // silence unused-variable warnings

        // Create a set using the options (exercise cre2_set_new).
        set = cre2_set_new(opt, CRE2_UNANCHORED);
    }

    // Create a compiled regexp using cre2_new.
    const char *pattern = "abc";
    int pattern_len = 3; // length of "abc"
    cre2_regexp_t *re = cre2_new(pattern, pattern_len, opt);

    // Prepare an input string and match storage for cre2_full_match.
    cre2_string_t text;
    text.data = "abc";
    text.length = 3;

    // Array to receive submatch strings (size 2: whole match + maybe one subgroup).
    cre2_string_t matches[2];
    matches[0].data = nullptr;
    matches[0].length = 0;
    matches[1].data = nullptr;
    matches[1].length = 0;

    // Perform a full match using the pattern string API (exercise cre2_full_match).
    // The function returns nonzero on success; we ignore the exact result here,
    // but calling it exercises the API.
    int full_match_result = cre2_full_match(pattern, &text, matches, 2);
    (void)full_match_result;

    // Clean up created objects if they were created.
    if (re) {
        cre2_delete(re);
        re = nullptr;
    }
    if (set) {
        cre2_set_delete(set);
        set = nullptr;
    }
    if (opt) {
        // Delete the options (exercise cre2_opt_delete).
        cre2_opt_delete(opt);
        opt = nullptr;
    }

    // On success, return 66 as requested.
    return 66;
}