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
//<ID> 920
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char easy_pattern[] = "([0-9]+)";
    const int easy_pattern_len = (int)(sizeof(easy_pattern) - 1);
    const char partial_pattern[] = "(num:([0-9]+))";
    const int partial_pattern_len = (int)(sizeof(partial_pattern) - 1);
    const char set_pattern[] = "alpha|beta|gamma";
    const char text_buf[] = "alpha 123 num:456 beta";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_options_t * opt = NULL;
    cre2_set * set = NULL;
    cre2_string_t easy_matches[2];
    cre2_string_t partial_matches[3];
    cre2_string_t text_cre2;
    int set_match_arr[4];
    size_t set_matches_count = 0;
    int easy_res = 0;
    int partial_res = 0;
    cre2_encoding_t enc = CRE2_UNKNOWN;
    long computed_check = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_one_line(opt, 1);
    enc = cre2_opt_encoding(opt);

    // step 3: Core operations
    memset(easy_matches, 0, sizeof(easy_matches));
    memset(partial_matches, 0, sizeof(partial_matches));
    memset(set_match_arr, 0, sizeof(set_match_arr));
    text_cre2.data = text_buf;
    text_cre2.length = text_len;
    easy_res = cre2_easy_match(easy_pattern, easy_pattern_len, text_buf, text_len, easy_matches, 2);
    partial_res = cre2_partial_match(partial_pattern, &text_cre2, partial_matches, 3);
    set = cre2_set_new(opt, CRE2_UNANCHORED);
    cre2_set_add_simple(set, set_pattern);
    cre2_set_compile(set);
    set_matches_count = cre2_set_match(set, text_buf, (size_t)text_len, set_match_arr, 4);

    // step 4: Validate / compute lightweight checksum
    computed_check = (long)easy_pattern_len + (long)partial_pattern_len + (long)easy_res + (long)partial_res + (long)enc + (long)easy_matches[0].length + (long)partial_matches[0].length + (long)set_matches_count + (long)set_match_arr[0];
    (void)computed_check;

    // step 5: Cleanup
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // step 6: API sequence test completed successfully
    return 66;
}