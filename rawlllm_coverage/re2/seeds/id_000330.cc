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
//<ID> 330
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // Create options and set the dot_nl option
    cre2_options_t *opt = cre2_opt_new();
    if (!opt) {
        return 1; // failed to allocate options
    }
    // use setter (void) and then use the required getter
    cre2_opt_set_dot_nl(opt, 1);
    int dotnl_flag = cre2_opt_dot_nl(opt);

    // Query version interface age (required API use)
    int ver_age = cre2_version_interface_age();

    // Create a set using the options and add a simple pattern (required API use)
    cre2_set *set = cre2_set_new(opt, (cre2_anchor_t)0);
    if (set) {
        int add_res = cre2_set_add_simple(set, "simple_pattern");
        (void)add_res;
    }

    // Create a regexp from a pattern (required API use)
    const char *pattern = "(grp)";
    int pattern_len = (int)strlen(pattern);
    cre2_regexp_t *re = cre2_new(pattern, pattern_len, opt);

    // Prepare rewrite and errmsg structures and call check_rewrite_string (required API use)
    cre2_string_t rewrite;
    rewrite.data = "$1_rewritten";
    rewrite.length = (int)strlen(rewrite.data);

    cre2_string_t errmsg;
    errmsg.data = NULL;
    errmsg.length = 0;

    int check_res = -1;
    if (re) {
        check_res = cre2_check_rewrite_string(re, &rewrite, &errmsg);
    }

    // Clean up allocated objects
    if (set) cre2_set_delete(set);
    if (re) cre2_delete(re);
    cre2_opt_delete(opt);

    // Use results to avoid "unused variable" issues (no-op logic)
    if (dotnl_flag < 0 || ver_age < 0 || check_res < -1) {
        // nothing to do, just keep values referenced
    }

    return 66;
}