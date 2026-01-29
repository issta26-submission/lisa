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
//<ID> 1346
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(?P<name>foo)(bar)?";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_buf[] = "prefix foobar suffix";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_named_groups_iter_t * iter = NULL;
    cre2_string_t text_str;
    cre2_string_t rewrite_str;
    cre2_string_t target_str;
    char target_buf[128];
    int extract_ret = 0;
    int err_code = 0;
    const char * pat_ptr = NULL;
    long computed_check = 0;

    // step 2: Setup
    memset(&text_str, 0, sizeof(text_str));
    memset(&rewrite_str, 0, sizeof(rewrite_str));
    memset(&target_str, 0, sizeof(target_str));
    memset(target_buf, 0, sizeof(target_buf));
    opt = cre2_opt_new();
    cre2_opt_set_perl_classes(opt, 1);

    // step 3: Operate
    re = cre2_new(pattern, pattern_len, opt);
    pat_ptr = cre2_pattern(re);
    iter = cre2_named_groups_iter_new(re);
    text_str.data = text_buf;
    text_str.length = text_len;
    rewrite_str.data = "$1-$2";
    rewrite_str.length = (int)(sizeof("$1-$2") - 1);
    target_str.data = target_buf;
    target_str.length = 0;
    extract_ret = cre2_extract_re(re, &text_str, &rewrite_str, &target_str);

    // step 4: Validate
    err_code = cre2_error_code(re);
    computed_check = (long)pattern_len + (long)text_len + (long)extract_ret + (long)err_code
                     + (long)target_str.length + (long)rewrite_str.length
                     + (long)(re != NULL) + (long)(iter != NULL) + (long)(pat_ptr != NULL);
    (void)computed_check;

    // step 5: Cleanup
    cre2_named_groups_iter_delete(iter);
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}