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
//<ID> 139
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    /* Create default options */
    cre2_options_t *opt = cre2_opt_new();
    if (!opt) return 1;

    /* Query version interface age (exercise API) */
    int iface_age = cre2_version_interface_age();
    (void)iface_age; /* silence unused warning */

    /* Create a set with the options (anchor = 0) */
    cre2_set *set = cre2_set_new(opt, (cre2_anchor_t)0);
    if (!set) {
        cre2_opt_delete(opt);
        return 2;
    }

    /* Add a pattern into the set, capturing any error text */
    const char *set_pattern = "(foo)(bar)";
    char errbuf[128] = {0};
    size_t set_pattern_len = 10; /* length of "(foo)(bar)" */
    int add_ret = cre2_set_add(set, set_pattern, set_pattern_len, errbuf, sizeof(errbuf));
    (void)add_ret; /* for this test we continue regardless; in real code check */

    /* Build a single regexp using the same options */
    const char *pattern = "foo";
    int pattern_len = 3;
    cre2_regexp_t *re = cre2_new(pattern, pattern_len, opt);
    if (!re) {
        cre2_set_delete(set);
        cre2_opt_delete(opt);
        return 3;
    }

    /* Prepare text and rewrite strings for global replace */
    std::string text = "abcdef foobar xyz";
    std::string rewrite_s = "baz"; /* replacement text */
    cre2_string_t text_and_target;
    text_and_target.data = text.c_str();
    text_and_target.length = (int)text.size();
    cre2_string_t rewrite;
    rewrite.data = rewrite_s.c_str();
    rewrite.length = (int)rewrite_s.size();

    /* Perform a single global replace using the compiled regexp */
    int grepl_ret = cre2_global_replace_re(re, &text_and_target, &rewrite);
    (void)grepl_ret; /* ignore result for this test */

    /* Cleanup */
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    return 66;
}