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
//<ID> 815
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations (Initialize)
    const char pattern[] = "foo";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_buf[] = "xx foo foo yy";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t text_and_target;
    cre2_string_t rewrite;
    int replace_res = 0;
    int computed_check = 0;

    // step 2: Configure
    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, 1);

    // step 3: Operate (compile and prepare strings)
    re = cre2_new(pattern, pattern_len, opt);
    memset(&text_and_target, 0, sizeof(text_and_target));
    text_and_target.data = text_buf;
    text_and_target.length = text_len;
    memset(&rewrite, 0, sizeof(rewrite));
    rewrite.data = "bar";
    rewrite.length = (int)(sizeof("bar") - 1);
    replace_res = cre2_replace(cre2_pattern(re), &text_and_target, &rewrite);

    // step 4: Validate (use returned values and pattern info to compute a check)
    computed_check = replace_res + text_and_target.length + rewrite.length + (int)cre2_pattern(re)[0];
    (void)computed_check;

    // step 5: Cleanup
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}