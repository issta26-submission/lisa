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
//<ID> 251
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // Create options
    cre2_options_t *opt = cre2_opt_new();
    if (!opt) {
        return 66;
    }

    // Create a set with default anchoring (cast 0 to cre2_anchor_t)
    cre2_set *set = cre2_set_new(opt, (cre2_anchor_t)0);
    if (!set) {
        cre2_opt_delete(opt);
        return 66;
    }

    // Add a couple of simple patterns to the set using the required API
    int add_r1 = cre2_set_add_simple(set, "abc.*");
    int add_r2 = cre2_set_add_simple(set, "^start");
    (void)add_r1;
    (void)add_r2;

    // Compile the set so it can be matched against text
    cre2_set_compile(set);

    // Prepare some text and an array to receive match indices
    const char *text = "zzabc123zz";
    size_t text_len = (sizeof("zzabc123zz") - 1);
    int match_indices[8] = {0};
    size_t num_matches = cre2_set_match(set, text, text_len, match_indices, sizeof(match_indices) / sizeof(match_indices[0]));
    (void)num_matches; // use result optionally

    // Create a single regexp to test possible_match_range
    const char *pattern = "a.*b";
    int pattern_len = (int)(sizeof("a.*b") - 1);
    cre2_regexp_t *rex = cre2_new(pattern, pattern_len, opt);
    if (rex) {
        cre2_string_t min_range = { nullptr, 0 };
        cre2_string_t max_range = { nullptr, 0 };
        // Call the required API to compute possible match range
        int pmr_res = cre2_possible_match_range(rex, &min_range, &max_range, 10);
        (void)pmr_res;
        // Note: min_range and max_range may point into internal memory managed by the library;
        // we do not free them here.

        // Clean up the regexp
        cre2_delete(rex);
    }

    // Clean up set and options
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    return 66;
}