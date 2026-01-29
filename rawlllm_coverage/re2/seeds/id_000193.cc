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
//<ID> 193
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // Create options and configure them
    cre2_options_t *opt = cre2_opt_new();
    // Set dot to match newline and toggle word boundary behavior
    cre2_opt_set_dot_nl(opt, 1);
    cre2_opt_set_word_boundary(opt, 1);

    // Create a set using the options (anchor unanchored)
    cre2_set *set = cre2_set_new(opt, CRE2_UNANCHORED);

    // Create a simple regexp using the same options
    const char pattern[] = "a";
    cre2_regexp_t *rex = cre2_new(pattern, 1, opt);

    // Prepare text and rewrite strings for replace_re
    cre2_string_t text_and_target;
    cre2_string_t rewrite;

    const char text_buf[] = "a";
    const char rewrite_buf[] = "b";

    text_and_target.data = text_buf;
    text_and_target.length = 1;

    rewrite.data = rewrite_buf;
    rewrite.length = 1;

    // Perform a replace using the compiled regexp
    // (ignore the return value for this test sequence)
    if (rex) {
        (void)cre2_replace_re(rex, &text_and_target, &rewrite);
    }

    // Clean up resources
    if (rex) cre2_delete(rex);
    if (set) cre2_set_delete(set);
    cre2_opt_delete(opt);

    return 66;
}