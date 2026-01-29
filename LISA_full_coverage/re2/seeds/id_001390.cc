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
//<ID> 1390
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern_replace[] = "(foo)";
    const int pattern_replace_len = (int)(sizeof(pattern_replace) - 1);
    const char pattern_rex[] = "(bar)";
    const int pattern_rex_len = (int)(sizeof(pattern_rex) - 1);
    cre2_options_t * opt = NULL;
    cre2_set * set = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t text_and_target;
    cre2_string_t rewrite;
    char text_buf[] = "foo bar foo baz";
    char rewrite_buf[] = "X";
    char set_error[128];
    int rc_replace = 0;
    int rc_global_replace_re = 0;
    int rc_set_add = 0;
    int num_groups = 0;
    long computed_check = 0;

    // step 2: Setup
    memset(&computed_check, 0, sizeof(computed_check));
    memset(set_error, 0, sizeof(set_error));
    opt = cre2_opt_new();
    text_and_target.data = text_buf;
    text_and_target.length = (int)(sizeof(text_buf) - 1);
    rewrite.data = rewrite_buf;
    rewrite.length = (int)(sizeof(rewrite_buf) - 1);

    // step 3: Configure / create objects
    set = cre2_set_new(opt, CRE2_UNANCHORED);
    re = cre2_new(pattern_rex, pattern_rex_len, opt);

    // step 4: Core operations (use required APIs)
    rc_replace = cre2_replace(pattern_replace, &text_and_target, &rewrite);
    rc_global_replace_re = cre2_global_replace_re(re, &text_and_target, &rewrite);
    rc_set_add = cre2_set_add(set, pattern_replace, (size_t)pattern_replace_len, set_error, sizeof(set_error));
    num_groups = cre2_num_capturing_groups(re);

    // step 5: Validate / compute lightweight checksum
    computed_check = (long)pattern_replace_len + (long)pattern_rex_len
                     + (long)rc_replace + (long)rc_global_replace_re + (long)rc_set_add
                     + (long)num_groups + (long)text_and_target.length + (long)rewrite.length
                     + (long)(opt != NULL) + (long)(set != NULL) + (long)(re != NULL)
                     + (long)(set_error[0] != 0);
    (void)computed_check;
    (void)set_error;

    // step 6: Cleanup
    cre2_set_delete(set);
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}