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
//<ID> 1709
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
    const char source_text[] = "abc abc ab";
    const int source_text_len = (int)(sizeof(source_text) - 1);
    char text_and_target_buf[64];
    char rewrite_buf[16];
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t text_and_target_str;
    cre2_string_t rewrite_str;
    cre2_string_t text_str;
    cre2_string_t pmatch[4];
    int rc_partial = 0;
    int rc_replace = 0;
    int num_groups = 0;
    int dot_nl_flag = 0;
    long checksum = 0;
    cre2_set * set_ptr = NULL;

    // step 2: Setup
    opt = cre2_opt_new();
    re = cre2_new(pattern, pattern_len, opt);

    // step 3: Configure / Operate
    memset(text_and_target_buf, 0, sizeof(text_and_target_buf));
    memset(rewrite_buf, 0, sizeof(rewrite_buf));
    memcpy(text_and_target_buf, source_text, (size_t)source_text_len);
    memcpy(rewrite_buf, "$1-$2", 5);

    text_and_target_str.data = text_and_target_buf;
    text_and_target_str.length = source_text_len;
    rewrite_str.data = rewrite_buf;
    rewrite_str.length = 5;

    text_str.data = text_and_target_buf;
    text_str.length = source_text_len;

    pmatch[0].data = NULL; pmatch[0].length = 0;
    pmatch[1].data = NULL; pmatch[1].length = 0;
    pmatch[2].data = NULL; pmatch[2].length = 0;
    pmatch[3].data = NULL; pmatch[3].length = 0;

    rc_partial = cre2_partial_match(pattern, &text_str, pmatch, 3);
    rc_replace = cre2_replace(pattern, &text_and_target_str, &rewrite_str);
    num_groups = cre2_num_capturing_groups(re);
    dot_nl_flag = cre2_opt_dot_nl(opt);

    // step 4: Validate
    checksum = (long)pattern_len + (long)source_text_len + (long)rc_partial + (long)rc_replace + (long)num_groups + (long)dot_nl_flag + (long)text_and_target_str.length + (long)rewrite_str.length + (long)(re != NULL) + (long)(opt != NULL);
    (void)checksum;

    // step 5: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);
    cre2_set_delete(set_ptr);

    // step 6: API sequence test completed successfully
    return 66;
}