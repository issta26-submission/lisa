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
//<ID> 284
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // Create options and configure them
    cre2_options_t *opt = cre2_opt_new();
    if (opt) {
        // Use the required option-setting APIs
        cre2_opt_set_one_line(opt, 1);      // set one-line mode
        cre2_opt_set_never_nl(opt, 0);     // allow newline by not setting never_nl
    }

    // Create a set with the configured options (use unanchored matching)
    cre2_set *set = nullptr;
    if (opt) {
        set = cre2_set_new(opt, CRE2_UNANCHORED);
    }

    // Add a simple pattern to the set and compile it (ignore return values for brevity)
    if (set) {
        cre2_set_add_simple(set, "(a)(b)");
        cre2_set_compile(set);
    }

    // Create a regexp with the same options
    const char *pattern = "(a)(b)";
    int pattern_len = 6; // length of "(a)(b)"
    cre2_regexp_t *re = nullptr;
    if (opt) {
        re = cre2_new(pattern, pattern_len, opt);
    }

    // Prepare a rewrite string and an errmsg container
    cre2_string_t rewrite;
    rewrite.data = "\\1-\\2"; // use backreferences in rewrite
    rewrite.length = 5;       // length of "\\1-\\2" (5 chars: \,1,-,\,2)

    cre2_string_t errmsg;
    errmsg.data = nullptr;
    errmsg.length = 0;

    // Use the required API to check the rewrite string against the compiled regexp
    if (re) {
        (void)cre2_check_rewrite_string(re, &rewrite, &errmsg);
    }

    // Cleanup allocated resources
    if (re) cre2_delete(re);
    if (set) cre2_set_delete(set);
    if (opt) cre2_opt_delete(opt);

    return 66;
}