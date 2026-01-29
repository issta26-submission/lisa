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
//<ID> 2179
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(?P<first>[A-Za-z]+)-(?P<num>[0-9]+)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_buf[] = "Alice-123 Bob-456";
    const int text_len = (int)(sizeof(text_buf) - 1);
    const char rewrite_buf[] = "$1:$2";
    const int rewrite_len = (int)(sizeof(rewrite_buf) - 1);
    cre2_string_t text_and_target;
    cre2_string_t rewrite;
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_named_groups_iter_t * iter = NULL;
    int never_capture_flag = 0;
    int word_boundary_flag = 0;
    int replace_ret = 0;
    long checksum = 0;

    // step 2: Setup
    memset(&text_and_target, 0, sizeof(text_and_target));
    memset(&rewrite, 0, sizeof(rewrite));
    opt = cre2_opt_new();
    cre2_opt_set_never_capture(opt, 1);
    cre2_opt_set_word_boundary(opt, 1);

    // step 3: Configure
    never_capture_flag = cre2_opt_never_capture(opt);
    word_boundary_flag = cre2_opt_word_boundary(opt);
    re = cre2_new(pattern, pattern_len, opt);

    // step 4: Operate
    iter = cre2_named_groups_iter_new(re);
    text_and_target.data = text_buf;
    text_and_target.length = text_len;
    rewrite.data = rewrite_buf;
    rewrite.length = rewrite_len;
    replace_ret = cre2_global_replace_re(re, &text_and_target, &rewrite);

    // step 5: Validate
    checksum = (long)pattern_len + (long)replace_ret + (long)never_capture_flag + (long)word_boundary_flag + (long)text_and_target.length + (long)rewrite.length + (long)(opt != NULL) + (long)(re != NULL) + (long)(iter != NULL);
    (void)checksum;

    // step 6: Cleanup
    cre2_named_groups_iter_delete(iter);
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}