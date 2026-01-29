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
//<ID> 182
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    /* Query the library interface revision (use result to avoid unused warnings). */
    int rev = cre2_version_interface_revision();
    (void)rev;

    /* Create options for compilation. */
    cre2_options_t *opt = cre2_opt_new();
    if (opt == NULL) {
        return 66;
    }

    /* Create a set using the options. */
    cre2_set *set = cre2_set_new(opt, CRE2_UNANCHORED);

    /* Prepare a pattern and compile it into a regexp using the same options. */
    const char pattern[] = "([A-Za-z]+)([0-9]+)";
    int pattern_len = (int)(sizeof(pattern) - 1);
    cre2_regexp_t *re = cre2_new(pattern, pattern_len, opt);

    /* Prepare input text and a rewrite string for extraction. */
    const char text_buf[] = "abc123";
    cre2_string_t text = { text_buf, (int)(sizeof(text_buf) - 1) };

    const char rewrite_buf[] = "$1-$2";
    cre2_string_t rewrite = { rewrite_buf, (int)(sizeof(rewrite_buf) - 1) };

    /* Target will receive the extracted data; initialize empty. */
    cre2_string_t target = { NULL, 0 };

    /* Attempt extraction using the compiled regexp (if compilation succeeded). */
    if (re != NULL) {
        (void)cre2_extract_re(re, &text, &rewrite, &target);
    }

    /* Clean up allocated objects. */
    if (re != NULL) {
        cre2_delete(re);
    }
    if (set != NULL) {
        cre2_set_delete(set);
    }
    cre2_opt_delete(opt);

    return 66;
}