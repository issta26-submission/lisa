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
//<ID> 93
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // Create options and configure them.
    cre2_options_t *opt = cre2_opt_new();
    if (!opt) return 0;

    // Read current POSIX-syntax flag and use it to decide longest-match setting.
    int posix = cre2_opt_posix_syntax(opt);
    cre2_opt_set_longest_match(opt, posix ? 0 : 1);

    // Enable logging of errors.
    cre2_opt_set_log_errors(opt, 1);

    // Create a regexp using the configured options.
    const std::string pattern = "abc";
    cre2_regexp_t *re = cre2_new(pattern.c_str(), static_cast<int>(pattern.size()), opt);
    if (!re) {
        cre2_opt_delete(opt);
        return 0;
    }

    // Prepare the target text and rewrite string for global replace.
    std::string text = "abc 123 abc abc";
    std::string rewrite = "X";

    cre2_string_t text_and_target = { text.c_str(), static_cast<int>(text.size()) };
    cre2_string_t rewrite_str = { rewrite.c_str(), static_cast<int>(rewrite.size()) };

    // Perform a global replace using the compiled regexp.
    int rc = cre2_global_replace_re(re, &text_and_target, &rewrite_str);

    // Clean up.
    cre2_delete(re);
    cre2_opt_delete(opt);

    // Consider the operation successful if the API returned a non-negative value.
    return (rc >= 0) ? 66 : 0;
}