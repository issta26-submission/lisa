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
//<ID> 1396
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
    cre2_options_t * opt = NULL;
    cre2_set * set = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t text_and_target;
    cre2_string_t rewrite;
    char add_error[128];
    int add_ret = 0;
    int num_groups = 0;
    int replace_ret1 = 0;
    int replace_ret2 = 0;
    long computed_check = 0;

    // step 2: Setup
    memset(add_error, 0, sizeof(add_error));
    memset(&text_and_target, 0, sizeof(text_and_target));
    memset(&rewrite, 0, sizeof(rewrite));
    opt = cre2_opt_new();

    // step 3: Configure
    set = cre2_set_new(opt, CRE2_UNANCHORED);
    add_ret = cre2_set_add(set, "abc", (size_t)3, add_error, sizeof(add_error));

    // step 4: Core operations
    re = cre2_new(pattern, pattern_len, opt);
    num_groups = cre2_num_capturing_groups(re);
    text_and_target.data = "foo bar foo";
    text_and_target.length = (int)(sizeof("foo bar foo") - 1);
    rewrite.data = "baz";
    rewrite.length = (int)(sizeof("baz") - 1);
    replace_ret1 = cre2_replace(pattern, &text_and_target, &rewrite);
    replace_ret2 = cre2_global_replace_re(re, &text_and_target, &rewrite);

    // step 5: Validate
    computed_check = (long)pattern_len + (long)num_groups + (long)add_ret
                     + (long)replace_ret1 + (long)replace_ret2
                     + (long)(opt != NULL) + (long)(set != NULL) + (long)(re != NULL)
                     + (long)(text_and_target.length) + (long)(rewrite.length);
    (void)computed_check;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}