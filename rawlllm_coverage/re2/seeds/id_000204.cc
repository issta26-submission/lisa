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
//<ID> 204
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // Create options and query library interface versions.
    cre2_options_t *opt = cre2_opt_new();
    if (!opt) {
        return 1; // failure to create options
    }
    int ver_cur = cre2_version_interface_current();
    int ver_rev = cre2_version_interface_revision();
    (void)ver_cur;
    (void)ver_rev;

    // Build a regexp using the options.
    const char pattern_lit[] = "([A-Za-z]+)";
    int pattern_len = (int)(sizeof(pattern_lit) - 1);
    cre2_regexp_t *re = cre2_new(pattern_lit, pattern_len, opt);

    // Prepare a cre2_string_t to receive any error argument information.
    cre2_string_t errarg;
    errarg.data = NULL;
    errarg.length = 0;
    if (re) {
        // Extract error argument (if any) from the compiled regexp.
        cre2_error_arg(re, &errarg);
    }

    // Prepare text and rewrite strings for cre2_replace.
    const char text_lit[] = "hello world";
    cre2_string_t text_and_target;
    text_and_target.data = text_lit;
    text_and_target.length = (int)(sizeof(text_lit) - 1);

    const char rewrite_lit[] = "universe";
    cre2_string_t rewrite;
    rewrite.data = rewrite_lit;
    rewrite.length = (int)(sizeof(rewrite_lit) - 1);

    // Perform a single replace operation using a simple pattern.
    int replace_res = cre2_replace("world", &text_and_target, &rewrite);
    (void)replace_res; // ignore result for this test sequence

    // Clean up: delete options and regexp if created.
    cre2_opt_delete(opt);
    if (re) {
        cre2_delete(re);
    }

    return 66;
}