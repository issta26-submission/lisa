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
//<ID> 819
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
    char text_buf[] = "xx foo yy";
    const int text_len = (int)(sizeof(text_buf) - 1);
    char rewrite_buf[] = "bar";
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    const char * pat_from_re = NULL;
    cre2_string_t text_and_target;
    cre2_string_t rewrite;
    int case_flag = 1;
    int replace_res = 0;
    long computed_check = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, case_flag);

    // step 3: Operate
    re = cre2_new(pattern, pattern_len, opt);
    pat_from_re = cre2_pattern(re);
    memset(&text_and_target, 0, sizeof(text_and_target));
    text_and_target.data = text_buf;
    text_and_target.length = text_len;
    memset(&rewrite, 0, sizeof(rewrite));
    rewrite.data = rewrite_buf;
    rewrite.length = (int)(sizeof(rewrite_buf) - 1);
    replace_res = cre2_replace(pat_from_re, &text_and_target, &rewrite);

    // step 4: Validate
    computed_check = (long)replace_res + (long)text_and_target.length + (long)(pat_from_re ? pat_from_re[0] : 0) + (long)case_flag;
    (void)computed_check;

    // step 5: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}