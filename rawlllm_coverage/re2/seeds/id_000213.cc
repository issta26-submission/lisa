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
//<ID> 213
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // Create options for the set and enable never_capture on it
    cre2_options_t *opt1 = cre2_opt_new();
    if (!opt1) return 1;
    cre2_opt_set_never_capture(opt1, 1);

    // Create a set with the options. Use 0 for the anchor value.
    cre2_set *set = cre2_set_new(opt1, (cre2_anchor_t)0);
    if (!set) {
        cre2_opt_delete(opt1);
        return 2;
    }

    // Add simple patterns to the set
    cre2_set_add_simple(set, "test");
    cre2_set_add_simple(set, "abc");

    // Optionally compile the set (ignore return for brevity)
    cre2_set_compile(set);

    // Delete the options used for the set (required by the task)
    cre2_opt_delete(opt1);

    // Create options for the regexp (separate from opt1)
    cre2_options_t *opt2 = cre2_opt_new();
    if (!opt2) {
        cre2_set_delete(set);
        return 3;
    }
    // Ensure capturing behavior is default (clear never_capture)
    cre2_opt_set_never_capture(opt2, 0);

    // Create a regexp with a capturing group
    const char *pattern = "a(b)c";
    cre2_regexp_t *re = cre2_new(pattern, (int)strlen(pattern), opt2);
    if (!re) {
        cre2_opt_delete(opt2);
        cre2_set_delete(set);
        return 4;
    }

    // Prepare text and match structures for find_and_consume_re
    cre2_string_t text;
    text.data = "zabcx";
    text.length = (int)strlen(text.data);

    cre2_string_t match;
    match.data = NULL;
    match.length = 0;

    // Attempt to find and consume using the compiled regexp
    // Pass nmatch = 1 to request one match/capture slot.
    int found = cre2_find_and_consume_re(re, &text, &match, 1);
    (void)found; // we don't assert on the result; just exercise the API

    // Create and then delete a named-groups iterator for the regexp
    cre2_named_groups_iter_t *iter = cre2_named_groups_iter_new(re);
    if (iter) {
        cre2_named_groups_iter_delete(iter);
    }

    // Cleanup remaining resources
    cre2_delete(re);
    cre2_opt_delete(opt2);
    cre2_set_delete(set);

    return 66;
}