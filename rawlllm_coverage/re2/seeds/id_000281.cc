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
//<ID> 281
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    cre2_options_t *opt = cre2_opt_new();
    if (!opt) {
        return 1;
    }

    /* Enable logging of errors and verify the flag via the getter. */
    cre2_opt_set_log_errors(opt, 1);
    int log_errors_flag = cre2_opt_log_errors(opt);
    (void)log_errors_flag; /* silence unused variable warning if any */

    /* Enable perl character classes and verify via the getter. */
    cre2_opt_set_perl_classes(opt, 1);
    int perl_classes_flag = cre2_opt_perl_classes(opt);
    (void)perl_classes_flag;

    /* Compile a simple regexp using the options. */
    const char pattern[] = "(\\w+)";
    cre2_regexp_t *rex = cre2_new(pattern, (int)(sizeof(pattern) - 1), opt);
    if (!rex) {
        cre2_opt_delete(opt);
        return 2;
    }

    /* Prepare a rewrite string to be checked and used for global replacement. */
    const char rewrite_s[] = "X-$1";
    cre2_string_t rewrite = { rewrite_s, (int)(sizeof(rewrite_s) - 1) };

    /* Prepare an errmsg buffer for cre2_check_rewrite_string to populate on failure. */
    char errmsg_buf[256] = {0};
    cre2_string_t errmsg = { errmsg_buf, 0 };

    /* Check the rewrite string for validity against the compiled regexp. */
    int check_rc = cre2_check_rewrite_string(rex, &rewrite, &errmsg);
    (void)check_rc; /* we proceed regardless, but keep the call exercised */

    /* Prepare the input text (and target buffer). Use a mutable char array. */
    char textbuf[] = "hello 123 world";
    cre2_string_t text_and_target = { textbuf, (int)(sizeof(textbuf) - 1) };

    /* Perform a global replace using the compiled regexp and the rewrite string. */
    int replace_rc = cre2_global_replace_re(rex, &text_and_target, &rewrite);
    (void)replace_rc; /* result is not critical for this test sequence */

    /* Cleanup resources. */
    cre2_delete(rex);
    cre2_opt_delete(opt);

    return 66;
}