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
//<ID> 198
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // Create options and configure them
    cre2_options_t *opt = cre2_opt_new();
    if (!opt) return 1;

    // Use cre2_opt_set_literal (required). Set to 0 to allow regex features.
    cre2_opt_set_literal(opt, 0);

    // Query one_line option (required usage of cre2_opt_one_line)
    int one_line_flag = cre2_opt_one_line(opt);
    (void)one_line_flag; // silence unused variable if not used further

    // Prepare a regex pattern and compile it with cre2_new
    const char pattern[] = "foo(\\d+)";
    int pattern_len = (int)(sizeof(pattern) - 1);
    cre2_regexp_t *re = cre2_new(pattern, pattern_len, opt);
    if (!re) {
        cre2_opt_delete(opt);
        return 1;
    }

    // Prepare text to match against
    const char text[] = "begin foo123 end";
    int textlen = (int)(sizeof(text) - 1);

    // Determine number of capturing groups and allocate match array
    int num_groups = cre2_num_capturing_groups(re);
    int nmatch = num_groups + 1; // whole match + capturing groups
    if (nmatch < 1) nmatch = 1;

    cre2_string_t *matches = new cre2_string_t[nmatch];
    for (int i = 0; i < nmatch; ++i) {
        matches[i].data = nullptr;
        matches[i].length = 0;
    }

    // Perform a match (required usage of cre2_match)
    int matched = cre2_match(re, text, textlen, 0, textlen, CRE2_UNANCHORED, matches, nmatch);
    if (!matched) {
        // Clean up
        delete [] matches;
        cre2_delete(re);
        cre2_opt_delete(opt);
        return 1;
    }

    // Create a set, add some patterns, and compile it (required usage of cre2_set_compile)
    cre2_set *set = cre2_set_new(opt, CRE2_UNANCHORED);
    if (!set) {
        delete [] matches;
        cre2_delete(re);
        cre2_opt_delete(opt);
        return 1;
    }

    // Add a couple of simple patterns to the set
    cre2_set_add_simple(set, "alpha");
    cre2_set_add_simple(set, "foo(\\d+)");

    // Compile the set
    int compile_ok = cre2_set_compile(set);
    if (!compile_ok) {
        cre2_set_delete(set);
        delete [] matches;
        cre2_delete(re);
        cre2_opt_delete(opt);
        return 1;
    }

    // Prepare for a replacement operation (required usage of cre2_replace)
    // text_and_target holds the text to operate on. Use a literal buffer.
    const char txt_buf[] = "foo123 and foo456";
    cre2_string_t text_and_target;
    text_and_target.data = txt_buf;
    text_and_target.length = (int)(sizeof(txt_buf) - 1);

    // Rewrite string: replace with "NUM"
    const char rewrite_buf[] = "NUM";
    cre2_string_t rewrite;
    rewrite.data = rewrite_buf;
    rewrite.length = (int)(sizeof(rewrite_buf) - 1);

    // Perform a single replace of the given pattern
    int replace_res = cre2_replace("foo(\\d+)", &text_and_target, &rewrite);
    (void)replace_res; // result is checked only to ensure call succeeds; ignore value specifics

    // Cleanup
    cre2_set_delete(set);
    delete [] matches;
    cre2_delete(re);
    cre2_opt_delete(opt);

    // Success
    return 66;
}