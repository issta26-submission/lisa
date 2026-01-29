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
//<ID> 630
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(a)(b)c";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char set_pat[] = "foo|bar";
    const char text_buf[] = "foo and bar in foobar";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    struct cre2_set * set = NULL;
    cre2_string_t text_and_target;
    cre2_string_t rewrite;
    int matches[8];
    size_t set_match_count = 0;
    int rc_replace = 0;
    int ver_rev = 0;
    int computed_check = 0;

    // step 2: Setup
    memset(matches, 0, sizeof(matches));
    opt = cre2_opt_new();
    ver_rev = cre2_version_interface_revision();

    // step 3: Configure
    re = cre2_new(pattern, pattern_len, opt);
    set = cre2_set_new(opt, (cre2_anchor_t)0);
    cre2_set_add_simple(set, set_pat);
    cre2_set_compile(set);

    // step 4: Operate
    text_and_target.data = text_buf;
    text_and_target.length = text_len;
    rewrite.data = "X";
    rewrite.length = 1;
    rc_replace = cre2_global_replace_re(re, &text_and_target, &rewrite);
    set_match_count = cre2_set_match(set, text_buf, (size_t)text_len, matches, (size_t)(sizeof(matches)/sizeof(matches[0])));

    // step 5: Validate
    computed_check = ver_rev + rc_replace + (int)set_match_count + matches[0] + matches[1] + text_and_target.length + rewrite.length;
    (void)computed_check;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}