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
//<ID> 922
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char set_pattern[] = "alpha|beta";
    const char easy_pattern[] = "([a-z]+)";
    const int easy_pattern_len = (int)(sizeof(easy_pattern) - 1);
    const char partial_pat[] = "num ([0-9]+)";
    const char text_buf[] = "alpha 123 beta gamma 456 num 789";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_options_t * opt = NULL;
    cre2_set * set = NULL;
    cre2_string_t match_arr[4];
    cre2_string_t text_wrapper;
    int set_matches[8];
    size_t set_match_count = 0;
    int easy_res = 0;
    int partial_res = 0;
    cre2_encoding_t enc = CRE2_UNKNOWN;
    long computed_check = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_one_line(opt, 1);
    enc = cre2_opt_encoding(opt);
    set = cre2_set_new(opt, CRE2_UNANCHORED);
    cre2_set_add_simple(set, set_pattern);
    cre2_set_compile(set);

    // step 3: Core operations
    memset(match_arr, 0, sizeof(match_arr));
    memset(set_matches, 0, sizeof(set_matches));
    text_wrapper.data = text_buf;
    text_wrapper.length = text_len;
    easy_res = cre2_easy_match(easy_pattern, easy_pattern_len, text_buf, text_len, match_arr, 2);
    partial_res = cre2_partial_match(partial_pat, &text_wrapper, match_arr + 2, 2);
    set_match_count = cre2_set_match(set, text_buf, (size_t)text_len, set_matches, (size_t)(sizeof(set_matches)/sizeof(set_matches[0])));

    // step 4: Validate / compute lightweight checksum
    computed_check = (long)enc + (long)easy_res + (long)partial_res + (long)set_match_count + (long)match_arr[0].length + (long)match_arr[2].length + (long)set_matches[0];
    (void)computed_check;

    // step 5: Cleanup
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}