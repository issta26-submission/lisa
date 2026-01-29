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
//<ID> 1283
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "id:([0-9]+)-item";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    char text_buf[] = "prefix id:12345-item suffix";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_options_t * opt = NULL;
    cre2_regexp_t * re = NULL;
    cre2_set * set = NULL;
    cre2_string_t matchstr;
    int match_arr[4];
    size_t set_matches = 0;
    int match_ret = 0;
    const char * re_pat = NULL;
    long computed_check = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    set = cre2_set_new(opt, CRE2_UNANCHORED);
    memset(&matchstr, 0, sizeof(matchstr));
    memset(match_arr, 0, sizeof(match_arr));
    matchstr.data = NULL;
    matchstr.length = 0;

    // step 3: Configure
    cre2_opt_set_case_sensitive(opt, 1);
    cre2_set_add_simple(set, pattern);
    cre2_set_compile(set);
    re = cre2_new(pattern, pattern_len, opt);

    // step 4: Core operations
    re_pat = cre2_pattern(re);
    match_ret = cre2_match(re, text_buf, text_len, 0, text_len, CRE2_UNANCHORED, &matchstr, 2);
    set_matches = cre2_set_match(set, text_buf, (size_t)text_len, match_arr, (size_t)4);

    // step 5: Validate / compute lightweight checksum
    computed_check = (long)pattern_len + (long)text_len + (long)set_matches + (long)match_ret
                     + (long)match_arr[0] + (long)matchstr.length + (long)(re != NULL)
                     + (long)(re_pat != NULL) + (long)(set != NULL) + (long)(opt != NULL);
    (void)computed_check;

    // step 6: Cleanup
    cre2_delete(re);
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}