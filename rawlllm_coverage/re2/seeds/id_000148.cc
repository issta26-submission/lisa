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
//<ID> 148
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // Create options
    cre2_options_t *opt = cre2_opt_new();
    if (!opt) {
        return 1;
    }

    // Compile a simple regex (use a literal so we can compute length without strlen)
    const char pat_literal[] = "(?P<num>[0-9]+)";
    int pat_len = (int)(sizeof(pat_literal) - 1);
    cre2_regexp_t *re = cre2_new(pat_literal, pat_len, opt);
    if (!re) {
        cre2_opt_delete(opt);
        return 2;
    }

    // Prepare input text as a cre2_string_t
    const char txt_literal[] = "123abc";
    cre2_string_t text = { txt_literal, (int)(sizeof(txt_literal) - 1) };

    // Prepare match array for up to 2 matches (whole match + one subgroup)
    cre2_string_t matches[2];
    matches[0].data = nullptr; matches[0].length = 0;
    matches[1].data = nullptr; matches[1].length = 0;

    // Try to find and consume with the compiled regexp
    int found = cre2_find_and_consume_re(re, &text, matches, 2);
    (void)found; // we don't require a specific outcome here; just exercise the API

    // Retrieve an error argument object (if any). This is safe to call even if there's no error.
    cre2_string_t err_arg;
    err_arg.data = nullptr;
    err_arg.length = 0;
    cre2_error_arg(re, &err_arg);
    // err_arg may now point to additional diagnostic info; we don't need to inspect it for this test.

    // Create a set and attempt to add an invalid pattern to exercise cre2_set_add's error reporting.
    cre2_set *set = cre2_set_new(opt, (cre2_anchor_t)0);
    if (set) {
        const char bad_pat[] = "(abc"; // intentionally malformed
        char errbuf[128] = {0};
        int add_res = cre2_set_add(set, bad_pat, (size_t)(sizeof(bad_pat) - 1), errbuf, sizeof(errbuf));
        (void)add_res;
        // errbuf may contain an error message for the bad pattern.
        cre2_set_delete(set);
    }

    // Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // Indicate success per instructions
    return 66;
}