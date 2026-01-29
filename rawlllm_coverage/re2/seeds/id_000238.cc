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
//<ID> 238
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    cre2_options_t *opt = cre2_opt_new();
    if (!opt) return 1;

    // Enable POSIX syntax on the options.
    cre2_opt_set_posix_syntax(opt, 1);

    // Query the case sensitivity flag (getter).
    int case_sensitive = cre2_opt_case_sensitive(opt);
    (void)case_sensitive; // silence unused-variable warning if needed

    // Prepare a pattern and create a compiled regexp with the options.
    const char pattern[] = "(ab)c";
    int pattern_len = (int)(sizeof(pattern) - 1);
    cre2_regexp_t *re = cre2_new(pattern, pattern_len, opt);
    if (!re) {
        cre2_opt_delete(opt);
        return 2;
    }

    // Quote the pattern (meta-quoting) into a buffer.
    cre2_string_t original = { pattern, pattern_len };
    char quoted_buf[128] = {0};
    cre2_string_t quoted = { quoted_buf, 0 };
    // Call quote meta (output is placed in quoted_buf / quoted).
    cre2_quote_meta(&quoted, &original);

    // Prepare an input text and a match container for full-match.
    const char text[] = "abc";
    cre2_string_t text_s = { text, (int)(sizeof(text) - 1) };

    // Provide buffers for capture results (at least one).
    char match0_buf[64] = {0};
    cre2_string_t matches[2];
    matches[0].data = match0_buf;
    matches[0].length = 0;
    matches[1].data = nullptr;
    matches[1].length = 0;

    // Attempt a full match using the compiled regexp.
    int full_matched = cre2_full_match_re(re, &text_s, matches, 1);
    (void)full_matched; // keep result (no further use here)

    // Prepare text_for_replace and rewrite string, then call replace.
    // Note: we set the initial length explicitly to the current content length.
    char text_and_target_buf[128] = "abc";
    cre2_string_t text_and_target = { text_and_target_buf, 3 }; // "abc" length = 3
    cre2_string_t rewrite = { "X", 1 }; // replace with "X"

    int replace_rv = cre2_replace_re(re, &text_and_target, &rewrite);
    (void)replace_rv; // accept return value but not required here

    // Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    return 66;
}