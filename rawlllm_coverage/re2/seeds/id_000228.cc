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
//<ID> 228
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // Prepare a simple pattern with one capturing group.
    const char pattern[] = "a(b)c";
    const int pattern_len = (int)(sizeof(pattern) - 1);

    // Create options and set one option as an example.
    cre2_options_t * opt = cre2_opt_new();
    if (!opt) {
        return 1;
    }
    cre2_opt_set_case_sensitive(opt, 1);

    // Compile the pattern into a cre2_regexp_t.
    cre2_regexp_t * re = cre2_new(pattern, pattern_len, opt);
    if (!re) {
        cre2_opt_delete(opt);
        return 2;
    }

    // Query number of capturing groups.
    int num_groups = cre2_num_capturing_groups(re);
    (void)num_groups; // we don't require a specific number; just exercise the API.

    // Prepare a cre2_string_t to receive an error argument (if any).
    cre2_string_t errarg;
    errarg.data = NULL;
    errarg.length = 0;
    cre2_error_arg(re, &errarg);

    // Use cre2_find_and_consume (non-compiled form) to find a match in some text.
    const char source_text[] = "xxa(b)czz";
    cre2_string_t text;
    text.data = source_text;
    text.length = (int)(sizeof(source_text) - 1);

    // Prepare an array to receive up to 2 match strings (full match + one capture).
    cre2_string_t matches[2];
    for (int i = 0; i < 2; ++i) {
        matches[i].data = NULL;
        matches[i].length = 0;
    }
    // nmatch = 2 to request full match + one capturing group.
    int found = cre2_find_and_consume(pattern, &text, matches, 2);
    (void)found; // ignore return value for this test sequence

    // Prepare a mutable text buffer for replace_re to operate on.
    char mutable_text[] = "a(b)c and a(b)c"; // mutable buffer
    cre2_string_t text_and_target;
    text_and_target.data = mutable_text;
    text_and_target.length = (int)(sizeof(mutable_text) - 1);

    // Prepare a rewrite string (replacement).
    const char rewrite_text[] = "X";
    cre2_string_t rewrite;
    rewrite.data = rewrite_text;
    rewrite.length = (int)(sizeof(rewrite_text) - 1);

    // Perform a replace using the compiled regexp.
    int repl_rc = cre2_replace_re(re, &text_and_target, &rewrite);
    (void)repl_rc; // just exercise the call; ignore result for this test

    // Clean up.
    cre2_delete(re);
    cre2_opt_delete(opt);

    return 66;
}