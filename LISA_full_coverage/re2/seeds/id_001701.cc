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
//<ID> 1701
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(ab)(c)?";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char source_text[] = "abc ab";
    const int source_text_len = (int)(sizeof(source_text) - 1);
    char text_buf[64];
    char text_and_target_buf[128];
    char rewrite_buf[16];
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    struct cre2_set * set = NULL;
    cre2_string_t text_str;
    cre2_string_t text_and_target_str;
    cre2_string_t rewrite_str;
    cre2_string_t pmatch[4];
    int rc_partial = 0;
    int rc_replace = 0;
    int num_groups = 0;
    int dotnl_flag = 0;
    long checksum = 0;

    // step 2: Setup
    memset(text_buf, 0, sizeof(text_buf));
    memset(text_and_target_buf, 0, sizeof(text_and_target_buf));
    memset(rewrite_buf, 0, sizeof(rewrite_buf));
    memcpy(text_buf, source_text, (size_t)source_text_len);
    memcpy(text_and_target_buf, source_text, (size_t)source_text_len);
    memcpy(rewrite_buf, "$1-$2", 5);

    opt = cre2_opt_new();
    cre2_opt_set_dot_nl(opt, 1);
    dotnl_flag = cre2_opt_dot_nl(opt);

    // step 3: Configure
    re = cre2_new(pattern, pattern_len, opt);
    set = cre2_set_new(opt, (cre2_anchor_t)0);

    // step 4: Operate
    text_str.data = text_buf;
    text_str.length = source_text_len;
    memset(pmatch, 0, sizeof(pmatch));
    rc_partial = cre2_partial_match(pattern, &text_str, pmatch, 4);

    text_and_target_str.data = text_and_target_buf;
    text_and_target_str.length = source_text_len;
    rewrite_str.data = rewrite_buf;
    rewrite_str.length = 5;
    num_groups = cre2_num_capturing_groups(re);
    rc_replace = cre2_replace(pattern, &text_and_target_str, &rewrite_str);

    // step 5: Validate
    checksum = (long)pattern_len + (long)source_text_len + (long)rc_partial + (long)rc_replace + (long)num_groups + (long)dotnl_flag + (long)text_and_target_str.length + (long)(opt != NULL) + (long)(re != NULL) + (long)(set != NULL);
    (void)checksum;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);
    cre2_set_delete(set);

    // API sequence test completed successfully
    return 66;
}