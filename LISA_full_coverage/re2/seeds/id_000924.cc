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
//<ID> 924
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
    const char partial_pattern[] = "(word)([A-Za-z]+)?";
    const int partial_pattern_len = (int)(sizeof(partial_pattern) - 1);
    const char set_pattern_simple[] = "alpha|beta|gamma";
    const int text_len_int = (int)28;
    const size_t text_len_size = (size_t)28;
    char text_buf[28] = "alpha 123 beta gamma 456";
    cre2_options_t * opt = NULL;
    cre2_encoding_t enc = CRE2_UNKNOWN;
    cre2_string_t easy_matches[2];
    cre2_string_t partial_matches[3];
    cre2_string_t text_str;
    cre2_set * set = NULL;
    int set_match_idx[4];
    size_t set_match_count = 0;
    int easy_res = 0;
    int partial_res = 0;
    int add_res = 0;
    int compile_res = 0;
    long computed_check = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_one_line(opt, 1);
    enc = cre2_opt_encoding(opt);

    // step 3: Core operations - simple easy match and partial match
    memset(easy_matches, 0, sizeof(easy_matches));
    easy_res = cre2_easy_match(easy_pattern, easy_pattern_len, text_buf, text_len_int, easy_matches, 2);
    text_str.data = text_buf;
    text_str.length = text_len_int;
    memset(partial_matches, 0, sizeof(partial_matches));
    partial_res = cre2_partial_match(partial_pattern, &text_str, partial_matches, 3);

    // step 4: Core operations - set creation and matching
    set = cre2_set_new(opt, CRE2_UNANCHORED);
    add_res = cre2_set_add_simple(set, set_pattern_simple);
    compile_res = cre2_set_compile(set);
    memset(set_match_idx, 0, sizeof(set_match_idx));
    set_match_count = cre2_set_match(set, text_buf, text_len_size, set_match_idx, 4);

    // step 5: Validate / compute lightweight checksum
    computed_check = (long)easy_pattern_len + (long)partial_pattern_len + (long)easy_res + (long)partial_res + (long)enc + (long)add_res + (long)compile_res + (long)set_match_count + (long)set_match_idx[0] + (long)easy_matches[0].length + (long)partial_matches[0].length;
    (void)computed_check;

    // step 6: Cleanup
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}