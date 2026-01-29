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
//<ID> 638
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(foo)(bar)?";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char text_buf[] = "xxfoobarYY";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    struct cre2_set * set = NULL;
    cre2_string_t text_and_target;
    cre2_string_t rewrite;
    int match_array[4];
    size_t set_matches = 0;
    int rc_replace = 0;
    int version_rev = 0;
    int computed_check = 0;
    int nmatch = 4;

    // step 2: Setup
    memset(match_array, 0, sizeof(match_array));
    text_and_target.data = text_buf;
    text_and_target.length = text_len;
    rewrite.data = "BAR";
    rewrite.length = 3;
    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, 1);
    version_rev = cre2_version_interface_revision();

    // step 3: Configure
    re = cre2_new(pattern, pattern_len, opt);
    set = cre2_set_new(opt, (cre2_anchor_t)0);
    cre2_set_add_simple(set, "(foo)");
    cre2_set_add_simple(set, "bar");
    cre2_set_compile(set);

    // step 4: Operate
    rc_replace = cre2_global_replace_re(re, &text_and_target, &rewrite);
    set_matches = cre2_set_match(set, text_buf, (size_t)text_len, match_array, (size_t)nmatch);

    // step 5: Validate
    computed_check = version_rev + rc_replace + (int)set_matches + match_array[0] + text_and_target.length + rewrite.length;
    (void)computed_check;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}