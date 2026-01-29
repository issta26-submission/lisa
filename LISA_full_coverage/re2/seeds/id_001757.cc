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
//<ID> 1757
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(hello)(world)?";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char source_text[] = "helloworld hello world hello";
    const int source_text_len = (int)(sizeof(source_text) - 1);
    char text_and_target_buf[128];
    char rewrite_buf[32];
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_set * set = NULL;
    cre2_string_t text_and_target_str;
    cre2_string_t rewrite_str;
    int num_groups = 0;
    int replace_res = 0;
    size_t set_match_count = 0;
    int match_indices[16];
    long checksum = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_literal(opt, 1);
    re = cre2_new(pattern, pattern_len, opt);
    num_groups = cre2_num_capturing_groups(re);

    // step 3: Configure
    memset(text_and_target_buf, 0, sizeof(text_and_target_buf));
    memset(rewrite_buf, 0, sizeof(rewrite_buf));
    memcpy(text_and_target_buf, source_text, (size_t)source_text_len);
    memcpy(rewrite_buf, "$1:$2", 5);
    text_and_target_str.data = text_and_target_buf;
    text_and_target_str.length = source_text_len;
    rewrite_str.data = rewrite_buf;
    rewrite_str.length = 5;

    // step 4: Core operations
    replace_res = cre2_replace_re(re, &text_and_target_str, &rewrite_str);
    set = cre2_set_new(opt, CRE2_UNANCHORED);
    cre2_set_add_simple(set, pattern);
    cre2_set_compile(set);
    memset(match_indices, 0, sizeof(match_indices));
    set_match_count = cre2_set_match(set, text_and_target_buf, (size_t)text_and_target_str.length, match_indices, (size_t)16);

    // step 5: Validate
    checksum = (long)pattern_len + (long)source_text_len + (long)num_groups + (long)replace_res + (long)set_match_count + (long)text_and_target_str.length + (long)rewrite_str.length + (long)(re != NULL) + (long)(opt != NULL) + (long)match_indices[0];
    (void)checksum;

    // step 6: Cleanup
    cre2_set_delete(set);
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}