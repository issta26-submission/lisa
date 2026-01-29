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
//<ID> 211
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
    cre2_opt_set_log_errors(opt, 1);
    cre2_opt_set_max_mem(opt, (int64_t)16 * 1024 * 1024); // 16 MiB

    // Prepare text and rewrite strings
    char text_buf[] = "abc one abc two";
    cre2_string_t text_and_target = { text_buf, (int)(sizeof(text_buf) - 1) };

    char rewrite_buf[] = "X";
    cre2_string_t rewrite = { rewrite_buf, (int)(sizeof(rewrite_buf) - 1) };

    // Use the non-compiled replace API
    int rc1 = cre2_replace("abc", &text_and_target, &rewrite);

    // Compile a regexp with the same options and use the compiled replace API
    cre2_regexp_t *re = cre2_new("abc", 3, opt);
    int rc2 = -1;
    if (re) {
        rc2 = cre2_replace_re(re, &text_and_target, &rewrite);
        cre2_delete(re);
    }

    // Clean up options
    cre2_opt_delete(opt);

    // Treat negative return values as failure; otherwise success
    if (rc1 < 0 || rc2 < 0) return 2;
    return 66;
}