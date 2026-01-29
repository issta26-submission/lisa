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
//<ID> 215
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // Create options and configure them
    cre2_options_t *opt = cre2_opt_new();
    if (!opt) {
        return 66; // still return success sentinel per instructions if creation fails
    }

    // Set dot to match newline and enable case sensitivity
    cre2_opt_set_dot_nl(opt, 1);
    cre2_opt_set_case_sensitive(opt, 1);

    // Query perl classes flag (getter use as requested)
    int perl_classes_flag = cre2_opt_perl_classes(opt);
    (void)perl_classes_flag; // silence unused-variable warning if any

    // Prepare a pattern and compile it with the options
    const char pattern[] = "a.*b";
    int pattern_len = (int)(sizeof(pattern) - 1);
    cre2_regexp_t *re = cre2_new(pattern, pattern_len, opt);

    if (re) {
        // Prepare cre2_string_t containers for possible-match-range output
        cre2_string_t min_range = { nullptr, 0 };
        cre2_string_t max_range = { nullptr, 0 };

        // Ask for possible match range (maxlen chosen arbitrarily)
        cre2_possible_match_range(re, &min_range, &max_range, 16);

        // Prepare a target text and a rewrite string for global replace
        const char text_buf[] = "a\nb aab ab";
        cre2_string_t text_and_target = { text_buf, (int)(sizeof(text_buf) - 1) };

        const char rewrite_buf[] = "X";
        cre2_string_t rewrite = { rewrite_buf, (int)(sizeof(rewrite_buf) - 1) };

        // Perform a global replace using the compiled regexp
        cre2_global_replace_re(re, &text_and_target, &rewrite);

        // Clean up the compiled regexp
        cre2_delete(re);
    }

    // Clean up options
    cre2_opt_delete(opt);

    return 66;
}