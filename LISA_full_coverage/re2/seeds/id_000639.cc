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
//<ID> 639
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
    char text_buf[] = "foo and foo";
    const int text_len = (int)(sizeof(text_buf) - 1);
    const char rewrite_buf[] = "bar";
    const int rewrite_len = (int)(sizeof(rewrite_buf) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    struct cre2_set * set = NULL;
    cre2_string_t text_and_target;
    cre2_string_t rewrite;
    int match_arr[4];
    size_t match_len = 4;
    size_t match_count = 0;
    int rc_replace = 0;
    int rc_compile = 0;
    int version_rev = 0;
    int computed_check = 0;

    // step 2: Setup
    memset(match_arr, 0, sizeof(match_arr));
    text_and_target.data = text_buf;
    text_and_target.length = text_len;
    rewrite.data = rewrite_buf;
    rewrite.length = rewrite_len;
    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, 1);

    // step 3: Configure
    re = cre2_new(pattern, pattern_len, opt);
    /* cre2_set_new expects a cre2_anchor_t for the second parameter;
       cast 0 to cre2_anchor_t to avoid an int -> enum mismatch. */
    set = cre2_set_new(opt, (cre2_anchor_t)0);
    cre2_set_add_simple(set, pattern);
    rc_compile = cre2_set_compile(set);
    version_rev = cre2_version_interface_revision();

    // step 4: Operate
    rc_replace = cre2_global_replace_re(re, &text_and_target, &rewrite);
    match_count = cre2_set_match(set, text_and_target.data, (size_t)text_and_target.length, match_arr, match_len);

    // step 5: Validate
    computed_check = rc_replace + rc_compile + (int)match_count + match_arr[0] + match_arr[1] + version_rev + text_and_target.length + rewrite.length;
    (void)computed_check;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}