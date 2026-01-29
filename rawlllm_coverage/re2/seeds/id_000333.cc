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
//<ID> 333
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // Create options object
    cre2_options_t *opt = cre2_opt_new();
    if (!opt) return 1;

    // Set some option flags (demonstrate setters)
    cre2_opt_set_case_sensitive(opt, 1);
    cre2_opt_set_one_line(opt, 1);

    // Query option values (required API usage)
    int is_case_sensitive = cre2_opt_case_sensitive(opt);
    int is_one_line = cre2_opt_one_line(opt);
    (void)is_case_sensitive;
    (void)is_one_line;

    // Create a set with the options
    cre2_set *set = cre2_set_new(opt, CRE2_UNANCHORED);
    if (!set) {
        cre2_opt_delete(opt);
        return 2;
    }

    // Add a simple pattern to the set and compile it
    int add_res = cre2_set_add_simple(set, "abc");
    (void)add_res;
    int compile_res = cre2_set_compile(set);
    (void)compile_res;

    // Attempt to match the set against some text
    int matches_buf[8] = {0};
    size_t nmatched = cre2_set_match(set, "zzabczz", 7, matches_buf, 8);
    (void)nmatched;

    // Demonstrate cre2_replace usage
    cre2_string_t text_and_target;
    text_and_target.data = "abc xyz abc";
    text_and_target.length = 11;
    cre2_string_t rewrite;
    rewrite.data = "def";
    rewrite.length = 3;
    int replace_res = cre2_replace("abc", &text_and_target, &rewrite);
    (void)replace_res;

    // Create a compiled regexp and run cre2_match
    cre2_regexp_t *re = cre2_new("abc", 3, opt);
    if (re) {
        cre2_string_t match_arr[2];
        // Initialize match slots (library may fill these)
        match_arr[0].data = nullptr; match_arr[0].length = 0;
        match_arr[1].data = nullptr; match_arr[1].length = 0;

        int mres = cre2_match(re, "abcde", 5, 0, 5, CRE2_UNANCHORED, match_arr, 2);
        (void)mres;
        // Clean up compiled regexp
        cre2_delete(re);
    }

    // Clean up set and options
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    return 66;
}