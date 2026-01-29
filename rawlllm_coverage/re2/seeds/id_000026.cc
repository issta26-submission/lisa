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
//<ID> 26
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // Create options and set the word-boundary flag
    cre2_options_t *opt = cre2_opt_new();
    if (!opt) return -1;
    cre2_opt_set_word_boundary(opt, 1);

    // Compile a small regexp using the options (will be deleted later)
    const char pat[] = "def";
    cre2_regexp_t *re = cre2_new(pat, (int)(sizeof(pat) - 1), opt);
    if (!re) {
        cre2_opt_delete(opt);
        return -2;
    }

    // Prepare strings for cre2_replace
    cre2_string_t text_and_target;
    const char source[] = "abc def ghi def";
    text_and_target.data = source;
    text_and_target.length = (int)(sizeof(source) - 1);

    cre2_string_t rewrite;
    const char repl[] = "X";
    rewrite.data = repl;
    rewrite.length = (int)(sizeof(repl) - 1);

    // Perform a replace using a pattern that uses word boundaries
    // Note: cre2_replace takes a C string pattern (escaped as needed)
    (void)cre2_replace("\\bdef\\b", &text_and_target, &rewrite);

    // Create a set using the same options and add a simple pattern
    cre2_set *set = cre2_set_new(opt, (cre2_anchor_t)0);
    if (set) {
        cre2_set_add_simple(set, "def");
        cre2_set_compile(set);

        // Match the set against the source string
        int matches[8] = {0};
        size_t nmatched = cre2_set_match(set, source, (size_t)text_and_target.length, matches, 8);
        (void)nmatched; // result used only to exercise the API

        // Clean up the set
        cre2_set_delete(set);
    }

    // Clean up regexp and options
    cre2_delete(re);
    cre2_opt_delete(opt);

    return 66;
}