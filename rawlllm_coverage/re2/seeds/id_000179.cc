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
//<ID> 179
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // Create options and configure them.
    cre2_options_t *opt = cre2_opt_new();
    if (!opt) return 1;

    // Set some options (use the setters available)...
    cre2_opt_set_literal(opt, 1);    // treat patterns as literal
    cre2_opt_set_never_nl(opt, 0);   // allow newlines normally

    // ...and read them back using the required getter APIs.
    int literal_flag = cre2_opt_literal(opt);
    int never_nl_flag = cre2_opt_never_nl(opt);
    (void)literal_flag;
    (void)never_nl_flag;

    // Prepare a simple pattern and create a compiled regexp.
    const char *pattern = "abc";
    int pattern_len = 3;
    cre2_regexp_t *re = cre2_new(pattern, pattern_len, opt);

    // Prepare input text and match container for cre2_consume.
    cre2_string_t text;
    text.data = "abcdef";
    text.length = 6;
    cre2_string_t match;
    match.data = NULL;
    match.length = 0;

    // Use cre2_consume (uncompiled-string-based API) against the text.
    // We pass the pattern string as the first argument (per the available signature).
    int consume_res = cre2_consume(pattern, &text, &match, 1);
    (void)consume_res;
    (void)match; // match may have been populated by the call

    // Create a set, add the same pattern, compile it and use cre2_set_match.
    cre2_set *set = cre2_set_new(opt, (cre2_anchor_t)0);
    if (set) {
        // Add a simple pattern and compile the set.
        cre2_set_add_simple(set, pattern);
        cre2_set_compile(set);

        // Prepare an array for matches and run cre2_set_match.
        int matches[10] = {0};
        size_t nm = cre2_set_match(set, text.data, (size_t)text.length, matches, 10);
        (void)nm;
        (void)matches;

        // Clean up the set.
        cre2_set_delete(set);
    }

    // Clean up other allocated objects.
    if (re) cre2_delete(re);
    cre2_opt_delete(opt);

    return 66;
}