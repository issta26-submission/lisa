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
//<ID> 1289
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(foo|bar)([0-9]{1,3})";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_buf[] = "prefix foo42 suffix bar7 done";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_set * set = NULL;
    cre2_string_t input;
    cre2_string_t match_arr[4];
    int set_match_indices[8];
    size_t set_matches = 0;
    int set_add_ret = 0;
    int set_compile_ret = 0;
    int re_match_ret = 0;
    const char * re_pat = NULL;
    long computed_check = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    set = cre2_set_new(opt, CRE2_UNANCHORED);
    memset(&input, 0, sizeof(input));
    memset(match_arr, 0, sizeof(match_arr));
    memset(set_match_indices, 0, sizeof(set_match_indices));
    input.data = text_buf;
    input.length = text_len;
    match_arr[0].data = NULL;
    match_arr[0].length = 0;
    match_arr[1].data = NULL;
    match_arr[1].length = 0;
    match_arr[2].data = NULL;
    match_arr[2].length = 0;
    match_arr[3].data = NULL;
    match_arr[3].length = 0;

    // step 3: Configure
    set_add_ret = cre2_set_add_simple(set, pattern);
    set_compile_ret = cre2_set_compile(set);

    // step 4: Core operations
    re = cre2_new(pattern, pattern_len, opt);
    re_match_ret = cre2_match(re, text_buf, text_len, 0, text_len, CRE2_UNANCHORED, match_arr, 4);
    set_matches = cre2_set_match(set, text_buf, (size_t)text_len, set_match_indices, (size_t)(sizeof(set_match_indices)/sizeof(set_match_indices[0])));
    re_pat = cre2_pattern(re);

    // step 5: Validate / compute lightweight checksum
    computed_check = (long)pattern_len + (long)text_len + (long)set_add_ret + (long)set_compile_ret
                     + (long)re_match_ret + (long)set_matches + (long)(re != NULL)
                     + (long)(re_pat != NULL) + (long)match_arr[0].length + (long)(opt != NULL)
                     + (long)(set != NULL);
    (void)computed_check;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}