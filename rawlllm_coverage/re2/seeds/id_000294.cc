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
//<ID> 294
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
        return 1;
    }

    // Compile a regexp with the options
    const char *pattern = "a.*b";
    int pattern_len = (int)(sizeof("a.*b") - 1); // 4
    cre2_regexp_t *re = cre2_new(pattern, pattern_len, opt);
    if (!re) {
        cre2_opt_delete(opt);
        return 2;
    }

    // Create a set using the same options (anchor value 0 used as default)
    cre2_set *set = cre2_set_new(opt, (cre2_anchor_t)0);
    if (!set) {
        cre2_delete(re);
        cre2_opt_delete(opt);
        return 3;
    }

    // Add a simple pattern to the set
    int add_res = cre2_set_add_simple(set, "a.*b");
    if (add_res != 0) {
        // cleanup and report failure
        cre2_set_delete(set);
        cre2_delete(re);
        cre2_opt_delete(opt);
        return 4;
    }

    // Cleanup
    cre2_set_delete(set);
    cre2_delete(re);
    cre2_opt_delete(opt);

    return 66;
}