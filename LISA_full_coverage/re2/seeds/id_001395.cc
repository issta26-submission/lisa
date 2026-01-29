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
//<ID> 1395
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(foo)(bar)|baz";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    cre2_options_t * opt = NULL;
    cre2_set * set = NULL;
    cre2_regexp_t * re = NULL;
    char error_buf[128];
    size_t error_len = sizeof(error_buf);
    int add_res = 0;
    cre2_string_t text_and_target;
    cre2_string_t rewrite;
    const char orig_text[] = "foobar baz foo";
    const char rewrite_text[] = "X";
    int replace_res = 0;
    int global_replace_res = 0;
    int num_groups = 0;
    long computed_check = 0;

    // step 2: Setup
    memset(&computed_check, 0, sizeof(computed_check));
    memset(error_buf, 0, error_len);
    opt = cre2_opt_new();

    // step 3: Configure
    set = cre2_set_new(opt, CRE2_UNANCHORED);

    // step 4: Core operations
    add_res = cre2_set_add(set, pattern, (size_t)pattern_len, error_buf, error_len);
    re = cre2_new(pattern, pattern_len, opt);
    text_and_target.data = orig_text;
    text_and_target.length = (int)strlen(orig_text);
    rewrite.data = rewrite_text;
    rewrite.length = (int)strlen(rewrite_text);
    replace_res = cre2_replace(pattern, &text_and_target, &rewrite);
    global_replace_res = cre2_global_replace_re(re, &text_and_target, &rewrite);
    num_groups = cre2_num_capturing_groups(re);

    // step 5: Validate
    computed_check = (long)pattern_len + (long)add_res + (long)replace_res
                     + (long)global_replace_res + (long)num_groups
                     + (long)(opt != NULL) + (long)(set != NULL) + (long)(re != NULL)
                     + (long)(text_and_target.length) + (long)(rewrite.length);
    (void)computed_check;
    (void)error_buf;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}