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
//<ID> 1280
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "user([0-9]+)@example\\.org";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_buf[] = "prefix user123@example.org suffix";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_options_t * opt = NULL;
    cre2_set * set = NULL;
    cre2_regexp_t * re = NULL;
    cre2_string_t match_str;
    int set_match_arr[4];
    size_t set_match_ret = 0;
    int match_ret = 0;
    const char * re_pat = NULL;
    long computed_check = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    set = cre2_set_new(opt, CRE2_UNANCHORED);
    memset(&match_str, 0, sizeof(match_str));
    match_str.data = NULL;
    match_str.length = 0;
    memset(set_match_arr, 0, sizeof(set_match_arr));

    // step 3: Configure
    cre2_opt_set_one_line(opt, 1);
    cre2_set_add_simple(set, pattern);
    cre2_set_compile(set);

    // step 4: Core operations
    re = cre2_new(pattern, pattern_len, opt);
    re_pat = cre2_pattern(re);
    match_ret = cre2_match(re, text_buf, text_len, 0, text_len, CRE2_UNANCHORED, &match_str, 2);
    set_match_ret = cre2_set_match(set, text_buf, (size_t)text_len, set_match_arr, (size_t)4);

    // step 5: Validate / compute lightweight checksum
    computed_check = (long)pattern_len + (long)text_len + (long)match_ret + (long)set_match_ret
                     + (long)(re != NULL) + (long)(re_pat != NULL) + (long)(match_str.length)
                     + (long)(opt != NULL) + (long)(set != NULL) + (long)set_match_arr[0];
    (void)computed_check;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}