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
//<ID> 928
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
    const char partial_pattern[] = "(num)([0-9]+)";
    const char set_pattern_simple[] = "alpha|beta|gamma";
    char text_buf[] = "alpha 123 beta 456 gamma 789";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_options_t * opt = NULL;
    cre2_set * set = NULL;
    cre2_encoding_t encoding = CRE2_UNKNOWN;
    cre2_string_t easy_matches[2];
    cre2_string_t partial_matches[3];
    cre2_string_t input_text;
    int set_match_indices[8];
    size_t set_match_count = 0;
    int easy_res = 0;
    int partial_res = 0;
    int add_res = 0;
    int compile_res = 0;
    long computed_check = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_one_line(opt, 1);
    encoding = cre2_opt_encoding(opt);
    /* cre2_set_new expects a cre2_anchor_t for the second argument; cast 0 to that type */
    set = cre2_set_new(opt, (cre2_anchor_t)0);
    add_res = cre2_set_add_simple(set, set_pattern_simple);
    compile_res = cre2_set_compile(set);

    // step 3: Core operations
    memset(easy_matches, 0, sizeof(easy_matches));
    memset(partial_matches, 0, sizeof(partial_matches));
    memset(set_match_indices, 0, sizeof(set_match_indices));
    input_text.data = text_buf;
    input_text.length = text_len;
    easy_res = cre2_easy_match(easy_pattern, easy_pattern_len, text_buf, text_len, easy_matches, 2);
    partial_res = cre2_partial_match(partial_pattern, &input_text, partial_matches, 3);
    set_match_count = cre2_set_match(set, text_buf, (size_t)text_len, set_match_indices, (size_t)(sizeof(set_match_indices) / sizeof(set_match_indices[0])));

    // step 4: Validate / compute lightweight checksum
    computed_check = (long)easy_res + (long)partial_res + (long)easy_matches[0].length + (long)partial_matches[0].length + (long)encoding + (long)set_match_count + (long)add_res + (long)compile_res;
    (void)computed_check;

    // step 5: Cleanup
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // step 6: API sequence test completed successfully
    return 66;
}