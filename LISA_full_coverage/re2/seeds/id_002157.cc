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
//<ID> 2157
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "([A-Za-z]+)-([0-9]+)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_buf[] = "User-1234 and Admin-5678";
    const int text_len = (int)(sizeof(text_buf) - 1);
    char text_and_target_buf[64];
    memset(text_and_target_buf, 0, sizeof(text_and_target_buf));
    char rewrite_buf[] = "$1:$2";
    cre2_string_t matches[3];
    cre2_string_t text_and_target;
    cre2_string_t rewrite;
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_set * set = NULL;
    int easy_ret = 0;
    int replace_ret = 0;
    int cs_flag = 0;
    long checksum = 0;

    // step 2: Setup (create and configure options)
    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, 0);
    cre2_opt_set_perl_classes(opt, 1);
    cs_flag = cre2_opt_case_sensitive(opt);

    // step 3: Compile regexp and create a set
    re = cre2_new(pattern, pattern_len, opt);
    set = cre2_set_new(opt, CRE2_UNANCHORED);

    // step 4: Initialize match and replacement containers
    matches[0].data = NULL; matches[0].length = 0;
    matches[1].data = NULL; matches[1].length = 0;
    matches[2].data = NULL; matches[2].length = 0;
    /* prepare text_and_target for in-place replacement */
    memcpy(text_and_target_buf, text_buf, (size_t)text_len);
    text_and_target.data = text_and_target_buf;
    text_and_target.length = text_len;
    rewrite.data = rewrite_buf;
    rewrite.length = (int)(sizeof(rewrite_buf) - 1);

    // step 5: Operate (easy match and replace using compiled regexp)
    easy_ret = cre2_easy_match(pattern, pattern_len, text_buf, text_len, matches, 3);
    replace_ret = cre2_replace_re(re, &text_and_target, &rewrite);

    // step 6: Validate and Cleanup (combine results and release resources)
    checksum = (long)pattern_len + (long)easy_ret + (long)replace_ret + (long)matches[0].length + (long)matches[1].length + (long)matches[2].length + (long)cs_flag + (long)(re != NULL) + (long)(set != NULL) + (long)(opt != NULL) + (long)text_and_target.length;
    (void)checksum;
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}