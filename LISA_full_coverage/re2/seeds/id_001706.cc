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
//<ID> 1706
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
    const char source_text[] = "foobar foo";
    const int source_text_len = (int)(sizeof(source_text) - 1);
    char text_and_target_buf[128];
    char rewrite_buf[] = "$1-$2";
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    struct cre2_set * set = NULL;
    cre2_string_t text_str;
    cre2_string_t text_and_target_str;
    cre2_string_t rewrite_str;
    cre2_string_t pmatch[3];
    int num_groups = 0;
    int rc_partial = 0;
    int rc_replace = 0;
    int dot_nl_flag = 0;
    long checksum = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_dot_nl(opt, 1);
    dot_nl_flag = cre2_opt_dot_nl(opt);
    re = cre2_new(pattern, pattern_len, opt);
    /* cre2_set_new expects a cre2_anchor_t for the second parameter.
       Use an explicit cast from 0 to cre2_anchor_t to match the signature
       without changing the logic. */
    set = cre2_set_new(opt, (cre2_anchor_t)0);

    // step 3: Configure
    memset(text_and_target_buf, 0, sizeof(text_and_target_buf));
    memcpy(text_and_target_buf, source_text, (size_t)source_text_len);
    text_and_target_str.data = text_and_target_buf;
    text_and_target_str.length = source_text_len;
    rewrite_str.data = rewrite_buf;
    rewrite_str.length = (int)(sizeof(rewrite_buf) - 1);
    text_str.data = text_and_target_buf;
    text_str.length = source_text_len;
    pmatch[0].data = NULL; pmatch[0].length = 0;
    pmatch[1].data = NULL; pmatch[1].length = 0;
    pmatch[2].data = NULL; pmatch[2].length = 0;

    // step 4: Operate
    num_groups = cre2_num_capturing_groups(re);
    rc_partial = cre2_partial_match(pattern, &text_str, pmatch, 3);
    rc_replace = cre2_replace(pattern, &text_and_target_str, &rewrite_str);

    // step 5: Validate
    checksum = 0;
    checksum += (long)pattern_len;
    checksum += (long)num_groups;
    checksum += (long)rc_partial;
    checksum += (long)rc_replace;
    checksum += (long)dot_nl_flag;
    checksum += (long)text_and_target_str.length;
    checksum += (long)(opt != NULL);
    checksum += (long)(re != NULL);
    checksum += (long)(set != NULL);
    (void)checksum;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);
    cre2_set_delete(set);

    // API sequence test completed successfully
    return 66;
}