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
//<ID> 89
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

    // Set the word boundary option (required usage)
    cre2_opt_set_word_boundary(opt, 1);

    // Query the never_nl option (required usage)
    int never_nl_flag = cre2_opt_never_nl(opt);
    (void)never_nl_flag; // silence unused-variable warnings

    // Create a cre2 set using the options (required usage of cre2_set_new)
    cre2_set *set = cre2_set_new(opt, CRE2_UNANCHORED);
    if (!set) {
        cre2_opt_delete(opt);
        return 2;
    }

    // We are required to call cre2_opt_delete; do so after creating the set.
    cre2_opt_delete(opt);
    opt = NULL;

    // Prepare text and rewrite strings for a global replace call (required usage)
    char text_buf[128] = "hello world, world!";
    int text_len = 0;
    while (text_buf[text_len]) ++text_len;

    cre2_string_t text_and_target;
    text_and_target.data = text_buf;
    text_and_target.length = text_len;

    // Replacement string "planet"
    cre2_string_t rewrite;
    rewrite.data = "planet";
    rewrite.length = 6;

    // Perform a global replace for pattern "world"
    int replace_result = cre2_global_replace("world", &text_and_target, &rewrite);
    (void)replace_result; // we don't assert on the return here; just exercise the API

    // Clean up the set
    cre2_set_delete(set);

    return 66;
}