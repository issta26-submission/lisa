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
//<ID> 1700
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(a)(b)?";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char source_text[] = "abx";
    const int source_text_len = (int)(sizeof(source_text) - 1);
    char text_and_target_buf[64];
    char rewrite_buf[16];
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    struct cre2_set * set = NULL;
    cre2_string_t text_and_target_str;
    cre2_string_t rewrite_str;
    cre2_string_t pmatch[4];
    int num_groups = 0;
    int rc_partial = 0;
    int rc_replace = 0;
    long checksum = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_dot_nl(opt, 1);
    set = cre2_set_new(opt, (cre2_anchor_t)0);
    re = cre2_new(pattern, pattern_len, opt);

    // step 3: Prepare buffers and structures
    memset(text_and_target_buf, 0, sizeof(text_and_target_buf));
    memset(rewrite_buf, 0, sizeof(rewrite_buf));
    memcpy(text_and_target_buf, source_text, (size_t)source_text_len);
    memcpy(rewrite_buf, "$1$2", 4);
    memset(pmatch, 0, sizeof(pmatch));
    text_and_target_str.data = text_and_target_buf;
    text_and_target_str.length = source_text_len;
    rewrite_str.data = rewrite_buf;
    rewrite_str.length = 4;

    // step 4: Core API operations
    num_groups = cre2_num_capturing_groups(re);
    rc_partial = cre2_partial_match(pattern, &text_and_target_str, pmatch, 4);
    rc_replace = cre2_replace(pattern, &text_and_target_str, &rewrite_str);

    // step 5: Validate results
    checksum = (long)pattern_len + (long)num_groups + (long)rc_partial + (long)rc_replace + (long)text_and_target_str.length + (long)rewrite_str.length + (long)(opt != NULL) + (long)(re != NULL) + (long)(set != NULL);
    (void)checksum;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);
    cre2_set_delete(set);

    // API sequence test completed successfully
    return 66;
}