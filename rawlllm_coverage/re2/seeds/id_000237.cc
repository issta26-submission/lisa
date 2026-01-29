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
//<ID> 237
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    /* Create options */
    cre2_options_t *opt = cre2_opt_new();
    if (!opt) return 1;

    /* Query a property (use the required API) */
    int case_sensitive = cre2_opt_case_sensitive(opt);
    (void)case_sensitive; /* silence unused-variable warnings */

    /* Prepare a pattern and compute its length without relying on headers */
    const char *pattern = "a(b)c";
    int pattern_len = 0;
    while (pattern[pattern_len] != '\0') ++pattern_len;

    /* Compile a regexp using the options */
    cre2_regexp_t *re = cre2_new(pattern, pattern_len, opt);
    if (!re) {
        cre2_opt_delete(opt);
        return 2;
    }

    /* Query number of capturing groups (use the required API) */
    int num_groups = cre2_num_capturing_groups(re);
    (void)num_groups; /* silence unused-variable warnings */

    /* Create a set and add a simple pattern (use the required API) */
    cre2_set *set = cre2_set_new(opt, (cre2_anchor_t)0);
    if (!set) {
        cre2_delete(re);
        cre2_opt_delete(opt);
        return 3;
    }

    int add_ret = cre2_set_add_simple(set, "(x)|y");
    (void)add_ret; /* allow either success or failure without affecting cleanup */

    /* Cleanup */
    cre2_set_delete(set);
    cre2_delete(re);
    cre2_opt_delete(opt);

    return 66;
}