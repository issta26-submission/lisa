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
//<ID> 636
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "foo";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char text_buf[] = "foo and foo";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    struct cre2_set * set = NULL;
    cre2_string_t text_and_target;
    cre2_string_t rewrite;
    int match[4];
    size_t match_len = 4;
    size_t matched = 0;
    int version_rev = 0;
    int rc_replace = 0;
    int computed_check = 0;

    // step 2: Setup
    memset(match, 0, sizeof(match));
    text_and_target.data = text_buf;
    text_and_target.length = text_len;
    rewrite.data = "bar";
    rewrite.length = 3;
    opt = cre2_opt_new();

    // step 3: Configure
    version_rev = cre2_version_interface_revision();
    re = cre2_new(pattern, pattern_len, opt);
    set = cre2_set_new(opt, (cre2_anchor_t)0);
    cre2_set_add_simple(set, "foo");
    cre2_set_compile(set);

    // step 4: Operate
    rc_replace = cre2_global_replace_re(re, &text_and_target, &rewrite);
    matched = cre2_set_match(set, text_buf, (size_t)text_len, match, match_len);

    // step 5: Validate
    computed_check = version_rev + rc_replace + (int)matched + match[0] + text_and_target.length + rewrite.length;
    (void)computed_check;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}