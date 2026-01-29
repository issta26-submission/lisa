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
//<ID> 839
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "foo(bar)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_buf[] = "zz foobar yy";
    const size_t text_len = (size_t)(sizeof(text_buf) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_named_groups_iter_t * iter = NULL;
    const char * found_name = NULL;
    int found_index = -1;
    cre2_encoding_t enc = CRE2_UNKNOWN;
    cre2_set * set = NULL;
    int case_flag = 1;
    int add_res = 0;
    int compile_res = 0;
    int match_arr[4];
    size_t set_matches = 0;
    long computed_check = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, case_flag);
    enc = cre2_opt_encoding(opt);

    // step 3: Core operations
    set = cre2_set_new(opt, CRE2_UNANCHORED);
    add_res = cre2_set_add_simple(set, pattern);
    compile_res = cre2_set_compile(set);
    memset(match_arr, 0, sizeof(match_arr));
    set_matches = cre2_set_match(set, text_buf, text_len, match_arr, (size_t)(sizeof(match_arr) / sizeof(match_arr[0])));
    re = cre2_new(pattern, pattern_len, opt);
    iter = cre2_named_groups_iter_new(re);
    cre2_named_groups_iter_next(iter, &found_name, &found_index);
    cre2_named_groups_iter_delete(iter);

    // step 4: Validate
    computed_check = (long)enc + (long)case_flag + (long)add_res + (long)compile_res + (long)set_matches + (long)found_index;
    (void)computed_check;

    // step 5: Cleanup
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}