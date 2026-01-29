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
//<ID> 201
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
    const char input_buf[] = "prefix foobar suffix";
    const int input_len = (int)(sizeof(input_buf) - 1);
    char work_buf[64];
    char target_buf[128];
    char rewrite_buf[] = "X-$1-$2-X";
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_named_groups_iter_t * iter = NULL;
    cre2_string_t text;
    cre2_string_t text_and_target;
    cre2_string_t rewrite;
    cre2_string_t target;
    int extract_res = 0;
    int replace_res = 0;

    // step 2: Setup
    memset(&text, 0, sizeof(text));
    memset(&text_and_target, 0, sizeof(text_and_target));
    memset(&rewrite, 0, sizeof(rewrite));
    memset(&target, 0, sizeof(target));
    memset(work_buf, 0, sizeof(work_buf));
    memset(target_buf, 0, sizeof(target_buf));
    opt = cre2_opt_new();
    re = cre2_new(pattern, pattern_len, opt);
    iter = cre2_named_groups_iter_new(re);

    // step 3: Configure
    text.data = input_buf;
    text.length = input_len;
    text_and_target.data = work_buf;
    /* copy input into work_buf for in-place replace */
    /* no loops/branches allowed, use memcpy via assignment through known sizes */
    (void)memcpy(work_buf, input_buf, (size_t)input_len);
    text_and_target.length = input_len;
    rewrite.data = rewrite_buf;
    rewrite.length = (int)(sizeof(rewrite_buf) - 1);
    target.data = target_buf;
    target.length = 0;
    cre2_opt_set_case_sensitive(opt, 1);

    // step 4: Core operations
    extract_res = cre2_extract_re(re, &text, &rewrite, &target);
    replace_res = cre2_global_replace_re(re, &text_and_target, &rewrite);

    // step 5: Validate / propagate results
    target.length = target.length + ((extract_res + replace_res) & 15);
    text_and_target.length = text_and_target.length + ((target.length ^ extract_res) & 7);
    cre2_opt_set_case_sensitive(opt, (replace_res >> 1) & 1);
    (void)pattern_len;
    (void)input_len;
    (void)iter;

    // step 6: Cleanup
    cre2_named_groups_iter_delete(iter);
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}