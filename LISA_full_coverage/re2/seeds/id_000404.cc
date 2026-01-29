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
//<ID> 404
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(ab)(?:cd)?(ef)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char set_pattern[] = "hello|world|ab";
    const size_t set_pattern_len = (size_t)(sizeof(set_pattern) - 1);
    const char text_buf[] = "xxabefyy hello world zz";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_options_t *opt = NULL;
    cre2_regexp_t *re = NULL;
    struct cre2_set *set = NULL;
    cre2_string_t easy_matches[4];
    cre2_string_t text_sp;
    int set_match_indices[8];
    size_t set_match_count = 0;
    int add_simple_res = 0;
    int compile_res = 0;
    int easy_res = 0;
    int num_groups = 0;
    int err_code = 0;
    int computed_check = 0;

    // step 2: Setup
    memset(easy_matches, 0, sizeof(easy_matches));
    memset(set_match_indices, 0, sizeof(set_match_indices));
    text_sp.data = text_buf;
    text_sp.length = text_len;
    opt = cre2_opt_new();
    set = cre2_set_new(opt, CRE2_UNANCHORED);

    // step 3: Configure
    cre2_opt_set_literal(opt, 0);
    cre2_opt_set_dot_nl(opt, 1);
    re = cre2_new(pattern, pattern_len, opt);
    add_simple_res = cre2_set_add_simple(set, set_pattern);
    compile_res = cre2_set_compile(set);

    // step 4: Operate
    easy_res = cre2_easy_match(pattern, pattern_len, text_buf, text_len, easy_matches, 4);
    num_groups = cre2_num_capturing_groups(re);
    set_match_count = cre2_set_match(set, text_buf, (size_t)text_len, set_match_indices, (size_t)(sizeof(set_match_indices) / sizeof(set_match_indices[0])));

    // step 5: Validate
    err_code = cre2_error_code(re);
    computed_check = easy_res + num_groups + (int)set_match_count + add_simple_res + compile_res + easy_matches[0].length + easy_matches[1].length + err_code + pattern_len + text_len;
    (void)computed_check;

    // step 6: Cleanup
    cre2_set_delete(set);
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}