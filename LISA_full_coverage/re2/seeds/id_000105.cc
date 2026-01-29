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
//<ID> 105
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cre2_api_sequence() {
    // step 1: Declarations
    const char pattern[] = "(foo)";
    const int pattern_len = (int)(sizeof(pattern) - 1);
    const char input_text[] = "here is foo in text";
    const int input_text_len = (int)(sizeof(input_text) - 1);
    cre2_options_t * opt = NULL;
    cre2_encoding_t enc = CRE2_UNKNOWN;
    int word_boundary_flag = 0;
    const int nmatch = 2;
    cre2_string_t matches[2];
    cre2_range_t ranges[2];
    int easy_res = 0;

    // step 2: Setup
    memset(&matches, 0, sizeof(matches));
    memset(&ranges, 0, sizeof(ranges));
    opt = cre2_opt_new();

    // step 3: Configure
    cre2_opt_set_word_boundary(opt, 1);
    word_boundary_flag = cre2_opt_word_boundary(opt);
    enc = cre2_opt_encoding(opt);

    // step 4: Operate
    easy_res = cre2_easy_match(pattern, pattern_len, input_text, input_text_len, &matches[0], nmatch);
    cre2_strings_to_ranges(input_text, &ranges[0], &matches[0], nmatch);

    // step 5: Validate
    (void)word_boundary_flag;
    (void)enc;
    (void)easy_res;
    (void)matches;
    (void)ranges;
    (void)input_text;
    (void)pattern;

    // step 6: Cleanup
    cre2_opt_delete(opt);

    // API sequence test completed successfully
    return 66;
}