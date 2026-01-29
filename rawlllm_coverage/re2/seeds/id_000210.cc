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
//<ID> 210
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // Query version string
    const char *ver = cre2_version_string();
    if (!ver) return 1;

    // Create options (use default options)
    cre2_options_t *opt = cre2_opt_new();
    if (!opt) return 2;

    // Create a regexp object from a pattern
    const char *pattern = "f(o)o";
    int pattern_len = 5; // "f(o)o" has 5 bytes
    cre2_regexp_t *re = cre2_new(pattern, pattern_len, opt);
    if (!re) {
        cre2_opt_delete(opt);
        return 3;
    }

    // Prepare input for partial match and capture storage
    cre2_string_t input = { "xxfoobar", 8 }; // text to search in
    cre2_string_t matches[3];
    for (int i = 0; i < 3; ++i) {
        matches[i].data = nullptr;
        matches[i].length = 0;
    }

    // Attempt a partial match using the pattern string API
    int pm_res = cre2_partial_match(pattern, &input, matches, 3);
    (void)pm_res; // accept whatever the result is for this test

    // Use global replace to replace all "foo" with "bar" in a target string
    cre2_string_t text_and_target = { "foo and foo", 11 };
    cre2_string_t rewrite = { "bar", 3 };
    int gr_res = cre2_global_replace("foo", &text_and_target, &rewrite);
    (void)gr_res;

    // Demonstrate cre2_set_add_simple: create a set, add a simple pattern, then delete it
    cre2_set *set = cre2_set_new(opt, (cre2_anchor_t)0);
    if (set) {
        int add_res = cre2_set_add_simple(set, "hello.*");
        (void)add_res;
        cre2_set_delete(set);
    }

    // Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    return 66;
}