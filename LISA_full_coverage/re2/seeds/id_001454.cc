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
//<ID> 1454
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
    const char text[] = "prefix foobar suffix";
    const int text_len = (int)(sizeof(text) - 1);
    const char rewrite_payload[] = "$2-$1";
    char errmsg_buffer[128];
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t text_and_target;
    cre2_string_t rewrite;
    cre2_string_t errmsg;
    int replace_ret = 0;
    int check_ret = 0;
    int num_caps = 0;
    long checksum = 0;

    // step 2: Setup
    memset(&text_and_target, 0, sizeof(text_and_target));
    memset(&rewrite, 0, sizeof(rewrite));
    memset(&errmsg, 0, sizeof(errmsg));
    memset(errmsg_buffer, 0, sizeof(errmsg_buffer));
    opt = cre2_opt_new();

    // step 3: Configure
    cre2_opt_set_encoding(opt, CRE2_UTF8);

    // step 4: Core operations
    re = cre2_new(pattern, pattern_len, opt);
    num_caps = cre2_num_capturing_groups(re);
    rewrite.data = rewrite_payload;
    rewrite.length = (int)(sizeof(rewrite_payload) - 1);
    errmsg.data = errmsg_buffer;
    errmsg.length = 0;
    check_ret = cre2_check_rewrite_string(re, &rewrite, &errmsg);
    text_and_target.data = text;
    text_and_target.length = text_len;
    replace_ret = cre2_replace(pattern, &text_and_target, &rewrite);

    // step 5: Validate
    checksum = (long)pattern_len + (long)text_len + (long)num_caps + (long)check_ret + (long)replace_ret
               + (long)rewrite.length + (long)errmsg.length + (long)(re != NULL) + (long)(opt != NULL);
    (void)checksum;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}