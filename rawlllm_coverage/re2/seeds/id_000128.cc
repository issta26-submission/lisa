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
//<ID> 128
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

    // Inspect and toggle case sensitivity (use required API cre2_opt_case_sensitive)
    int cs = cre2_opt_case_sensitive(opt);
    cre2_opt_set_case_sensitive(opt, cs ? 0 : 1);

    // Read max memory (required use of cre2_opt_max_mem)
    int64_t maxmem = cre2_opt_max_mem(opt);
    (void)maxmem; // silence unused-variable warnings in environments that check

    // Prepare a pattern and construct a regexp using the options (required use of cre2_new)
    const char *pattern = "^(a|b)+$";
    int pattern_len = 8; // length of "^(a|b)+$"
    cre2_regexp_t *re = cre2_new(pattern, pattern_len, opt);

    // Create a set with the same options (required use of cre2_set_new)
    cre2_set *set = cre2_set_new(opt, CRE2_UNANCHORED);
    if (!set) {
        if (re) cre2_delete(re);
        cre2_opt_delete(opt);
        return 2;
    }

    // Add the same pattern to the set and compile it
    cre2_set_add_simple(set, pattern);
    cre2_set_compile(set);

    // Try matching the set against some text
    const char *text = "abab";
    int match_indexes[10] = {0};
    size_t nmatched = cre2_set_match(set, text, 4 /* strlen("abab") */, match_indexes, 10);
    (void)nmatched; // accept result, but not required for this test

    // Cleanup
    if (re) cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    return 66;
}