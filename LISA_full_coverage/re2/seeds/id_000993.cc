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
//<ID> 993
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(?P<name>[A-Za-z]+)([0-9]+)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_buf[] = "alice123 bob456 charlie789";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_named_groups_iter_t * iter = NULL;
    cre2_set * set = NULL;
    int match_arr[8];
    size_t match_count = 0;
    int add_res = 0;
    int compile_res = 0;
    cre2_encoding_t enc = CRE2_UNKNOWN;
    long computed_check = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_encoding(opt, CRE2_UTF8);
    cre2_opt_set_case_sensitive(opt, 1);
    enc = cre2_opt_encoding(opt);

    // step 3: Operate - compile regexp, create iterator, create and populate set
    re = cre2_new(pattern, pattern_len, opt);
    iter = cre2_named_groups_iter_new(re);
    set = cre2_set_new(opt, (cre2_anchor_t)0);
    add_res = cre2_set_add_simple(set, pattern);
    compile_res = cre2_set_compile(set);

    // step 4: Operate - attempt to match the set against text and compute checksum
    memset(match_arr, 0xFF, sizeof(match_arr));
    match_count = cre2_set_match(set, text_buf, (size_t)text_len, match_arr, (size_t)(sizeof(match_arr)/sizeof(match_arr[0])));
    computed_check = (long)enc + (long)pattern_len + (long)add_res + (long)compile_res + (long)match_count + (long)match_arr[0];
    (void)computed_check;

    // step 5: Cleanup
    cre2_named_groups_iter_delete(iter);
    cre2_set_delete(set);
    cre2_delete(re);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}