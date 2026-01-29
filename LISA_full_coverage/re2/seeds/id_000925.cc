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
//<ID> 925
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
    const char partial_pattern[] = "(num:)([0-9]+)";
    const int partial_pattern_len = (int)(sizeof(partial_pattern) - 1);
    char text_buf[] = "num:12345 alpha beta gamma";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_options_t * opt = NULL;
    cre2_set * set = NULL;
    cre2_string_t easy_matches[3];
    cre2_string_t partial_matches[4];
    cre2_string_t text_str;
    int set_matches_idx[8];
    size_t set_match_count = 0;
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
    easy_res = cre2_easy_match(easy_pattern, easy_pattern_len, text_buf, text_len, easy_matches, 2);
    text_str.data = text_buf;
    text_str.length = text_len;
    memset(partial_matches, 0, sizeof(partial_matches));
    partial_res = cre2_partial_match(partial_pattern, &text_str, partial_matches, 3);
    set = cre2_set_new(opt, CRE2_UNANCHORED);
    cre2_set_add_simple(set, "alpha");
    cre2_set_add_simple(set, "beta");
    cre2_set_add_simple(set, "gamma");
    cre2_set_compile(set);
    memset(set_matches_idx, 0, sizeof(set_matches_idx));
    set_match_count = cre2_set_match(set, text_buf, (size_t)text_len, set_matches_idx, 8);

    // step 4: Validate / compute lightweight checksum
    computed_check = (long)enc + (long)easy_res + (long)partial_res + (long)easy_matches[0].length + (long)partial_matches[0].length + (long)set_match_count + (long)text_str.length;
    (void)computed_check;

    // step 5: Cleanup
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // step 6: API sequence test completed successfully
    return 66;
}