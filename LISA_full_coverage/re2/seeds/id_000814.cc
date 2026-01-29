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
//<ID> 814
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char orig_pattern[] = "abc";
    const int pattern_len = (int)(sizeof(orig_pattern) - 1);
    char text_buf[] = "xx abc yy";
    const int text_len = (int)(sizeof(text_buf) - 1);
    char rewrite_buf[] = "Z";
    const int rewrite_len = (int)(sizeof(rewrite_buf) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t text_and_target;
    cre2_string_t rewrite;
    int replace_result = 0;
    int computed_check = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, 1);
    re = cre2_new(orig_pattern, pattern_len, opt);

    // step 3: Operate
    memset(&text_and_target, 0, sizeof(text_and_target));
    memset(&rewrite, 0, sizeof(rewrite));
    text_and_target.data = text_buf;
    text_and_target.length = text_len;
    rewrite.data = rewrite_buf;
    rewrite.length = rewrite_len;
    replace_result = cre2_replace(cre2_pattern(re), &text_and_target, &rewrite);

    // step 4: Validate
    computed_check = (int)text_and_target.length + (int)rewrite.length + replace_result + (int)cre2_pattern(re)[0];
    (void)computed_check;

    // step 5: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}