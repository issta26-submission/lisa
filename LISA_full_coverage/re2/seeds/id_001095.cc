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
//<ID> 1095
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(ab)c?([0-9]+)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_buf[] = "xab12y";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    struct cre2_set * set = NULL;
    cre2_string_t text_str;
    cre2_string_t matches[4];
    const int nmatch = (int)(sizeof(matches) / sizeof(matches[0]));
    int ret_partial = 0;
    int num_groups = 0;
    int set_add_ret = 0;
    int set_compile_ret = 0;
    long computed_check = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, 1);
    cre2_opt_set_longest_match(opt, 1);
    memset(matches, 0, sizeof(matches));
    text_str.data = text_buf;
    text_str.length = text_len;

    // step 3: Core operations (compile single regexp and create/compile a set)
    re = cre2_new(pattern, pattern_len, opt);
    num_groups = cre2_num_capturing_groups(re);
    set = cre2_set_new(opt, CRE2_UNANCHORED);
    set_add_ret = cre2_set_add_simple(set, "foo|bar");
    set_compile_ret = cre2_set_compile(set);

    // step 4: Operate (use partial match on the pattern string API)
    ret_partial = cre2_partial_match(pattern, &text_str, matches, nmatch);

    // step 5: Validate / compute lightweight checksum
    computed_check = (long)num_groups + (long)ret_partial + (long)set_compile_ret + (long)pattern_len + (long)text_len + (long)(matches[0].length) + (long)set_add_ret;
    (void)computed_check;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}