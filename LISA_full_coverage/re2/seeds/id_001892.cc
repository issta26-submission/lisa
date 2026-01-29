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
//<ID> 1892
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
    cre2_regexp_t * re = NULL;
    const char rewrite_c[] = "\\1-X-\\2";
    const char errmsg_c[] = "";
    cre2_string_t rewrite;
    cre2_string_t errmsg;
    int one_line_flag = 0;
    int check_res = 0;
    int err_code = 0;
    long checksum = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_one_line(opt, 1);

    // step 3: Core operations
    re = cre2_new(pattern, pattern_len, opt);

    // step 4: Operate with rewrite check
    rewrite.data = rewrite_c;
    rewrite.length = (int)(sizeof(rewrite_c) - 1);
    errmsg.data = errmsg_c;
    errmsg.length = 0;
    check_res = cre2_check_rewrite_string(re, &rewrite, &errmsg);

    // step 5: Validate
    one_line_flag = cre2_opt_one_line(opt);
    err_code = cre2_error_code(re);
    checksum = (long)pattern_len + (long)one_line_flag + (long)check_res + (long)err_code + (long)(opt != NULL) + (long)(re != NULL) + (long)rewrite.length + (long)errmsg.length;
    (void)checksum;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}