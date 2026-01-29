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
//<ID> 401
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "a(bc)?d";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char set_pat1[] = "hello";
    const char set_pat2[] = "world";
    const char text_buf[] = "xxabcdyy hello";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_options_t *opt = NULL;
    cre2_regexp_t *re = NULL;
    struct cre2_set *set = NULL;
    cre2_string_t easy_matches[3];
    int set_matches[8];
    int easy_res = 0;
    int num_groups = 0;
    size_t set_match_count = 0;
    int add_res1 = 0;
    int add_res2 = 0;
    int compile_res = 0;
    int computed_check = 0;

    // step 2: Setup
    memset(easy_matches, 0, sizeof(easy_matches));
    memset(set_matches, 0, sizeof(set_matches));
    opt = cre2_opt_new();
    set = cre2_set_new(opt, CRE2_UNANCHORED);

    // step 3: Configure
    cre2_opt_set_literal(opt, 0);
    cre2_opt_set_dot_nl(opt, 1);
    re = cre2_new(pattern, pattern_len, opt);
    add_res1 = cre2_set_add_simple(set, set_pat1);
    add_res2 = cre2_set_add_simple(set, set_pat2);
    compile_res = cre2_set_compile(set);

    // step 4: Operate
    easy_res = cre2_easy_match(pattern, pattern_len, text_buf, text_len, easy_matches, 3);
    num_groups = cre2_num_capturing_groups(re);
    set_match_count = cre2_set_match(set, text_buf, (size_t)text_len, set_matches, (size_t)(sizeof(set_matches)/sizeof(set_matches[0])));

    // step 5: Validate
    computed_check = easy_res + num_groups + (int)set_match_count + easy_matches[0].length + easy_matches[1].length + add_res1 + add_res2 + compile_res + pattern_len + text_len;
    (void)computed_check;

    // step 6: Cleanup
    cre2_set_delete(set);
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}