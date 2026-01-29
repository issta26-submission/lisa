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
//<ID> 192
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // Create options for set/regex creation
    cre2_options_t *opt = cre2_opt_new();
    if (!opt) {
        return 66; // still return success per spec, but nothing to clean up
    }

    // Create a set with default anchor (cast 0 to cre2_anchor_t)
    cre2_set *set = cre2_set_new(opt, (cre2_anchor_t)0);
    if (!set) {
        cre2_opt_delete(opt);
        return 66;
    }

    // Add a couple of simple patterns to the set
    const char patt1[] = "^(hello)$";
    const char patt2[] = "world";
    // cre2_set_add_simple returns int (0 on success in typical C APIs) - ignore exact result but call it
    (void)cre2_set_add_simple(set, patt1);
    (void)cre2_set_add_simple(set, patt2);

    // Compile the set before matching (best practice)
    (void)cre2_set_compile(set);

    // Use cre2_full_match with a pattern and a single input string
    const char text_literal[] = "hello";
    cre2_string_t text = { text_literal, (int)(sizeof(text_literal) - 1) };

    // Prepare an array for captures (one slot)
    cre2_string_t captures[1];
    captures[0].data = nullptr;
    captures[0].length = 0;

    // Perform full match using patt1 (should match "hello")
    (void)cre2_full_match(patt1, &text, captures, 1);

    // Also demonstrate cre2_new to compile a single regex object, then delete it
    cre2_regexp_t *re = cre2_new(patt1, (int)(sizeof(patt1) - 1), opt);
    if (re) {
        cre2_delete(re);
    }

    // Use cre2_set_match to find which of the set patterns match the text
    int match_indices[4] = { -1, -1, -1, -1 };
    size_t nfound = cre2_set_match(set, text_literal, (size_t)(sizeof(text_literal) - 1), match_indices, 4);
    (void)nfound; // result used only to exercise the API

    // Clean up
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    return 66;
}