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
//<ID> 152
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    const char *pattern = "a(b)c";
    int pattern_len = (int)strlen(pattern);

    /* Create options and set case sensitivity */
    cre2_options_t *opt = cre2_opt_new();
    if (!opt) return 1;
    cre2_opt_set_case_sensitive(opt, 1);

    /* Compile a regexp using the options */
    cre2_regexp_t *re = cre2_new(pattern, pattern_len, opt);
    if (!re) {
        cre2_opt_delete(opt);
        return 2;
    }

    /* Prepare input text and match storage */
    const char *input = "xxabczz";
    cre2_string_t text = { input, (int)strlen(input) };
    /* Reserve space for whole match + one capturing group */
    cre2_string_t matches[2];
    for (int i = 0; i < 2; ++i) {
        matches[i].data = NULL;
        matches[i].length = 0;
    }

    /* Use the pattern-based find-and-consume (non-_re) API */
    int nmatch = 2; /* whole match + 1 capture */
    int found = cre2_find_and_consume(pattern, &text, matches, nmatch);
    (void)found; /* result used only for exercising the API in this test */

    /* Clean up compiled regexp and options */
    cre2_delete(re);
    cre2_opt_delete(opt);

    return 66;
}