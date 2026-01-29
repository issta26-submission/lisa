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
//<ID> 236
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // Create options and set encoding to UTF-8
    cre2_options_t *opt = cre2_opt_new();
    if (!opt) return 1;
    cre2_opt_set_encoding(opt, CRE2_UTF8);

    // Create a set, add a simple pattern, compile and delete the set
    cre2_set *set = cre2_set_new(opt, (cre2_anchor_t)0);
    if (set) {
        cre2_set_add_simple(set, "test");
        cre2_set_compile(set);
        cre2_set_delete(set); // required API usage
        set = nullptr;
    }

    // Create a regexp from a simple pattern
    cre2_regexp_t *re = cre2_new("test", 4, opt);

    // Prepare strings for consume
    cre2_string_t text = { "testing", 7 };
    cre2_string_t match = { nullptr, 0 };

    // Use cre2_consume (required API usage)
    (void)cre2_consume("test", &text, &match, 1);

    // Retrieve error argument (if any) from the regexp (required API usage)
    cre2_string_t errmsg_arg = { nullptr, 0 };
    if (re) {
        cre2_error_arg(re, &errmsg_arg);
        cre2_delete(re);
    }

    // Clean up options
    cre2_opt_delete(opt);

    return 66;
}