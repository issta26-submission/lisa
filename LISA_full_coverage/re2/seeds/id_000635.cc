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
//<ID> 635
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(a)(b)?";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char text_buf[] = "xxabyyab";
    const int text_len = (int)(sizeof(text_buf) - 1);
    const char rewrite_buf[] = "R";
    const int rewrite_len = (int)(sizeof(rewrite_buf) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * rex = NULL;
    cre2_set * set = NULL;
    cre2_string_t text_and_target;
    cre2_string_t rewrite_string;
    int matches[4];
    size_t set_matches = 0;
    int rc_replace = 0;
    int computed_check = 0;

    // step 2: Setup
    memset(matches, 0, sizeof(matches));
    text_and_target.data = text_buf;
    text_and_target.length = text_len;
    rewrite_string.data = rewrite_buf;
    rewrite_string.length = rewrite_len;
    opt = cre2_opt_new();

    // step 3: Configure
    cre2_opt_set_case_sensitive(opt, 1);
    rex = cre2_new(pattern, pattern_len, opt);
    set = cre2_set_new(opt, (cre2_anchor_t)0);
    cre2_set_add_simple(set, "ab");
    cre2_set_compile(set);

    // step 4: Operate
    rc_replace = cre2_global_replace_re(rex, &text_and_target, &rewrite_string);
    set_matches = cre2_set_match(set, text_and_target.data, (size_t)text_and_target.length, matches, (size_t)4);

    // step 5: Validate
    computed_check = cre2_version_interface_revision() + rc_replace + (int)set_matches + matches[0] + matches[1] + text_and_target.length + rewrite_string.length;
    (void)computed_check;

    // step 6: Cleanup
    cre2_delete(rex);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}