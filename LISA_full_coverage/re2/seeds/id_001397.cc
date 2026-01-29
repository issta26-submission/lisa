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
//<ID> 1397
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(foo)(bar)?";
    const size_t pattern_len = (size_t)(sizeof(pattern) - 1);
    cre2_options_t * opt = NULL;
    cre2_set * set = NULL;
    cre2_regexp_t * re = NULL;
    int num_groups = 0;
    int set_add_ret = 0;
    int replace_ret = 0;
    int global_replace_ret = 0;
    long computed_check = 0;
    char error_buf[128];
    char mutable_text[128];
    cre2_string_t text_and_target;
    cre2_string_t rewrite;

    // step 2: Setup
    memset(&computed_check, 0, sizeof(computed_check));
    memset(error_buf, 0, sizeof(error_buf));
    opt = cre2_opt_new();
    set = cre2_set_new(opt, (cre2_anchor_t)0);

    // step 3: Configure
    set_add_ret = cre2_set_add(set, pattern, pattern_len, error_buf, sizeof(error_buf));
    re = cre2_new(pattern, (int)pattern_len, opt);

    // step 4: Operate
    num_groups = cre2_num_capturing_groups(re);
    memset(mutable_text, 0, sizeof(mutable_text));
    memcpy(mutable_text, "foo bar foobar", 14);
    text_and_target.data = mutable_text;
    text_and_target.length = (int)strlen(mutable_text);
    rewrite.data = "X";
    rewrite.length = 1;
    replace_ret = cre2_replace(pattern, &text_and_target, &rewrite);
    global_replace_ret = cre2_global_replace_re(re, &text_and_target, &rewrite);

    // step 5: Validate
    computed_check = (long)pattern_len + (long)num_groups + (long)set_add_ret
                     + (long)replace_ret + (long)global_replace_ret
                     + (long)(error_buf[0] != 0) + (long)(re != NULL)
                     + (long)(set != NULL) + (long)(opt != NULL);
    (void)computed_check;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}