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
//<ID> 921
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
    const char partial_pattern[] = "(foo)(bar)?";
    const int partial_pattern_len = (int)(sizeof(partial_pattern) - 1);
    const char set_pattern[] = "alpha|beta";
    const char text_buf[] = "alpha 123 foo bar beta";
    const int text_len = (int)(sizeof(text_buf) - 1);

    cre2_options_t * opt = NULL;
    cre2_set * set = NULL;

    cre2_string_t easy_matches[3];
    cre2_string_t partial_matches[3];
    cre2_string_t text_str;
    int set_match_indices[8];

    int easy_res = 0;
    int partial_res = 0;
    int add_res = 0;
    int compile_res = 0;
    size_t set_res = 0;
    int encoding_val = 0;
    long checksum = 0;

    // step 2: Setup
    opt = cre2_opt_new();
    cre2_opt_set_one_line(opt, 1);
    encoding_val = (int)cre2_opt_encoding(opt);

    // step 3: Core operations
    memset(easy_matches, 0, sizeof(easy_matches));
    memset(partial_matches, 0, sizeof(partial_matches));
    memset(set_match_indices, 0, sizeof(set_match_indices));

    text_str.data = text_buf;
    text_str.length = text_len;

    easy_res = cre2_easy_match(easy_pattern, easy_pattern_len, text_buf, text_len, easy_matches, 3);
    partial_res = cre2_partial_match(partial_pattern, &text_str, partial_matches, 3);

    set = cre2_set_new(opt, CRE2_UNANCHORED);
    add_res = cre2_set_add_simple(set, set_pattern);
    compile_res = cre2_set_compile(set);
    set_res = cre2_set_match(set, text_buf, text_len, set_match_indices, 8);

    // step 4: Validate / compute lightweight checksum
    checksum = 0;
    checksum += (long)easy_pattern_len;
    checksum += (long)partial_pattern_len;
    checksum += (long)easy_res;
    checksum += (long)partial_res;
    checksum += (long)encoding_val;
    checksum += (long)add_res;
    checksum += (long)compile_res;
    checksum += (long)set_res;
    checksum += (long)easy_matches[0].length;
    checksum += (long)partial_matches[0].length;
    (void)checksum;

    // step 5: Cleanup
    cre2_set_delete(set);
    cre2_opt_delete(opt);

    // step 6: API sequence test completed successfully
    // API sequence test completed successfully
    return 66;
}