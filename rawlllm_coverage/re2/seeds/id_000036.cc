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
//<ID> 36
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // Create options
    cre2_options_t *opt = cre2_opt_new();
    if (!opt) return 1;

    // Create a set (use anchor 0)
    cre2_set *set = cre2_set_new(opt, (cre2_anchor_t)0);
    if (!set) {
        cre2_opt_delete(opt);
        return 2;
    }

    // Prepare a simple pattern and compile a regexp with the options
    const char *pattern = "([0-9]+)";
    int patlen = (int)strlen(pattern);
    cre2_regexp_t *rex = cre2_new(pattern, patlen, opt);
    if (!rex) {
        // cleanup
        cre2_set_delete(set);
        cre2_opt_delete(opt);
        return 3;
    }

    // Create an iterator for named groups (pattern has none, but call is valid)
    cre2_named_groups_iter_t *iter = cre2_named_groups_iter_new(rex);
    if (iter) {
        const char *name = nullptr;
        int index = 0;
        // iterate (will simply not iterate for patterns without named groups)
        while (cre2_named_groups_iter_next(iter, &name, &index)) {
            // In a test driver we could examine name/index; here just iterate safely.
            (void)name;
            (void)index;
        }
        // delete iterator
        cre2_named_groups_iter_delete(iter);
    }

    // Prepare input text and rewrite string for replace operation.
    // Use strdup so the API can safely reassign/allocate if it does so.
    char *input = nullptr;
    char *rewrite_buf = nullptr;
    {
        const char *s = "abc123def456";
        input = (char*)malloc(strlen(s) + 1);
        if (input) strcpy(input, s);
        const char *r = "$1X";
        rewrite_buf = (char*)malloc(strlen(r) + 1);
        if (rewrite_buf) strcpy(rewrite_buf, r);
    }

    cre2_string_t text_and_target = { input, input ? (int)strlen(input) : 0 };
    cre2_string_t rewrite = { rewrite_buf, rewrite_buf ? (int)strlen(rewrite_buf) : 0 };

    // Call replace with the compiled regexp
    int rc = cre2_replace_re(rex, &text_and_target, &rewrite);
    // rc might indicate success/failure; we don't enforce a specific value here.
    (void)rc;

    // Note: We avoid freeing text_and_target.data or rewrite.data here because
    // the cre2 API may have reallocated or taken ownership. To be conservative
    // in this small test driver we simply allow the OS to reclaim process memory.

    // Cleanup compiled regexp, set, and options
    cre2_delete(rex);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // Return 66 to indicate success as requested
    return 66;
}