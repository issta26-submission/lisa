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
//<ID> 1682
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
    const char source_text[] = "foobar foo bar foobar";
    const int source_text_len = (int)(sizeof(source_text) - 1);
    char text_and_target_buf[64] = {0};
    char rewrite_buf[16] = {0};
    cre2_options_t *opt = NULL;
    cre2_regexp_t *re = NULL;
    cre2_string_t text_and_target_str;
    cre2_string_t rewrite_str;
    int num_groups = 0;
    int replace_rc = 0;
    long checksum = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_literal(opt, 1);

    // step 3: Configure
    re = cre2_new(pattern, pattern_len, opt);

    // step 4: Operate
    /* prepare buffers */
    memset(text_and_target_buf, 0, sizeof(text_and_target_buf));
    memcpy(text_and_target_buf, source_text, (size_t)source_text_len);
    memset(rewrite_buf, 0, sizeof(rewrite_buf));
    memcpy(rewrite_buf, "$1", 2);

    text_and_target_str.data = text_and_target_buf;
    text_and_target_str.length = source_text_len;

    rewrite_str.data = rewrite_buf;
    rewrite_str.length = 2;

    num_groups = cre2_num_capturing_groups(re);
    replace_rc = cre2_replace(pattern, &text_and_target_str, &rewrite_str);

    // step 5: Validate
    checksum = (long)pattern_len + (long)num_groups + (long)replace_rc + (long)text_and_target_str.length + (long)(opt != NULL) + (long)(re != NULL);
    (void)checksum;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}