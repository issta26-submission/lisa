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
//<ID> 64
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
        return 66; // still consider as test end; nothing to clean up
    }

    // Set a reasonable memory limit for the options (using setter available)
    cre2_opt_set_max_mem(opt, (int64_t)(4 * 1024 * 1024)); // 4 MB

    // Query the max mem back to exercise cre2_opt_max_mem
    int64_t maxmem = cre2_opt_max_mem(opt);
    (void)maxmem; // silence unused variable warning in contexts where not checked

    // Create a pattern set with the options and an unanchored search
    cre2_set *set = cre2_set_new(opt, CRE2_UNANCHORED);
    if (!set) {
        cre2_opt_delete(opt);
        return 66;
    }

    // Add a simple pattern to the set
    int add_res = cre2_set_add_simple(set, "test");
    (void)add_res;

    // Compile the set (best-effort; ignore the return value for this test)
    cre2_set_compile(set);

    // Create a compiled regexp (reuse the options)
    const char *pat = "test";
    cre2_regexp_t *rex = cre2_new(pat, 4, opt);

    // Prepare text and rewrite strings for global replace
    // Use writable buffer for text in case implementation expects to modify or reference it
    static char text_buf[] = "this is a test; test!";
    cre2_string_t text_and_target;
    text_and_target.data = text_buf;
    text_and_target.length = (int)(sizeof(text_buf) - 1);

    // Replacement string
    static const char replace_buf[] = "X";
    cre2_string_t rewrite;
    rewrite.data = replace_buf;
    rewrite.length = 1;

    // Perform a global replace using the compiled regexp
    if (rex) {
        int rep_res = cre2_global_replace_re(rex, &text_and_target, &rewrite);
        (void)rep_res;
    }

    // Cleanup
    if (rex) cre2_delete(rex);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    return 66;
}