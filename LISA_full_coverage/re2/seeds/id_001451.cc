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
//<ID> 1451
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(foo)(bar)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_buf[] = "begin foobar end";
    const int text_len = (int)(sizeof(text_buf) - 1);
    char rewrite_buf[] = "$2-$1";
    const int rewrite_len = (int)(sizeof(rewrite_buf) - 1);
    char errmsg_buf[128];
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t text_and_target;
    cre2_string_t rewrite;
    cre2_string_t errmsg;
    int num_caps = 0;
    int check_res = 0;
    int replace_res = 0;
    long checksum = 0;

    // step 2: Setup
    memset(errmsg_buf, 0, sizeof(errmsg_buf));
    text_and_target.data = text_buf;
    text_and_target.length = text_len;
    rewrite.data = rewrite_buf;
    rewrite.length = rewrite_len;
    errmsg.data = errmsg_buf;
    errmsg.length = 0;
    opt = cre2_opt_new();

    // step 3: Configure
    cre2_opt_set_encoding(opt, CRE2_UTF8);

    // step 4: Core operations
    re = cre2_new(pattern, pattern_len, opt);
    num_caps = cre2_num_capturing_groups(re);
    check_res = cre2_check_rewrite_string(re, &rewrite, &errmsg);
    replace_res = cre2_replace(pattern, &text_and_target, &rewrite);

    // step 5: Validate
    checksum = (long)pattern_len + (long)text_and_target.length + (long)rewrite.length
               + (long)num_caps + (long)check_res + (long)replace_res
               + (long)(re != NULL) + (long)(opt != NULL) + (long)errmsg.length;
    (void)checksum;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}