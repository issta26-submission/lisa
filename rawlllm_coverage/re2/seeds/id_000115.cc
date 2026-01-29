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
//<ID> 115
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // Create options and enable case-sensitive matching.
    cre2_options_t *opt = cre2_opt_new();
    if (!opt) return 1;
    cre2_opt_set_case_sensitive(opt, 1);

    // Pattern to compile.
    const char *pattern = "a";
    // compute pattern length manually to avoid relying on headers
    int pat_len = 0;
    while (pattern[pat_len]) ++pat_len;

    // Compile a regexp using the options.
    cre2_regexp_t *re = cre2_new(pattern, pat_len, opt);
    // Even if compilation fails, continue to exercise global replace and cleanup.
    
    // Prepare a mutable text buffer for global_replace.
    const char *initial = "a cat and a dog";
    int text_len = 0;
    while (initial[text_len]) ++text_len;
    char *text_buf = new char[text_len + 1];
    for (int i = 0; i <= text_len; ++i) text_buf[i] = initial[i]; // include null terminator

    cre2_string_t text_and_target = { text_buf, text_len };

    // Rewrite string: replace "a" with "x"
    const char *rew = "x";
    cre2_string_t rewrite = { rew, 1 };

    // Perform a global replace using the pattern form of the API.
    int rc = cre2_global_replace(pattern, &text_and_target, &rewrite);
    (void)rc; // ignore result for this test sequence

    // Clean up: delete compiled regexp (if created) and options, and free buffer.
    if (re) cre2_delete(re);
    cre2_opt_delete(opt);
    delete [] text_buf;

    return 66;
}