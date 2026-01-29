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
//<ID> 280
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // Create options object
    cre2_options_t *opt = cre2_opt_new();
    if (!opt) return 1;

    // Set case sensitivity and encoding as required
    cre2_opt_set_case_sensitive(opt, 1);
    cre2_opt_set_encoding(opt, CRE2_UTF8);

    // Query perl classes flag (use the required API)
    int perl_classes_flag = cre2_opt_perl_classes(opt);
    // Optionally, if perl classes are disabled, leave as-is; just demonstrate usage.
    (void)perl_classes_flag;

    // Prepare a simple pattern with two capturing groups: "(a)(b)" (length 6)
    const char pattern[] = "(a)(b)";
    int pattern_len = 6;

    // Compile the regexp with the options
    cre2_regexp_t *re = cre2_new(pattern, pattern_len, opt);
    if (!re) {
        cre2_opt_delete(opt);
        return 2;
    }

    // Query number of capturing groups (required API)
    int num_groups = cre2_num_capturing_groups(re);
    // Expect 2 for the pattern "(a)(b)"; treat mismatch as non-fatal but check correctness
    if (num_groups != 2) {
        // Clean up and report failure
        cre2_delete(re);
        cre2_opt_delete(opt);
        return 3;
    }

    // Clean up
    cre2_delete(re);
    cre2_opt_delete(opt);

    return 66;
}