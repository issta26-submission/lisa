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
//<ID> 926
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
    const char pm_pattern[] = "(ab)(cd)?";
    const int pm_pattern_len = (int)(sizeof(pm_pattern) - 1);
    const char set_pattern[] = "alpha|beta|gamma";
    const char text_buf[] = "prefix 12345 suffix abcd and alpha";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_options_t * opt = NULL;
    cre2_set * set = NULL;
    cre2_encoding_t enc = CRE2_UNKNOWN;
    cre2_string_t easy_match_arr[2];
    cre2_string_t pm_input[1];
    cre2_string_t pm_match_arr[3];
    int set_matches[4];
    int easy_res = 0;
    int pm_res = 0;
    int add_res = 0;
    int compile_res = 0;
    size_t set_match_count = 0;
    long computed_check = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_one_line(opt, 1);
    enc = cre2_opt_encoding(opt);

    // step 3: Core operations
    memset(easy_match_arr, 0, sizeof(easy_match_arr));
    easy_res = cre2_easy_match(easy_pattern, easy_pattern_len, text_buf, text_len, easy_match_arr, 2);

    pm_input[0].data = text_buf;
    pm_input[0].length = text_len;
    memset(pm_match_arr, 0, sizeof(pm_match_arr));
    pm_res = cre2_partial_match(pm_pattern, pm_input, pm_match_arr, 3);

    set = cre2_set_new(opt, CRE2_UNANCHORED);
    add_res = cre2_set_add_simple(set, set_pattern);
    compile_res = cre2_set_compile(set);
    memset(set_matches, 0, sizeof(set_matches));
    set_match_count = cre2_set_match(set, text_buf, (size_t)text_len, set_matches, 4);

    // step 4: Validate / compute lightweight checksum
    computed_check = (long)easy_pattern_len + (long)enc + (long)easy_res + (long)pm_res + (long)pm_match_arr[0].length + (long)add_res + (long)compile_res + (long)set_match_count + (long)set_matches[0];
    (void)computed_check;

    // step 5: Cleanup
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}