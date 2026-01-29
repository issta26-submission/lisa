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
//<ID> 342
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    cre2_options_t *opt = cre2_opt_new();
    if (opt) {
        cre2_opt_set_case_sensitive(opt, 1);
        cre2_opt_set_perl_classes(opt, 1);
    }

    const char *pattern = "(?P<first>a)(b)"; // one named group "first" and one unnamed
    int pattern_len = (int)strlen(pattern);

    cre2_regexp_t *re = cre2_new(pattern, pattern_len, opt);
    cre2_named_groups_iter_t *iter = NULL;
    if (re) {
        iter = cre2_named_groups_iter_new(re);
    }

    // Prepare input text and cre2_string_t for full match
    const char *text = "ab";
    cre2_string_t input;
    input.data = text;
    input.length = (int)strlen(text);

    // Determine number of capture groups (including the whole match as group 0)
    int ncap = 1;
    if (re) {
        int ng = cre2_num_capturing_groups(re);
        if (ng >= 0) {
            ncap = ng + 1;
        }
    }

    // Allocate arrays for captures and ranges on the stack
    std::vector<cre2_string_t> captures(ncap);
    for (int i = 0; i < ncap; ++i) {
        captures[i].data = NULL;
        captures[i].length = 0;
    }
    std::vector<cre2_range_t> ranges(ncap);
    for (int i = 0; i < ncap; ++i) {
        ranges[i].start = -1;
        ranges[i].past = -1;
    }

    // Try a full match using the compiled regexp
    if (re) {
        int matched = cre2_full_match_re(re, &input, captures.data(), ncap);
        (void)matched; // we don't rely on the result for eventual cleanup

        // Convert the returned capture strings into ranges within the original text
        cre2_strings_to_ranges(text, ranges.data(), captures.data(), ncap);
    }

    // If an iterator exists, iterate once (demonstration) and then delete it
    if (iter) {
        const char *name = NULL;
        int index = -1;
        // call next at least once (if a named group exists this will set name/index)
        cre2_named_groups_iter_next(iter, &name, &index);
        cre2_named_groups_iter_delete(iter);
        iter = NULL;
    }

    // Demonstrate creation and deletion of a set
    cre2_set *set = NULL;
    if (opt) {
        set = cre2_set_new(opt, (cre2_anchor_t)0);
        if (set) {
            cre2_set_add_simple(set, "a");
            cre2_set_compile(set);
        }
    }
    if (set) {
        cre2_set_delete(set);
        set = NULL;
    }

    // Cleanup regexp and options
    if (re) {
        cre2_delete(re);
        re = NULL;
    }
    if (opt) {
        cre2_opt_delete(opt);
        opt = NULL;
    }

    return 66;
}