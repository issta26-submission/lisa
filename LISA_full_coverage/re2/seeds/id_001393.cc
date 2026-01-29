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
//<ID> 1393
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(foo)(bar)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char simple_pattern[] = "foo";
    const int simple_pattern_len = (int)(sizeof(simple_pattern) - 1);
    cre2_options_t * opt = NULL;
    cre2_set * set = NULL;
    cre2_regexp_t * re = NULL;
    char set_error[128];
    int add_ret = 0;
    int num_groups = 0;
    char text_buf[] = "foobarfoobarfoo";
    cre2_string_t text_and_target = { text_buf, (int)(sizeof(text_buf) - 1) };
    char rewrite_simple_buf[] = "QUX";
    cre2_string_t rewrite_simple = { rewrite_simple_buf, (int)(sizeof(rewrite_simple_buf) - 1) };
    char rewrite_regex_buf[] = "$1_";
    cre2_string_t rewrite_regex = { rewrite_regex_buf, (int)(sizeof(rewrite_regex_buf) - 1) };
    int replace_ret = 0;
    int global_replace_ret = 0;
    long computed_check = 0;

    // step 2: Setup
    memset(&computed_check, 0, sizeof(computed_check));
    memset(set_error, 0, sizeof(set_error));
    opt = cre2_opt_new();

    // step 3: Configure / prepare set and regex
    set = cre2_set_new(opt, CRE2_UNANCHORED);
    add_ret = cre2_set_add(set, pattern, (size_t)pattern_len, set_error, sizeof(set_error));
    re = cre2_new(pattern, pattern_len, opt);

    // step 4: Core operations: inspect regex and perform replacements
    num_groups = cre2_num_capturing_groups(re);
    replace_ret = cre2_replace(simple_pattern, &text_and_target, &rewrite_simple);
    global_replace_ret = cre2_global_replace_re(re, &text_and_target, &rewrite_regex);

    // step 5: Validate / compute lightweight checksum
    computed_check = (long)pattern_len + (long)simple_pattern_len + (long)num_groups
                     + (long)add_ret + (long)replace_ret + (long)global_replace_ret
                     + (long)(opt != NULL) + (long)(set != NULL) + (long)(re != NULL)
                     + (long)(text_and_target.length);
    (void)computed_check;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}