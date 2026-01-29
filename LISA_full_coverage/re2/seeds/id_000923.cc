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
//<ID> 923
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char easy_pattern[] = "([a-z]+)";
    const int easy_pattern_len = (int)(sizeof(easy_pattern) - 1);
    const char partial_pattern[] = "prefix([0-9]+)suffix";
    const char set_pat1[] = "alpha";
    const char set_pat2[] = "prefix([0-9]+)suffix";
    char text_buf[] = "prefix123suffix and alpha inside";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_options_t * opt = NULL;
    cre2_set * set = NULL;
    cre2_string_t match_arr[4];
    cre2_string_t input_str;
    int add_res1 = 0;
    int add_res2 = 0;
    int compile_res = 0;
    int easy_res = 0;
    int partial_res = 0;
    size_t set_matches = 0;
    int set_match_indices[8];
    int encoding_val = 0;
    long computed_check = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_one_line(opt, 1);
    encoding_val = (int)cre2_opt_encoding(opt);
    set = cre2_set_new(opt, CRE2_UNANCHORED);
    add_res1 = cre2_set_add_simple(set, set_pat1);
    add_res2 = cre2_set_add_simple(set, set_pat2);
    compile_res = cre2_set_compile(set);

    // step 3: Core operations
    memset(match_arr, 0, sizeof(match_arr));
    input_str.data = text_buf;
    input_str.length = text_len;
    easy_res = cre2_easy_match(easy_pattern, easy_pattern_len, text_buf, text_len, match_arr, 1);
    partial_res = cre2_partial_match(partial_pattern, &input_str, match_arr, 2);
    set_matches = cre2_set_match(set, text_buf, (size_t)text_len, set_match_indices, 8);

    // step 4: Validate / compute lightweight checksum
    computed_check = (long)easy_res + (long)partial_res + (long)set_matches + (long)match_arr[0].length + (long)encoding_val + (long)add_res1 + (long)add_res2 + (long)compile_res + (long)set_match_indices[0];
    (void)computed_check;

    // step 5: Cleanup
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}