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
//<ID> 17
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // Create options for regex/set creation
    cre2_options_t *opt = cre2_opt_new();
    if (!opt) {
        return 1; // failed to create options
    }

    // Create a pattern set using the options
    cre2_set *set = cre2_set_new(opt, (cre2_anchor_t)0);
    if (!set) {
        cre2_opt_delete(opt);
        return 2; // failed to create set
    }

    // Add a simple pattern to the set (provide an error buffer)
    const char *pattern_to_add = "hello";
    char errbuf[128] = {0};
    int add_res = cre2_set_add(set, pattern_to_add, (size_t)5, errbuf, sizeof(errbuf));
    (void)add_res; // tolerate failures in this demo, but keep the call

    // Compile the set
    int compile_res = cre2_set_compile(set);
    (void)compile_res;

    // Create a simple regexp (using the same options)
    cre2_regexp_t *re = cre2_new("foo", 3, opt);

    // Prepare input and rewrite strings for cre2_replace
    cre2_string_t text_and_target;
    text_and_target.data = "abcfoo";
    text_and_target.length = 6;

    cre2_string_t rewrite;
    rewrite.data = "X";
    rewrite.length = 1;

    // Use the cre2_replace API (pattern-based replace)
    int replace_res = cre2_replace("foo", &text_and_target, &rewrite);
    (void)replace_res;

    // Clean up created regexp if any
    if (re) {
        cre2_delete(re);
    }

    // Clean up set and options
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    return 66;
}