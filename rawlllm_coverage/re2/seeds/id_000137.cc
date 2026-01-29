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
//<ID> 137
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

    /* Ensure patterns are case-sensitive and check via the getter. */
    cre2_opt_set_case_sensitive(opt, 1);
    int cs = cre2_opt_case_sensitive(opt);
    (void)cs; /* silence unused-variable warnings if any */

    /* Compile a simple regex "foo" using the options. */
    cre2_regexp_t *rex = cre2_new("foo", 3, opt);
    if (!rex) {
        cre2_opt_delete(opt);
        return 2;
    }

    /* Prepare the subject text in a writable buffer and a rewrite string. */
    char buffer[32] = "foo baz foo";
    cre2_string_t text_and_target = { buffer, 11 }; /* "foo baz foo" length is 11 */
    cre2_string_t rewrite = { "bar", 3 };

    /* Attempt an in-place replace using the compiled regex. */
    int rc = cre2_replace_re(rex, &text_and_target, &rewrite);
    (void)rc; /* we don't require any particular return handling here */

    /* Retrieve any error argument info from the regex (valid even if no error). */
    cre2_string_t errmsg = { NULL, 0 };
    cre2_error_arg(rex, &errmsg);

    /* Clean up resources. */
    cre2_delete(rex);
    cre2_opt_delete(opt);

    return 66;
}