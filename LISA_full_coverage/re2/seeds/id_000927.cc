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
//<ID> 927
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
    const char partial_pattern[] = "num:([0-9]+)";
    const int partial_pattern_len = (int)(sizeof(partial_pattern) - 1);
    const char set_pattern[] = "alpha|beta|gamma";
    const char text1[] = "abc def ghi";
    const int text1_len = (int)(sizeof(text1) - 1);
    const char text2[] = "num:12345 and tail";
    cre2_options_t * opt = NULL;
    cre2_set * set = NULL;
    cre2_string_t easy_matches[3];
    cre2_string_t partial_input;
    cre2_string_t partial_outputs[2];
    int set_match_indices[4];
    int easy_res = 0;
    int partial_res = 0;
    size_t set_matches = 0;
    cre2_encoding_t enc = CRE2_UNKNOWN;
    long computed_check = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_one_line(opt, 1);
    enc = cre2_opt_encoding(opt);

    // step 3: Core operations
    memset(easy_matches, 0, sizeof(easy_matches));
    easy_res = cre2_easy_match(easy_pattern, easy_pattern_len, text1, text1_len, easy_matches, 3);

    partial_input.data = text2;
    partial_input.length = (int)(sizeof(text2) - 1);
    memset(partial_outputs, 0, sizeof(partial_outputs));
    partial_res = cre2_partial_match(partial_pattern, &partial_input, partial_outputs, 2);

    set = cre2_set_new(opt, CRE2_UNANCHORED);
    cre2_set_add_simple(set, set_pattern);
    cre2_set_compile(set);
    memset(set_match_indices, -1, sizeof(set_match_indices));
    set_matches = cre2_set_match(set, text1, (size_t)text1_len, set_match_indices, 4);

    // step 4: Validate / compute lightweight checksum
    computed_check = (long)enc + (long)easy_res + (long)partial_res + (long)set_matches + (long)easy_matches[0].length + (long)partial_outputs[0].length + (long)set_match_indices[0];
    (void)computed_check;

    // step 5: Cleanup
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}