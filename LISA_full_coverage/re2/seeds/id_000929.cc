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
//<ID> 929
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
    const char text_buf[] = "prefix 123 middle 456 suffix";
    const int text_len = (int)(sizeof(text_buf) - 1);
    cre2_options_t * opt = NULL;
    cre2_set * set = NULL;
    cre2_encoding_t enc = CRE2_UNKNOWN;
    cre2_string_t easy_matches[2];
    cre2_string_t input;
    cre2_string_t partial_matches[3];
    int set_match_indices[4];
    size_t set_count = 0;
    int easy_res = 0;
    int partial_res = 0;
    int add_res = 0;
    long computed_check = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_one_line(opt, 1);
    enc = cre2_opt_encoding(opt);

    // step 3: Core operations
    set = cre2_set_new(opt, CRE2_UNANCHORED);
    add_res = cre2_set_add_simple(set, "123|456");
    memset(easy_matches, 0, sizeof(easy_matches));
    easy_res = cre2_easy_match(easy_pattern, easy_pattern_len, text_buf, text_len, easy_matches, 2);
    input.data = text_buf;
    input.length = text_len;
    memset(partial_matches, 0, sizeof(partial_matches));
    partial_res = cre2_partial_match("prefix\\s*([0-9]+)", &input, partial_matches, 2);
    memset(set_match_indices, -1, sizeof(set_match_indices));
    set_count = cre2_set_match(set, text_buf, (size_t)text_len, set_match_indices, 4);

    // step 4: Validate / compute lightweight checksum
    computed_check = (long)enc + (long)easy_res + (long)partial_res + (long)set_count + (long)easy_matches[0].length + (long)partial_matches[0].length + (long)add_res + (long)set_match_indices[0];
    (void)computed_check;

    // step 5: Cleanup
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}