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
//<ID> 113
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    cre2_options_t *opt = nullptr;
    cre2_set *set = nullptr;
    cre2_regexp_t *re = nullptr;
    cre2_named_groups_iter_t *iter = nullptr;
    int match_buf[8] = {0};

    // Create options
    opt = cre2_opt_new();
    if (!opt) {
        // couldn't create options; still return success code per spec only after cleanup
        return 66;
    }

    // Create a set with the default (unanchoed) anchor
    set = cre2_set_new(opt, CRE2_UNANCHORED);
    if (set) {
        // Add a couple of simple patterns and compile the set
        cre2_set_add_simple(set, "foo");
        cre2_set_add_simple(set, "bar");
        cre2_set_compile(set);

        // Attempt a match against some text
        const char *text = "this is foobar";
        size_t text_len = strlen(text);
        size_t nmatched = cre2_set_match(set, text, text_len, match_buf, sizeof(match_buf) / sizeof(match_buf[0]));
        (void)nmatched; // use result if needed; here we just exercise the API
    }

    // Create a regexp (with a named group) and exercise the named-groups iterator
    const char *pattern = "(?P<name>foo)(bar)";
    int pat_len = (int)strlen(pattern);
    re = cre2_new(pattern, pat_len, opt);
    if (re) {
        iter = cre2_named_groups_iter_new(re);
        if (iter) {
            // We don't need to iterate for this test, just create and delete the iterator to exercise APIs
            cre2_named_groups_iter_delete(iter);
            iter = nullptr;
        }
        cre2_delete(re);
        re = nullptr;
    }

    // Clean up
    if (set) cre2_set_delete(set);
    if (opt) cre2_opt_delete(opt);

    return 66;
}