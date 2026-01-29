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
//<ID> 271
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

    // Enable logging of errors, set longest-match behavior, and set case sensitivity off
    cre2_opt_set_log_errors(opt, 1);
    cre2_opt_set_longest_match(opt, 1);
    cre2_opt_set_case_sensitive(opt, 0);

    // Query word-boundary option (just to exercise the getter)
    int word_boundary = cre2_opt_word_boundary(opt);
    (void)word_boundary; // silence unused variable warning if not otherwise used

    // Prepare a simple pattern with two capturing groups and a corresponding rewrite string
    const char *pattern = "^(a)(b)";
    int pattern_len = (int)(sizeof("^(a)(b)") - 1);

    cre2_regexp_t *re = cre2_new(pattern, pattern_len, opt);
    if (!re) {
        cre2_opt_delete(opt);
        return 1;
    }

    // A rewrite that swaps the two capture groups: \2\1
    cre2_string_t rewrite = { "\\2\\1", (int)(sizeof("\\2\\1") - 1) };
    cre2_string_t errmsg = { nullptr, 0 };

    int ok = cre2_check_rewrite_string(re, &rewrite, &errmsg);

    // Clean up
    cre2_delete(re);
    cre2_opt_delete(opt);

    if (!ok) return 1;
    return 66;
}