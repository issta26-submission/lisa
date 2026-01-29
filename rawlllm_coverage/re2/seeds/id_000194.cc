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
//<ID> 194
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // Create and configure options
    cre2_options_t *opt = cre2_opt_new();
    if (!opt) {
        return 1; // failed to allocate options
    }
    // enable dot matches newline and read it back (use required getter)
    cre2_opt_set_dot_nl(opt, 1);
    int dot_nl_flag = cre2_opt_dot_nl(opt);
    (void)dot_nl_flag; // silence unused variable warning

    // Prepare a pattern and compile a regexp with the options
    char pattern[] = "(\\w+)-(\\d+)";
    cre2_regexp_t *re = cre2_new(pattern, (int)(sizeof(pattern) - 1), opt);

    // Prepare a buffer to be used with cre2_replace
    char textbuf[] = "alpha-1 beta-22 gamma-333";
    cre2_string_t text_and_target = { textbuf, (int)(sizeof(textbuf) - 1) };
    char replbuf[] = "[$1:$2]";
    cre2_string_t rewrite = { replbuf, (int)(sizeof(replbuf) - 1) };

    // Perform a single replace using the pattern (uses cre2_replace)
    // Note: cre2_replace may modify text_and_target.data (in-place or via internals)
    int replace_rc = cre2_replace(pattern, &text_and_target, &rewrite);
    (void)replace_rc;

    // Prepare strings for cre2_extract_re (uses compiled regexp 're')
    char text2[] = "foo alpha-1 bar";
    cre2_string_t text_struct = { text2, (int)(sizeof(text2) - 1) };
    char rewrite2buf[] = "$1";
    cre2_string_t rewrite2 = { rewrite2buf, (int)(sizeof(rewrite2buf) - 1) };
    char target_buf[128] = {0};
    cre2_string_t target = { target_buf, 0 };

    if (re) {
        // Extract using the compiled regexp (uses cre2_extract_re)
        int extract_rc = cre2_extract_re(re, &text_struct, &rewrite2, &target);
        (void)extract_rc;
    }

    // Delete the compiled regexp (uses cre2_delete)
    if (re) {
        cre2_delete(re);
        re = nullptr;
    }

    // Create a set of patterns, add some patterns, compile and match against text
    cre2_set *set = cre2_set_new(opt, (cre2_anchor_t)0);
    if (set) {
        // add simple patterns
        cre2_set_add_simple(set, "alpha-1");
        cre2_set_add_simple(set, "beta-22");
        cre2_set_compile(set);

        // Prepare an array to receive match indexes and call cre2_set_match
        int matches[4] = { -1, -1, -1, -1 };
        size_t matched = cre2_set_match(set, textbuf, (size_t)(sizeof(textbuf) - 1), matches, 4);
        (void)matched;
        (void)matches;

        cre2_set_delete(set);
    }

    // Clean up options
    cre2_opt_delete(opt);

    return 66;
}