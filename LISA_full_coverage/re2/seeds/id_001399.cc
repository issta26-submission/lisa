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
//<ID> 1399
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
    const char add_pattern[] = "foo";
    const size_t add_pattern_len = (size_t)(sizeof(add_pattern) - 1);
    cre2_options_t * opt = NULL;
    cre2_set * set = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t text_and_target;
    cre2_string_t rewrite;
    char errorbuf[128];
    int num_groups = 0;
    int ret_replace = 0;
    int ret_global_replace_re = 0;
    int ret_set_add = 0;
    long checksum = 0;

    // step 2: Setup
    memset(&text_and_target, 0, sizeof(text_and_target));
    memset(&rewrite, 0, sizeof(rewrite));
    memset(errorbuf, 0, sizeof(errorbuf));
    opt = cre2_opt_new();

    // step 3: Configure
    cre2_opt_set_case_sensitive(opt, 1);
    cre2_opt_set_log_errors(opt, 1);

    // step 4: Core operations
    set = cre2_set_new(opt, CRE2_UNANCHORED);
    ret_set_add = cre2_set_add(set, add_pattern, add_pattern_len, errorbuf, sizeof(errorbuf));
    re = cre2_new(pattern, pattern_len, opt);
    num_groups = cre2_num_capturing_groups(re);
    text_and_target.data = "foo bar baz foo";
    text_and_target.length = (int)(sizeof("foo bar baz foo") - 1);
    rewrite.data = "qux";
    rewrite.length = (int)(sizeof("qux") - 1);
    ret_replace = cre2_replace(add_pattern, &text_and_target, &rewrite);
    ret_global_replace_re = cre2_global_replace_re(re, &text_and_target, &rewrite);

    // step 5: Validate
    checksum = (long)num_groups + (long)ret_replace + (long)ret_global_replace_re + (long)ret_set_add + (long)text_and_target.length;
    (void)checksum;
    (void)errorbuf;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}