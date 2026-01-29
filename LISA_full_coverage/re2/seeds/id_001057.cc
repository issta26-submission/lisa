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
//<ID> 1057
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "([A-Z]+)([0-9]+)?";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_buf[] = "ABC123 def GHI456 JKL";
    const int text_len = (int)(sizeof(text_buf) - 1);
    char match_buf[128];
    int match_arr[8];
    const size_t match_len = sizeof(match_arr) / sizeof(match_arr[0]);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_set * set = NULL;
    cre2_string_t match_str;
    int match_res = 0;
    size_t set_matches = 0;
    const char * compiled_pat = NULL;
    int64_t opt_max_mem = 0;
    long computed_check = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_case_sensitive(opt, 1);
    cre2_opt_set_log_errors(opt, 1);
    cre2_opt_set_max_mem(opt, (int64_t)1048576);
    memset(match_buf, 0, sizeof(match_buf));
    memset(match_arr, 0, sizeof(match_arr));

    // step 3: Core operations (compile regexp and build set)
    re = cre2_new(pattern, pattern_len, opt);
    set = cre2_set_new(opt, CRE2_UNANCHORED);
    cre2_set_add_simple(set, pattern);
    cre2_set_compile(set);

    // step 4: Operate (perform a match and a set match, query pattern and option)
    match_str.data = match_buf;
    match_str.length = 0;
    match_res = cre2_match(re, text_buf, text_len, 0, text_len, CRE2_UNANCHORED, &match_str, 3);
    set_matches = cre2_set_match(set, text_buf, (size_t)text_len, match_arr, match_len);
    compiled_pat = cre2_pattern(re);
    opt_max_mem = cre2_opt_max_mem(opt);

    // step 5: Validate / compute lightweight checksum
    computed_check = (long)match_res + (long)set_matches + (long)match_str.length + (long)pattern_len + (long)text_len + (long)opt_max_mem + (long)(compiled_pat ? compiled_pat[0] : 0);
    (void)computed_check;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);
    // API sequence test completed successfully
    return 66;
}