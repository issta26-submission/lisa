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
//<ID> 445
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(?P<first>foo)(bar)?";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char set_pattern[] = "hello|foo|bar";
    const char text_buf[] = "prefix foo suffix";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_options_t *opt = NULL;
    cre2_regexp_t *re = NULL;
    cre2_set *set = NULL;
    cre2_named_groups_iter_t *iter = NULL;
    cre2_string_t text_sp;
    const char *found_name = NULL;
    int found_index = -1;
    int add_res = 0;
    int compile_res = 0;
    int one_line_flag = 0;
    cre2_encoding_t enc = CRE2_UNKNOWN;
    int next_res_int = 0;
    int computed_check = 0;

    // step 2: Setup
    memset(&text_sp, 0, sizeof(text_sp));
    text_sp.data = text_buf;
    text_sp.length = text_len;
    opt = cre2_opt_new();
    set = cre2_set_new(opt, CRE2_UNANCHORED);

    // step 3: Configure
    cre2_opt_set_one_line(opt, 1);
    one_line_flag = cre2_opt_one_line(opt);
    cre2_opt_set_encoding(opt, CRE2_UTF8);
    enc = cre2_opt_encoding(opt);
    re = cre2_new(pattern, pattern_len, opt);
    add_res = cre2_set_add_simple(set, set_pattern);
    compile_res = cre2_set_compile(set);

    // step 4: Operate
    iter = cre2_named_groups_iter_new(re);
    next_res_int = (int)cre2_named_groups_iter_next(iter, &found_name, &found_index);

    // step 5: Validate
    computed_check = pattern_len + (int)text_sp.length + add_res + compile_res + one_line_flag + (int)enc + found_index + next_res_int + (int)(found_name != NULL);
    (void)computed_check;

    // step 6: Cleanup
    cre2_named_groups_iter_delete(iter);
    cre2_set_delete(set);
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}