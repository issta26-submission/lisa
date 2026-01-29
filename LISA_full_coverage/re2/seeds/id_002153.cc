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
//<ID> 2153
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "([A-Za-z]+)-(\\d+)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_and_target_buf[] = "Alice-123 Bob-456";
    const int text_and_target_len = (int)(sizeof(text_and_target_buf) - 1);
    char target_work_buf[128];
    memset(target_work_buf, 0, sizeof(target_work_buf));
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_set * set = NULL;
    cre2_string_t matches[3];
    cre2_string_t text_and_target;
    cre2_string_t rewrite;
    char rewrite_buf[] = "$1:$2";
    int easy_ret = 0;
    int replace_ret = 0;
    long checksum = 0;

    // step 2: Setup (create and configure options)
    opt = cre2_opt_new();
    cre2_opt_set_perl_classes(opt, 1);
    cre2_opt_set_case_sensitive(opt, 0);

    // step 3: Compile regexp and create a set
    re = cre2_new(pattern, pattern_len, opt);
    set = cre2_set_new(opt, CRE2_UNANCHORED);

    // step 4: Initialize match containers and rewrite/target structures
    matches[0].data = NULL; matches[0].length = 0;
    matches[1].data = NULL; matches[1].length = 0;
    matches[2].data = NULL; matches[2].length = 0;
    text_and_target.data = text_and_target_buf;
    text_and_target.length = text_and_target_len;
    rewrite.data = rewrite_buf;
    rewrite.length = (int)(sizeof(rewrite_buf) - 1);

    // step 5: Operate (perform easy match on the text and a replace using compiled regexp)
    easy_ret = cre2_easy_match(pattern, pattern_len, text_and_target_buf, text_and_target_len, matches, 3);
    replace_ret = cre2_replace_re(re, &text_and_target, &rewrite);

    // step 6: Validate and Cleanup (combine values so results flow through, then release resources)
    checksum = (long)pattern_len + (long)easy_ret + (long)replace_ret + (long)matches[0].length + (long)matches[1].length + (long)text_and_target.length + (long)(re != NULL) + (long)(set != NULL) + (long)(opt != NULL);
    (void)checksum;
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}